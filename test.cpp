#include <chrono>

#include "rwkv_tokenizer.h"
#include "stdio.h"

int main() {
  RWKV_Tokenizer rwkv_tokenizer("rwkv_vocab_v20230424.json");
  std::string test =
      "hello world and 一些中文 to make sure the tokenizers are the same, 1, "
      "2, 3, 4, 5, 6, 7, 8, 9, 10, @, #, $, %, ^, &, *, (, ), _, +, =, -, [, "
      "], {, }, |, \\, ;, :, ', \", ,, <, >, ., /, ?, !, ~, `hello world and "
      "一些中文 to make sure the tokenizers are the same, 1, 2, 3, 4, 5, 6, 7, "
      "8, 9, 10, @, #, $, %, ^, &, *, (, ), _, +, =, -, [, ], {, }, |, \\, ;, "
      ":, ', \", ,, <, >, ., /, ?, !, ~, `hello world and 一些中文 to make "
      "sure the tokenizers are the same, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, @, #, "
      "$, %, ^, &, *, (, ), _, +, =, -, [, ], {, }, |, \\, ;, :, ', \", ,, <, "
      ">, ., /, ?, !, ~, `hello world and 一些中文 to make sure the tokenizers "
      "are the same, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, @, #, $, %, ^, &, *, (, ), "
      "_, +, =, -, [, ], {, }, |, \\, ;, :, ', \", ,, <, >, ., /, ?, !, ~, `";
  std::vector<std::vector<uint32_t>> res;

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < 1000; ++i) {
    res = rwkv_tokenizer.encode(test);
  }

  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<double> elapsed_time = end - start;
  std::cout << "Total execution time: " << elapsed_time.count() << " seconds"
            << std::endl;

  // 遍历外部向量
  for (const auto& inner_vec : res) {
    // 遍历内部向量
    for (uint32_t val : inner_vec) {
      std::cout << val << " ";
    }
    std::cout << std::endl;
  }
}
