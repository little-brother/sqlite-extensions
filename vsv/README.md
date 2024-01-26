The extension provides access to CSV files as virtual tables.<br>
Author: Keith Medcalf.

### Usage
`users.csv`
```
1,root,123
2,Bob,pass
3,Alice,abc
```

```
create virtual table users using vsv(
    filename = users.csv,
    schema = "create table users(id integer, email text, password text)",
    columns = 3,
    affinity = integer
);
```

### Download
[vsv.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/vsv-x32.zip)<br>
[vsv.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/vsv-x64.zip)

### How to build by mingw64
```
set PATH=c:\mingw64\mingw64\bin\;%PATH%
gcc -I ./include -shared ./src/vsv.c -o vsv.dll -s -static
```