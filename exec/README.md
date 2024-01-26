The extension contains one function to grab command output as a result.

### Usage

* **exec (cmd, codepage = 'UTF16')**<br>
  Execute a shell command and return console output as a result.<br>
  `Codepage` defines a code page of command output and it's a one of: `ANSI`, `CP437`, `UTF7`, `UTF8`, `UTF16`. If `cmd` starts from `powershell` and `codepage` is empty then `CP437` is used.
  ```
  select exec('powershell -nologo "Get-Content C:/data.txt"'); -- as one value
  select * from exec('powershell Get-Content C:/data.txt -Encoding UTF8', 'CP437'); -- as a table
  ```

### Download
[exec.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/exec-x32.zip)<br>
[exec.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/exec-x64.zip)

### How to build by mingw64
```
set PATH=c:\mingw64\mingw64\bin\;%PATH%
gcc -I ./include -shared ./src/exec.c -o exec.dll -s -static
```