The special sqlite-gui extension is used for [scripting](https://github.com/little-brother/sqlite-gui/wiki#jinja-scripting).

### Usage

* **inja (template, arg1, arg2, ...)**<br>
  Parse jinia-like templates.
  ```
  select inja('{% set id = argv.1 %} {{ id }}', 10 ) --> 10
  select inja('select {{ argv.1 }}; select {{ argv.2 }}', 'a', 'b'); --> select a; select b;
  select inja('{% for i in range (2) %}select {{ i + 1 }}; {% endfor %}'); --> select 1; select 2;
  ``` 

### Download
[inja.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/inja-x32.zip)<br>
[inja.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/inja-x64.zip)

### How to build by mingw64
```
set PATH=c:\mingw64\mingw64\bin\;%PATH%
g++ -I ./include -shared ./src/inja.cpp ./include/inja.hpp ./include/json.hpp -o inja.dll -s -static -Os -std=c++17
```