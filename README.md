# RWKV_Tokenizer_CPP

This repository contains a C++ implementation of the RWKV Tokenizer, which is designed to tokenize text for use with the RWKV language model. The tokenizer uses a vocabulary file in JSON format and relies on the RapidJSON library for parsing.
This repository will prepare for the subsequent development of LLM-TPU-RWKV.

## **Notice !**
**This repository is in the development stage, please do not use it in the production environment!**

## Project Structure

The project is structured as follows:
```
rwkv_tokenizer
├─ .gitignore
├─ CMakeLists.txt
├─ README.md
├─ inc
│  ├─ rapidjson <https://github.com/Tencent/rapidjson/tree/master/include/rapidjson>
│  └─ rwkv_tokenizer.h
├─ src
│  └─ rwkv_tokenizer.cpp
├─ run.sh
├─ rwkv_vocab_v20230424.json
└─ test.cpp
```

## Prerequisites

Before you begin, ensure you have met the following requirements:
- You have a working C++ environment.
- You have installed CMake.
- You have git installed to clone the repository.


## Performance

```
hello world and 一些中文 to make sure the tokenizers are the same, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, @, #, $, %, ^, &, *, (, ), _, +, =, -, [, ], {, }, |, \\, ;, :, ', \", ,, <, >, ., /, ?, !, ~, `hello world and 一些中文 to make sure the tokenizers are the same, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, @, #, $, %, ^, &, *, (, ), _, +, =, -, [, ], {, }, |, \\, ;, :, ', \", ,, <, >, ., /, ?, !, ~, `hello world and 一些中文 to make sure the tokenizers are the same, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, @, #, $, %, ^, &, *, (, ), _, +, =, -, [, ], {, }, |, \\, ;, :, ', \", ,, <, >, ., /, ?, !, ~, `hello world and 一些中文 to make sure the tokenizers are the same, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, @, #, $, %, ^, &, *, (, ), _, +, =, -, [, ], {, }, |, \\, ;, :, ', \", ,, <, >, ., /, ?, !, ~, `
```
Decoding the same paragraph of text results in a performance improvement of three times compared to the Python TRIE version

Here are some test results (for reference only)

CPP(release build):
```
./test 
Total execution time: 0.0424805 seconds
```
PythonTRIE:
```
python .\test_python.py
cost=0.13954401016235352s
```
PythonNormal:
```
python .\test_python.py
cost=5.990019083023071s
```

If you need to test the optimal performance of CPP tokenizer, you need to modify `CMakeLists.txt` to switch the compilation mode to `release` mode


## Acknowledgments

This project makes use of the following open-source projects:

- [RapidJSON](https://github.com/Tencent/rapidjson)

This project refers to the following open-source projects:

- [ChatRWKV-TRIE-Tokenizer](https://github.com/TkskKurumi/ChatRWKV-TRIE-Tokenizer)

- [rwkv_cuda](https://github.com/npk48/rwkv_cuda)