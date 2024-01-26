The extension implements regexp function and operator.<br>
Author: [D. Richard Hipp](https://sqlite.org/src/file/ext/misc/regexp.c).



### Usage

* **regexp (pattern, str)**<br>
  Returns 1 if `str` is matched to `pattern` and returns 0 otherwise.<br>
  The following regular expression syntax is supported
     * `X*` - zero or more occurrences of X
     * `X+` - one or more occurrences of X
     * `X?` - zero or one occurrences of X
     * `X{p,q}` - between p and q occurrences of X
     * `(X)` - match X
     * `X|Y` - X or Y
     * `^X` - X occurring at the beginning of the string
     * `X$` - X occurring at the end of the string
     * `.` - Match any single character
     * `\c` - Character c where c is one of \{}()[]|*+?.
     * `\c` - C-language escapes for c in afnrtv.  ex: \t or \n
     * `\uXXXX` - Where XXXX is exactly 4 hex digits, unicode value XXXX
     * `\xXX` - Where XX is exactly 2 hex digits, unicode value XX
     * `[abc]` - Any single character from the set abc
     * `[^abc]` - Any single character not in the set abc
     * `[a-z]` - Any single character in the range a-z
     * `[^a-z]` - Any single character not in the range a-z
     * `\b` - Word boundary
     * `\w` - Word character.  [A-Za-z0-9_]
     * `\W` - Non-word character
     * `\d` - Digit
     * `\D` - Non-digit
     * `\s` - Whitespace character
     * `\S` - Non-whitespace character<br>
  
The following expressions are equivalent: `<url REGEXP '^google.*'>` and `<regexp('^google.*', url)>`.
  ```
  select regexp('^google.*', 'google.com'); --> 1
  ```

### Download
[regexp.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/regexp-x32.zip)<br>
[regexp.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/regexp-x64.zip)