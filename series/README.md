The extension implements one table-valued-function for generating sequence values.<br>
Author: [D. Richard Hipp](https://sqlite.org/src/file/ext/misc/series.c).



### Usage

* **generate_series (start = 0, stop = Infinity, step = 1)**<br>
  Generate a series of values, from start to stop with a step size of one 

  ```
  select * from generate_series(0, 100, 5); --> 21 rows with values from 0 to 100
  ```

### Download
[series.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/series-x32.zip)<br>
[series.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/series-x64.zip)