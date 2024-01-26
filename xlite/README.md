The extension provides access to Excel(.xlsx, .xls) and ODS (.ods) spreadsheets as virtual tables.<br>
Author: [Sergey Khabibullin (x2bool)](https://github.com/x2bool/xlite).

### Usage
```
create virtual table test_data using xlite (
  filename 'D:/MyBook.xlsx',
  worksheet 'Sheet1',
  range 'A2:F', -- optional
  colnames '1' -- optional
);
```

### Download
[xlite.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/xlite-x32.zip)<br>
[xlite.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/xlite-x64.zip)