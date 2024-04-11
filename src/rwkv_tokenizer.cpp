

#include "rwkv_tokenizer.h"

static const char *kTypeNames[] = {"Null",  "False",  "True",  "Object",
                                   "Array", "String", "Number"};

Trie::Trie(Trie *front, u_char ch) : front(front), ch(ch) {
  to = std::vector<Trie *>(256, nullptr);
};

/**
 * key=字符
 * idx=默认参数
 */
Trie *Trie::add(const std::string &key, add_val &val, size_t idx) {
  if (idx == key.size()) {
    // std::string value = val==defaultPair ? key : val;
    // 不传这个值会炸掉，因此不用判None(......)和BoPeng的Tokenizer有小区别
    add_val add_value = val;
    values[add_value.idx] = add_value.token_str;
    return this;
  }
  u_char ch = key[idx];
  if (to[ch] == nullptr) {
    to[ch] = new Trie(this, ch);
  }
  return to[ch]->add(key, val, idx + 1);
};

/**
 *
 */
Result Trie::find_longest(std::string &key, size_t idx) {
  Trie *u = this;
  u_char ch = key[idx];
  //   std::cout << "ch " << ch << std::endl;

  Result ret = {idx = idx, u, {}};
  while (u->to[ch] != nullptr) {
    u = u->to[ch];
    idx += 1;
    // std::cout << "!u->values.empty() " << !u->values.empty() << std::endl;

    if (!u->values.empty()) {
      ret = Result{idx, u, u->values};
    }
    //
    if (idx == key.size()) {
      break;
    }
    ch = key[idx];
  }
  return ret;
}

RWKV_Tokenizer::RWKV_Tokenizer(const std::string &tokenizer_path) {
  FILE *fp = fopen(tokenizer_path.c_str(), "r");
  char readBuffer[65536];
  rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
  rapidjson::Document d;
  d.ParseStream(is);
  fclose(fp);
  if (d.HasParseError()) {
    std::cout << "Error parsing JSON" << std::endl;
    return;
  }
  if (d.IsObject()) {
    // 遍历 JSON 对象的所有成员
    for (rapidjson::Value::ConstMemberIterator itr = d.MemberBegin();
         itr != d.MemberEnd(); ++itr) {
      std::vector<std::string> value;
      int key = std::stoi(itr->name.GetString());  // 获取键和值
      if (itr->value.IsString()) {
        value = {itr->value.GetString()};
        idx2token[key] = value;
      } else if (itr->value.IsArray()) {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        itr->value.Accept(writer);
        value.push_back(buffer.GetString());
        idx2token[key] = value;
      } else {
        printf("Type of member is %s\n", kTypeNames[itr->value.GetType()]);
        // auto type = itr->value.GetType();
      };
    }

    for (const auto &entry : idx2token) {
      uint32_t key = entry.first;
      const std::vector<std::string> &value = entry.second;
      // 将value中的数据组合成一个字符串
      std::string value_str;
      for (const auto &str : value) {
        value_str += str;
      }
      add_val add_v;
      add_v.idx = key;
      add_v.token_str = value_str;
      // 将value_str添加到root中
      //   std::cout << key << " " << value_str << std::endl;
      // printf("Type of member is %s\n",
      //            kTypeNames[value_str.GetType()]);
      root.add(value_str, add_v);
    }
  }
};

std::vector<uint32_t> RWKV_Tokenizer::encodeBytes(std::string &inputs) {
  size_t idx = 0;
  std::vector<uint32_t> tokens;
  while (idx < inputs.size()) {
    // size_t _idx = idx;
    Result res = root.find_longest(inputs, idx);

    // std::cout << "idx " << idx << std::endl;
    // std::cout << "res.idx " << res.idx << std::endl;
    assert(res.idx != idx);
    auto it = std::begin(res.values);
    uint32_t token = it->first;
    tokens.push_back(token);
    idx = res.idx;
  }
  return tokens;
};

std::string RWKV_Tokenizer::decodeBytes(std::vector<uint32_t> &tokens) {
  std::string result;
  for (auto token : tokens) {
    // 查找token对应的字符串向量
    auto it = idx2token.find(token);
    if (it != idx2token.end()) {
      // 迭代字符串向量，并将每个字符串附加到结果中
      for (const auto &str : it->second) {
        result += str;
      }
    } else {
      // 处理错误情况：token不存在于映射中
      throw std::runtime_error("Invalid token.");
    }
  }
  return result;
};

std::vector<std::vector<uint32_t>> RWKV_Tokenizer::encode(std::string &input) {
  std::vector<std::vector<uint32_t>> result;
  result.push_back(
      encodeBytes(input));  // 将编码后的结果作为唯一的元素添加到结果向量中
  return result;
}
// 处理字符串列表输入的 encode 函数
std::vector<std::vector<uint32_t>> RWKV_Tokenizer::encode(
    std::vector<std::string> &inputs) {
  std::vector<std::vector<uint32_t>> result;
  for (std::string &input : inputs) {
    result.push_back(
        encodeBytes(input));  // 对每个字符串进行编码并添加到结果向量中
  }
  return result;
}
// 解码函数
std::vector<std::string> RWKV_Tokenizer::decode(
    std::vector<std::vector<uint32_t>> &tokens) {
  std::vector<std::string> decodedStrings;
  for (auto &batch : tokens) {
    decodedStrings.push_back(
        decodeBytes(batch));  // 解码每一批tokens并添加到结果向量中
  }
  return decodedStrings;
}
// // 编码
// std::vector<std::vector<uint32_t>> encode(std::vector<std::string> &inputs);
// std::vector<std::vector<uint32_t>> encode(std::string &inputs);
// // 解码
// std::vector<std::string> decode(std::vector<std::vector<uint32_t>> &tokens);

// // 单次编码: 字符->token
// std::vector<uint32_t> encodeBytes(std::string &inputs);
// // 单次解码: token->字符
// std::string decodeBytes(std::vector<uint32_t> &tokens);
