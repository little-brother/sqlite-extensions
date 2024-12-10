The extension provides read-only access to [Apache Parquet](https://parquet.apache.org/)-files as virtual tables.

### Usage

* **parquet (filepath)**<br>
  ```
  create virtual table demo using parquet('demo.parquet');
  select * from demo;
  ```

### Download
[parquet.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/parquet-x32.zip)<br>
[parquet.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/parquet-x64.zip)

### How to build by Golang/mingw64
1. Download and install [Golang](https://golang.org/dl/) 1.20+
2. Download and unpack [mingw64](https://winlibs.com/) 12+
3. Move to `src`-folder
   ```bash
   cd src
    ```
4. Download go-dependencies
   ```bash
   go get -v ./
    ```
5. Build 32-bit extension 
   ```bash
   set PATH=c:\mingw64\mingw32\bin\;%PATH% 
   set GOARCH=386 
   go build -o parquet.dll -buildmode=c-shared -ldflags="-s -w -extldflags=-static"
   ```
6. Build 64-bit extension 
   ```bash
   set PATH=c:\mingw64\mingw64\bin\;%PATH% 
   set GOARCH=amd64
   go build -o parquet.dll -buildmode=c-shared -ldflags="-s -w -extldflags=-static"
   ```
<details>
  <summary>Windows 7 support</summary>

  The extension uses [`parquet-go`](https://github.com/parquet-go/parquet-go)-module. The latest version of the module requires Go 1.22+ with slices. But Go 1.22+ produces non-working binaries for Windows 7. To bypass it you can use Go 1.20 and `parquet-go` 0.21.0.<br>
  Use `go get github.com/parquet-go/parquet-go@v0.21.0` to obtain this version.
</details>