// Based on https://github.com/julien040/anyquery/blob/main/module/read_parquet.go
package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"os"
	"strings"
	"regexp"

	"go.riyazali.net/sqlite"
	"github.com/parquet-go/parquet-go"
	"github.com/edsrzf/mmap-go"
	"github.com/gammazero/deque"
)

type ParquetModule struct {
}

type ParquetTable struct {
	mmap   mmap.MMap
	column map[int]string
}

type ParquetCursor struct {
	column    map[int]string
	reader    *parquet.GenericReader[any]
	rowBuffer *deque.Deque[map[string]interface{}]
	rowID     int64
	eof bool
	noMoreRows bool
}

const rowToRequestPerBatch = 16

type argParam struct {
	name  string
	value *string
}

var argRegExp *regexp.Regexp = regexp.MustCompile(`^\s*['"]?([^= '"]+?)['"]?\s*=\s*['"]?(.*?)['"]?\s*$`)

func parseArgs(params []argParam, args []string) {
	for i := 0; i < len(args); i++ {
		if args[i] == "" {
			continue
		}

		matches := argRegExp.FindStringSubmatch(args[i])
		if matches == nil {
			continue
		}

		matches[1] = strings.ToLower(matches[1])

		for j := 0; j < len(params); j++ {
			if matches[1] == params[j].name {
				*params[j].value = matches[2]
				break
			}
		}
	}
}

var sqliteValidName *regexp.Regexp = regexp.MustCompile(`[^\p{L}\p{N}_]+`)

func transformSQLiteValidName(s string) string {
	s = strings.TrimSpace(s)
	spaceRemoved := strings.Map(func(r rune) rune {
		if r == ' ' || r == '.' || r == '-' || r == '/' {
			return '_'
		}
		return r
	}, s)

	return sqliteValidName.ReplaceAllString(spaceRemoved, "")
}

func openMmapedFile(filePath string) (mmap.MMap, error) {
	file, err := os.Open(filePath)
	if err != nil {
		return nil, err
	}

	return mmap.Map(file, mmap.RDONLY, 0)
}

func (m *ParquetModule) Create(c *sqlite.Conn, args []string, declare func(string) error) (sqlite.VirtualTable, error) {
	return m.Connect(c, args, declare)
}

func (v *ParquetModule) DestroyModule() {}

func (m *ParquetModule) Connect(c *sqlite.Conn, args []string, declare func(string) error) (sqlite.VirtualTable, error) {
	fileName := ""
	if len(args) > 3 {
		fileName = strings.Trim(args[3], "' \"")
	}

	params := []argParam{
		{"file", &fileName},
		{"file_name", &fileName},
		{"filename", &fileName},
		{"src", &fileName},
		{"path", &fileName},
		{"file_path", &fileName},
		{"filepath", &fileName},
		{"url", &fileName},
	}

	parseArgs(params, args)

	if fileName == "" {
		return nil, fmt.Errorf("missing file to open. Specify it with SELECT * FROM read_parquet('file.parquet')")
	}

	mmap := mmap.MMap{}
	var err error

	mmap, err = openMmapedFile(fileName)
	if err != nil {
		return nil, fmt.Errorf("failed to open the file: %s", err)
	}

	byteReader := bytes.NewReader(mmap)

	reader := parquet.NewGenericReader[any](byteReader)

	column := make(map[int]string)

	sqlSchema := strings.Builder{}
	sqlSchema.WriteString("CREATE TABLE parquet (")
	for i, field := range reader.Schema().Fields() {
		if i > 0 {
			sqlSchema.WriteString(", ")
		}
		sqlSchema.WriteRune('"')
		sqlSchema.WriteString(transformSQLiteValidName(field.Name()))
		sqlSchema.WriteRune('"')
		sqlSchema.WriteString(" ")
		switch field.Type().String() {
		case "BOOLEAN":
			sqlSchema.WriteString("INTEGER")
		case "INT32", "INT64", "INT96", "INT(64,true)", "INT(64,false)", "INT(96,true)", "INT(96,false)", "DATE":
			sqlSchema.WriteString("INTEGER")
		case "FLOAT", "DOUBLE":
			sqlSchema.WriteString("REAL")
		case "BYTE_ARRAY", "FIXED_LEN_BYTE_ARRAY", "STRING":
			sqlSchema.WriteString("TEXT")
		default:
			sqlSchema.WriteString("TEXT")
		}

		column[i] = field.Name()
	}
	sqlSchema.WriteString(");")
	declare(sqlSchema.String())

	return &ParquetTable{mmap: mmap, column: column}, nil
}

