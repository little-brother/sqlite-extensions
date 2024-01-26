The extension implements functions for managing XML content stored in an SQLite database as text. <a href="https://en.wikipedia.org/wiki/XPath#Syntax_and_semantics_(XPath_1.0)">XPath 1.0</a> is used to obtain a data.

### Usage

* **xml_valid(xml)**<br>
  Return 1 if the argument is well-formed XML and 0, otherwise.<br>
  ```
  select xml_valid('<a>A</a>'); --> 1
  select xml_valid('<a>A/a>'); --> 0
  ```

* **xml_extract(xml, xpath, delim = "")**<br>
  Extract a node content or an attribute value.<br>
  ```
  select xml_extract('<a>A</a>', 'a'); --> <a>A</a>
  select xml_extract('<a>A</a>', 'a/text()'); --> A
  select xml_extract('<a>A</a><a>B</a>', 'a/text()', ','); --> A,B
  select xml_extract('<a id = "1">A</a>', 'a/@id'); --> 1
  ```

* **xml_append(xml, xpath, insertion, pos = after)**<br>
  Append a node or an attribute based on the pos (one of `first`, `before`, `after`, `last`, `child` or `child first`/`child last`).<br>
  The `child` is ignored for the attribute. The insertion should be valid (there is no check).<br>
  ```
  select xml_append('<a>A</a><a>B</a><a>C</a>', 'a[2]', '<b>D</b>', 'after') xml; 
  --> <a>A</a><a>B</a><b>D</b><a>C</a>
 
  select xml_append('<a>A</a><a>B</a><a>C</a>', 'a[2]', '<b>D</b>', 'child') xml; 
  --> <a>A</a><a>B<b>D</b></a><a>C</a>
 
  select xml_append('<a>A</a><a id="1">B</a><a id="2">C</a>', 'a/@id', 'x="2"', 'first') xml; 
  --> <a>A</a><a x="2" id="1">B</a><a x="2" id="2">C</a>
  ```

* **xml_update(xml, xpath, replacement)**<br>
  Update nodes or attributes. The replacement should be valid (there is no check). If the replacement is `NULL` then the call equals to **xml_remove(xml, path)**.<br>
  ```
  select xml_update('<a>A</a><a id="1">B</a><a id="2">C</a>', 'a[2]', '<b>D</b>');
  --> <a>A</a><b>D</b><a id="2">C</a>

  select xml_update('<a>A</a><a id="1">B</a><a id="2">C</a>', 'a/@id', '3');
  --> <a>A</a><a id="3">B</a><a id="3">C</a>
  ```

* **xml_remove(xml, xpath)**<br>
  Remove nodes or attributes.<br>
  ```
  select xml_remove('<a>A</a><a id="1">B</a><a id="2">C</a>', 'a[2]');
  --> <a>A</a><a id="2">C</a>

  select xml_remove('<a>A</a><a id="1">B</a><a id="2">C</a>', 'a/@id');
  --> <a>A</a><a>B</a><a>C</a>
  ``` 
		
* **xml_each(xml, xpath)**<br>
  Return node values as a table.<br>
  ```
  select * from xml_each('<a>A</a><a>B</a><a>C</a>', 'a/text()'); --> Three rows: A, B and C
  ```

### Download
[xml.dll 32-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/xml-x32.zip)<br>
[xml.dll 64-bit](https://github.com/little-brother/sqlite-extensions/releases/latest/download/xml-x64.zip)

### How to build by mingw64
```
set PATH=c:\mingw64\mingw64\bin\;%PATH%
g++ -I ./include -shared ./src/xml.cpp ./include/pugixml.cpp -o xml.dll -s -static -DPUGIXML_NO_STL -Os
```