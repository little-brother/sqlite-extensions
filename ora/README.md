The extension provides a bunch of helper functions.

### Usage

* **rownum(startBy)**<br>
  Return a row number in the resultset starting from a passed argument.
  ```
  select *, rownum(0) from mytable;
  ```

* **decode(expr, key1, value1, ke2, value2, ..., defValue)**<br>
  Compare expr to each key one by one. If expr is equal to a key, then returns the corresponding value.<br>
  If no match is found, then returns defValue. If defValue is omitted, then returns NULL.
  ```
  decode(1 < 2, false, 'NO', true, 'YES', '???'); --> YES
  ```

* **crc32(str)**<br>
  Calculate crc32 checksum
  ```
  select crc32('hello'); --> 907060870
  ```
* **md5(str)**<br>
  Calculate md5 checksum
  ```
  select md5('hello'); --> 5d41402abc4b2a76b9719d911017c592
  ```

* **base64_encode (str)**<br>
  Encode the given string with base64.
  ```
  select base64_encode('foobar'); --> Zm9vYmFy
  ```

* **base64_decode (str)**<br>
  Decode a base64 encoded string.
  ```
  select base64_encode('Zm9vYmFy'); --> foobar
  ```
* **strpart(str, delimiter, partno)**<br>
  Return substring for a delimiter and a part number.
  ```
  select strpart('ab-cd-ef', '-', 2); --> 'cd'
  select strpart('20.01.2021', '.', 3); --> 2021
  select strpart('20-01/20/21', '-/', -2); --> 20
  select strpart('D:/Docs/Book1.xls', '.', -2); --> Book1
  ```

* **conv(num, from_base, to_base)**<br>
  Return a string representation of convertion a number from one numeric base number system to another numeric base number system.<br>
  The minimum base is 2 and the maximum base is 36.<br>
  Only positive numbers are supported.
  ```
  select conv(15, 10, 2) --> 1111
  ```

* **tosize(nBytes)**<br>
  Return a human readable size.
  ```
  select tosize(1024); --> 1.00KB
  select tosize(2 * 1024 * 1024); --> 2.00MB
  ```

### Download
[ora.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/ora-x32.zip)<br>
[ora.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/ora-x64.zip)

### How to build by mingw64
```
set PATH=c:\mingw64\mingw64\bin\;%PATH%
gcc -I ./include -shared ./src/ora.c -o ora.dll -s -static
```