func (t *ParquetTable) Open() (sqlite.VirtualCursor, error) {
	reader := parquet.NewGenericReader[any](bytes.NewReader(t.mmap))
	return &ParquetCursor{
		column:    t.column,
		reader:    reader,
		rowBuffer: deque.New[map[string]interface{}](),
	}, nil
}

func (t *ParquetTable) Disconnect() error {
	if t.mmap != nil {
		return t.mmap.Unmap()
	}
	return nil
}

func (t *ParquetTable) Destroy() error {
	return nil
}

func (t *ParquetTable) BestIndex(cst *sqlite.IndexInfoInput) (*sqlite.IndexInfoOutput, error) {
	return &sqlite.IndexInfoOutput{
		IndexNumber: 1,
		ConstraintUsage: make([]*sqlite.ConstraintUsage, len(cst.Constraints)),
	}, nil
}

func (t *ParquetCursor) Filter(idxNum int, idxStr string, vals ...sqlite.Value) error {
	return t.requestRows()
}

func (t *ParquetCursor) requestRows() error {
	buffer := make([]any, rowToRequestPerBatch)
	rowFound, err := t.reader.Read(buffer)
	if err == io.EOF {
		t.noMoreRows = true
	} else if err != nil {
		return err
	}
	for i := 0; i < rowFound; i++ {
		if mapVal, ok := buffer[i].(map[string]interface{}); ok {
			t.rowBuffer.PushBack(mapVal)
		}
	}

	return nil
}

func (t *ParquetCursor) Next() error {
	if t.rowBuffer.Len() != 0 {
		t.rowBuffer.PopFront()
	}
	if t.rowBuffer.Len() == 0 {
		if t.noMoreRows {
			t.eof = true
			return nil
		}
		err := t.requestRows()
		if err != nil {
			return err
		}
	}

	if t.rowBuffer.Len() == 0 && t.noMoreRows {
		t.eof = true
		return nil
	}

	t.rowID++
	return nil
}

func (t *ParquetCursor) Column(context *sqlite.VirtualTableContext, col int) error {
	colName, ok := t.column[col]
	if !ok {
		context.ResultNull()
		return nil
	}
	val, ok := t.rowBuffer.Front()[colName]
	if !ok {
		context.ResultNull()
		return nil
	}

	switch valParsed := val.(type) {
	case bool:
		if valParsed {
			context.ResultInt(1)
		} else {
			context.ResultInt(0)
		}
	case int:
		context.ResultInt(valParsed)
	case int8:
		context.ResultInt(int(valParsed))
	case int16:
		context.ResultInt(int(valParsed))
	case int32:
		context.ResultInt(int(valParsed))
	case int64:
		context.ResultInt64(valParsed)
	case uint64:
		context.ResultInt64(int64(valParsed))
	case float32:
		context.ResultFloat(float64(valParsed))
	case float64:
		context.ResultFloat(valParsed)
	case string:
		context.ResultText(valParsed)
	case []byte:
		context.ResultBlob(valParsed)
	case map[string]interface{}:
		marshaled, err := json.Marshal(valParsed)
		if err != nil {
			context.ResultNull()
		} else {
			context.ResultText(string(marshaled))
		}
	default:
		context.ResultNull()
	}

	return nil
}

func (t *ParquetCursor) Eof() bool {
	return t.eof
}

func (t *ParquetCursor) Rowid() (int64, error) {
	return t.rowID, nil
}

func (t *ParquetCursor) Close() error {
	return t.reader.Close()
}

func init() {
	sqlite.Register(func(api *sqlite.ExtensionApi) (sqlite.ErrorCode, error) {
		if err := api.CreateModule("parquet", &ParquetModule{},
			sqlite.EponymousOnly(false)); err != nil {
			return sqlite.SQLITE_ERROR, err
		}
		return sqlite.SQLITE_OK, nil
	})
}

func main() {}