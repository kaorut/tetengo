search_dict
===========

Shows the word entries corresponding to the specified key.

Synopsis
--------

```sh
search_dict lex.csv dict.bin
```

Description
-----------

Specify UniDic `lex.csv` and `dict.bin`.
`dict.bin` is generated by the make_dict program.

Launching search_dict, enter a keyword after the prompt.
Then it shows word entries in the form of the lines of `lex.csv`.

To exit the program, press Ctrl+D (or Ctrl+Z on Windows).

### About UniDic

UniDic is an electronic dictionary for Japanese natural language processings.
It is developed by National Institute for Japanese Language and Linguistics.

The dictionary data can be obtained in the following website:

https://unidic.ninjal.ac.jp/download#unidic_bccwj

Download the latest archive file unidic-cwj-x.y.z.zip from the site.

`lex.csv` that search_dict uses is found in the archive.

Return Value
------------

Returns 0 when the program exits successfully.

Returns a non-zero value when some error is happened.

---

Copyright (C) 2019-2020 kaoru  https://www.tetengo.org/
