Fuzzy-matching helpers.<br>
Author: [Anton Zhiyanov (nalgeon)](https://github.com/nalgeon/sqlean/blob/main/docs/fuzzy.md).



-   Measure distance between two strings.
-   Compute phonetic string code.
-   Transliterate a string.

All functions except `translit` support only ASCII strings.

### Usage
* **dlevenshtein(x, y)**<br>
  Calculate Damerau-Levenshtein distance.
  ```
  select dlevenshtein('awesome', 'aewsme'); --> 2 
  ``` 

* **levenshtein(x, y)**<br>
  Calculate Levenshtein distance.
  ```
  select levenshtein('awesome', 'aewsme'); --> 3 
  ``` 

* **hamming(x, y)**<br>
  Calculate Hamming distance.
  ```
  select hamming('awesome', 'aewsome'); --> 2
  ``` 

* **jaro_winkler(x, y)**<br>
  Calculate Jaro-Winkler distance.
  ```
  select jaro_winkler('awesome', 'aewsme'); --> 0.907 
  ``` 

* **osa_distance(x, y)**<br>
  Calculate Optimal String Alignment distance.
  ```
  select osa_distance('awesome', 'aewsme'); --> 3
  ``` 

* **edit_distance(x, y)**<br>
  Calculate spellcheck edit distance.
  ```
  select edit_distance('awesome', 'aewsme'); --> 215
  ``` 

* **caverphone(x)**<br>
  Calculate caverphone code.
  ```
  select caverphone('awesome'); --> AWSM111111
  ```

* **phonetic_hash(x)**<br>
  Calculate spellcheck phonetic code.
  ```
  select phonetic_hash('awesome'); --> ABACAMA
  ```

* **soundex(x)**<br>
  Return soundex code.
  ```
  select soundex('awesome'); --> A250
  ```

* **rsoundex(x)**<br>
  Return refined Soundex code.
  ```
  select rsoundex('awesome'); --> A03080
  ```

* **translit(x)**<br>
  Convert the input string from UTF-8 into pure ASCII by converting all non-ASCII characters to some combination of characters
in the ASCII subset.
  ```
  select translit('𐱨㦲'); --> privet
  ```

### Download
[fuzzy.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/fuzzy-x32.zip)<br>
[fuzzy.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/fuzzy-x64.zip)