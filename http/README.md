The extension for making HTTP requests.<br>
You can use [`exec`](https://github.com/little-brother/sqlite-extensions/tree/main/exec)-extension with `curl` as an alternative for special cases.

### Usage

* **http_request(url, [method = GET [, headers [, body [, response_type [, as_blob]]]])**<br>
  Make http request.<br>
  `method` should be one of: `GET`, `POST`, `DELETE`, `PUT`, `PATCH`.<br>
  `response_type` is one of: `BODY` (default), `HEADERS`, `COOKIES`, `STATUS`.<br><br>
  The headers for non-empty body is set as `Content-Type: application/x-www-form-urlencoded`.<br>
  Use `HEAD`-method to check that the url is alive.
  ```
  select http_request('httpbin.org/get');
  select http_request('https://httpbin.org/post', 'POST', null,'a=10&b=100') x; --> form: a = 10, b = 100
  select http_request('https://httpbin.org/post', 'POST', 'Content-Type: multipart/form-data\nboundary=AAABBBCCC','a=10&b=100') x; --> data: a=10&b=100
  ```

* **http_timeouts(resolve_timeout, connect_timeout, send_timeout, receive_timeout)**<br>
  Set request timeouts in milliseconds. Default timeouts are 1000 (1 sec).<br>
  ```
  select http_timeouts(100, 100, 100, 100);
  ```
* **http_encode(uri_component)**<br>
  Encode special characters in a URI component.
  ```
  select http_encode('абв'); --> %D0%B0%D0%B1%D0%B2

### Download
[http.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/http-x32.zip)<br>
[http.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/http-x64.zip)

### How to build by mingw64
```
set PATH=c:\mingw64\mingw64\bin\;%PATH%
gcc -I ./include -shared ./src/http.c -o http.dll -s -static -lwinhttp
```
### Troubleshooting
* On Windows 7 some sites can reject https-requestes due 12175 error. To fix it you should enable SSL and TLS by adding `HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Internet Settings\WinHttp`- QWORD-key with `4100000020` as a value. More info is [here](https://support.microsoft.com/en-us/topic/update-to-enable-tls-1-1-and-tls-1-2-as-default-secure-protocols-in-winhttp-in-windows-c4bd73d2-31d7-761e-0178-11268bb10392).
* If you often get a connection error, try to increase timeouts.
