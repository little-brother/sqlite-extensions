The extension allows to read and write files on a disk.<br>
Author: [D. Richard Hipp](https://sqlite.org/src/file/ext/misc/fileio.c).

### Usage

* **writefile (path, data [, mode [, mtime]])**<br>
  Write data as blob to file. Mode should correspond to a POSIX mode value (file type + permissions, as returned in the stat.st_modefield by the stat() system call).<br>
  ```
  select writefile ('D:/blob.bin', x'37e79f'); --> 3
  ```

* **readfile (path)**<br>
  Read and return the contents of file FILE (type blob) from disk.
  ```
  select readfile ('D:/blob.bin'); --> BLOB
  ```

* **fsdir (path [, dir])**<br>
  Return one row for the directory, and one row for each file within the hierarchy rooted at path.
  ```
  select * from fsdir ('C:/Temp');
  ```

### Download
[fileio.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/fileio-x32.zip)<br>
[fileio.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/fileio-x64.zip)