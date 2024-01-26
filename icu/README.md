Add partial support for national symbols by redefinition lower and upper functions.<br>
Author: unknown.



### Usage

* **upper (str)**<br>
  Return uppercase for string.
  ```
  select upper('ы'); --> Ы
  ```

* **lower (str)**<br>
  Return lowercase for string.
  ```
  select lower('Ы'); --> ы
  ```

Be aware, `like`-operator is changed too
```
select 'привет' like 'Прив_т'; --> the result is 1 with the extension and 0 without
```


### Download
[icu.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/icu-x32.zip)<br>
[icu.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/icu-x64.zip)

### How to build by mingw64
```
set PATH=c:\mingw64\mingw64\bin\;%PATH%
gcc -I ./include -shared ./src/icu.c -o icu.dll -s -static
```