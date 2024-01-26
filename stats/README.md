Common statistical functions.<br>
Authors: Liam Healy, [Anton Zhiyanov (nalgeon)](https://github.com/nalgeon/sqlean/blob/main/docs/stats.md).

### Usage
* **median(x)**<br>
  Aggregate function to calculate median (50-th percentile). 

* **percentile_N(data)**<br>
  Aggregate function to calculate N-th percentile. N is one of: 25, 75, 90, 95, 99.
 
* **percentile(x, perc)**<br>
  Aggregate function to calculate perc-th percentile..

* **stddev(x)/stddev_samp(x)**<br>
  Aggregate function to sample standard deviation. 
 
* **stddev_pop(x)**<br>
  Aggregate function to population standard deviation. 

* **variance(x)/var_samp(x)**<br>
  Aggregate function to sample variance. 

* **var_pop(x)**<br>
  Aggregate function to population variance. 

* **stddev(x)/stddev_samp(x)**<br>
  Aggregate function to sample standard deviation. 

* **generate_series(start[, stop[, step]])**<br>
  The table-valued function to generate a sequence of integer values starting with start, ending with stop (inclusive) with an optional step.
  ```
  select * from generate_series(5, 100, 5);
  ```  

### Download
[stats.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/stats-x32.zip)<br>
[stats.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/stats-x64.zip)