The extension implements functions that handling RFC-4122 UUIDs.<br>
Author: [D. Richard Hipp](https://sqlite.org/src/file/ext/misc/uuid.c).

### Usage

* **uuid ()**<br>
  Generate a version 4 UUID as a string.
  ```
  select uuid(); --> 0b3fb911-feba-47cf-af8a-e2094aec9f6f
  ```

* **uuid_str (uuid)**<br>
  Convert an uuid into a well-formed UUID string.
  ```
  select uuid_str('0b3fb911feba47cfaf8ae2094aec9f6f'); --> 0b3fb911-feba-47cf-af8a-e2094aec9f6f
  ```

* **uuid_blob (uuid)**<br>
  Convert an uuid into a 16-byte blob
  ```
  select uuid_blob('0b3fb911-feba-47cf-af8a-e2094aec9f6f'); --> Blob, 16b
  ```

### Download
[uuid.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/uuid-x32.zip)<br>
[uuid.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/uuid-x64.zip)