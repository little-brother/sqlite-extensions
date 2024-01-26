Hashing, encoding and decoding functions.<br>
Author: [Anton Zhiyanov (nalgeon)](https://github.com/nalgeon/sqlean/blob/main/docs/crypto.md).



### Usage
* **md5(data)**<br>
  Calculate md5-checksum.
  ```
  select md5('hello'); --> BLOB, 16b
  ``` 

* **shaN(data)**<br>
  Calculate sha-checksum.
  ```
  select sha1('hello'); --> BLOB, 20b
  select sha256('hello'); --> BLOB, 32b
  select sha384('hello'); --> BLOB, 40b
  select sha512('hello'); --> BLOB, 64b
  ```
 
* **encode(data, algo)**<br>
  Encode binary data into a textual representation using the specified algorithm: `base32`, `base64`, `base85`, `hex` and `url`.
  ```
  select encode('hello', 'base64'); --> aGVsbG8=
  select encode('/hello?text=(ಠ_ಠ)', 'url'); --> %2Fhello%3Ft%3D%28%E0%B2%A0_%E0%B2%A0%29
  ``` 

* **decode(text, algo)**<br>
  Decode binary data from a textual representation using the specified algorithm: `base32`, `base64`, `base85`, `hex` and `url`. 
  ```
  select decode('aGVsbG8=', 'base64'); --> hello
  select decode('%2Fhello%3Ft%3D%28%E0%B2%A0_%E0%B2%A0%29', 'url'); --> /hello?t=(ಠ_ಠ)
  ``` 

### Download
[crypto.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/crypto-x32.zip)<br>
[crypto.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/crypto-x64.zip)