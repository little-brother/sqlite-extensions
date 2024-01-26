Set of string functions, from `slice`, `contains` and `count` to `split_part`, `trim` and `repeat`.<br>
Author: [Anton Zhiyanov (nalgeon)](https://github.com/nalgeon/sqlean/blob/main/docs/text.md).

### Usage
* **text_substring(str, start [,length])**<br>
  Extract a substring of length characters starting at the start position (1-based). By default, extracts all characters from start to the end of the string.
  ```
  select text_substring('hello world', 7); --> world
  select text_substring('hello world', 7, 3); --> wor
  ``` 

* **text_slice(str, start [,end])**<br>
  Extract a substring from the start position inclusive to the end position non-inclusive (1-based). By default, `end` is the end of the string. Both `start` and `end` can be negative, in which case they are counted from the end of the string toward the beginning of the string.
  ```
  select text_slice('hello world', 7); --> world
  select text_slice('hello world', 7, 12); --> world
  select text_slice('hello world', -5); --> world
  select text_slice('hello world', -5, -2); --> wor
  ```
 
* **text_left(str, length)**<br>
  Extract a substring of length characters from the beginning of the string. For negative length, extracts all but the last `length`-characters.
  ```  
  select text_left('hello world', 5); --> hello
  select text_left('hello world', -6); --> hello
  ``` 

* **text_right(str, length)**<br>
  Extract a substring of length characters from the end of the string. For negative length, extracts all but the first `length`-characters.
  ```  
  select text_right('hello world', 5); --> world
  select text_right('hello world', -6); --> world
  ``` 

* **text_index(str, other)**<br>
  Return the first index of the other substring in the original string.
  ```
  select text_index('hello yellow', 'ello'); --> 2
  select text_index('hello yellow', 'x'); --> 0
  ``` 

* **text_last_index(str, other)**<br>
  Return the last index of the other substring in the original string.
  ```
  select text_last_index('hello yellow', 'ello'); --> 8
  select text_last_index('hello yellow', 'x'); --> 0
  ```

* **text_contains(str, other)**<br>
  Check if the string contains the other substring.
  ```  
  select text_contains('hello yellow', 'ello'); --> 1
  select text_contains('hello yellow', 'x'); --> 0
  ``` 

* **text_has_prefix(str, other)**<br>
  Check if the string starts with the `other` substring.
  ```    
  select text_has_suffix('hello yellow', 'hello'); --> 0
  select text_has_suffix('hello yellow', 'yellow'); --> 1
  ```

* **text_has_suffix(str, other)**<br>
  Check if the string ends with the `other` substring.
  ```  
  select text_has_suffix('hello yellow', 'hello'); --> 0
  select text_has_suffix('hello yellow', 'yellow'); --> 1
  ``` 

* **text_count(str, other)**<br>
  Count how many times the `other` substring is contained in the original string.
  ```  
  select text_count('hello yellow', 'ello'); --> 2
  select text_count('hello yellow', 'x') = 0; --> 0
  ``` 

* **text_split(str, sep, n)**<br>
  Split a string by a separator and returns the n-th part (counting from one). When `n` is negative, return the `n-th`-from-last part.
  ```  
  select text_split('one|two|three', '|', 2); --> two
  select text_split('one|two|three', '|', -1); --> three
  select text_split('one|two|three', ';', 2); --> (empty string)
  ``` 

* **text_concat(str, ...)**<br>
  Concatenate strings and returns the resulting string. Null values are ignored.
  ```  
  select text_concat('one', 'two', 'three'); --> onetwothree
  select text_concat('one', null, 'three'); --> onethree
  ``` 

* **text_join(sep, str, ...)**<br>
  Join strings using the separator. Null values are ignored.
  ```  
  select text_join('|', 'one', 'two'); --> one|two
  select text_join('|', 'one', null, 'three'); --> one|three
  ``` 

* **text_repeat(str, count)**<br>
  Concatenate the string to itself a given number of times and returns the resulting string.
  ```  
  select text_repeat('one', 3); --> oneoneone
  ``` 

* **text_ltrim(str [,chars])**<br>
  Trim certain characters (spaces by default) from the beginning of the string.
  ```  
  select text_ltrim('  hello'); --> hello
  select text_ltrim('273hello', '123456789'); --> hello
  ``` 

* **text_rtrim(str [,chars])**<br>
  Trim certain characters (spaces by default) from the end of the string.
  ```  
  select text_rtrim('hello  '); --> hello
  select text_rtrim('hello273', '123456789'); --> hello
  ``` 

* **text_trim(str [,chars])**<br>
  Trim certain characters (spaces by default) from the beginning and end of the string.
  ```  
  select text_trim('  hello  '); --> hello 
  select text_trim('273hello273', '123456789'); --> hello
  ``` 

* **text_lpad(str, length [,fill])**<br>
  Pad the string to the specified length by prepending certain characters (spaces by default).
  ```
  select text_lpad('hello', 7, '*'); --> **hello
  ``` 

* **text_rpad(str, length [,fill])**<br>
  Pad the string to the specified length by appending certain characters (spaces by default).
  ```  
  select text_rpad('hello', 7, '*'); --> hello**
  ``` 

* **text_replace(str, old, new [,count])**<br>
  Replace `old` substrings with `new` substrings in the original string, but not more than count times. By default, replaces all `old` substrings.
  ```  
  select text_replace('hello', 'l', '*'); --> he**o
  select text_replace('hello', 'l', '*', 1); --> he*lo
  ``` 

* **text_translate(str, from, to)**<br>
  Replace each string character that matches a character in the `from` set with the corresponding character in the to set. If `from` is longer than `to`, occurrences of the extra characters in `from` are deleted.
  ```  
  select text_translate('hello', 'ol', '01'); --> he110
  select text_translate('hello', 'ol', '0'); --> he0
  ``` 

* **text_reverse(str)**<br>
  Reverse the order of the characters in the string.
  ```  
  select text_reverse('hello'); --> olleh
  ``` 

* **text_length(str)**<br>
  Return the number of characters in the string.
  ```  
  select text_length('𐱨㦲'); --> 6
  ``` 

* **text_size(str)**<br>
  Return the number of bytes in the string.
  ```  
  select text_size('𐱨㦲'); --> 12
  ``` 

* **text_bitsize(str)**<br>
  Return the number of bits in the string.
  ```  
  select text_bitsize('one'); --> 24
  ``` 

### Download
[text.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/text-x32.zip)<br>
[text.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/text-x64.zip)