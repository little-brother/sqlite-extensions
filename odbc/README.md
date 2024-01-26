The extension provides a way to work with any databases through ODBC interface.

### Usage

* **odbc_read(connectionString, query, target)**<br>
  Read data via ODBC from external source and write it to SQLite table. If the target table doesn't exist, it'll be created.<br>
  Use `TABLES` as the query to obtain a table list.
  ```
  select odbc_read('Driver={Microsoft Text Driver (*.txt; *.csv)};Dbq=D:/csv/', 'select * from animals.csv', 'animals')
  ```

* **odbc_write(query, connectionString, target)**<br>
  Upload the query resultset from SQLite to a remote database. The target table must exists.
  ```
  select odbc_write('select * from animals where family = "Felidae"', 'DSN=Zoo.mdb', 'cats')
  ```

* **odbc_query(connectionString, query)**<br>
  Execute an query on a remote database e.g. to create or purge a table.<br>
  ```
  select odbc_query('DSN=Zoo', 'create table cats (id integer, name varchar2(50))')
  ```

* **odbc_dsn()**<br>
  Return local DSN list as json array: `{"result": ["MyData", "Csv", ...], ...}`
  ```
  select odbc_dsn()
  ```

Remarks
* A result of a function is a json: `{"result": "ok", ...}` on done and `{"error": "<msg>"}` on error.
* Some sources e.g. CSV-files are readonly via ODBC.
* Use 32bit ODBC manager `C:\Windows\SysWOW64\odbcad32.exe` to define a local DSN.
* You should install appropriate ODBC driver to gain access to [MySQL](https://dev.mysql.com/downloads/connector/odbc/), [PosgreSQL](https://odbc.postgresql.org/), [Mongo](https://github.com/mongodb/mongo-odbc-driver/releases), etc.

### Download
[odbc.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/odbc-x32.zip)<br>
[odbc.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/odbc-x64.zip)

### How to build by mingw64
```
set PATH=c:\mingw64\mingw64\bin\;%PATH%
gcc -I ./include -shared ./src/odbc.c -o odbc.dll -s -static -lodbc32
```