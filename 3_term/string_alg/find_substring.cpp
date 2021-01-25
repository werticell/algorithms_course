#include <iostream>
#include <vector>

std::vector<uint32_t> CalculatePrefixFunction(std::string_view str) {
  /*
   * for the index i stores the length of max prefix which is equal
   * to some suffix of the string str[0...i]
   */
  std::vector<uint32_t> prefix_function_values(str.size());

  for (uint32_t i = 1; i < str.size(); ++i) {
    uint32_t longest_prefix_length = prefix_function_values[i - 1];
    while (longest_prefix_length > 0 && str[longest_prefix_length] != str[i]) {
      longest_prefix_length = prefix_function_values[longest_prefix_length - 1];
    }
    if (str[longest_prefix_length] == str[i]) {
      prefix_function_values[i] = longest_prefix_length + 1;
    }
  }
  return prefix_function_values;
}

std::vector<uint32_t> FindSubstring(std::string_view template_string,
                                    std::string_view text) {
  std::vector<uint32_t> prefix_function_values =
      CalculatePrefixFunction(template_string);
  std::vector<uint32_t> result;

  uint32_t last_prefix_function_value = 0;
  for (uint32_t i = 0; i < text.size(); ++i) {
    while (last_prefix_function_value > 0 &&
           template_string[last_prefix_function_value] != text[i]) {
      last_prefix_function_value =
          prefix_function_values[last_prefix_function_value - 1];
    }
    if (template_string[last_prefix_function_value] == text[i]) {
      ++last_prefix_function_value;
      if (last_prefix_function_value == template_string.size()) {
        result.push_back(i - template_string.size() + 1);
      }
    }
  }
  return result;
}

int main() {
  std::string template_string;
  std::string text;
  std::cin >> template_string >> text;
  auto substring_positions = FindSubstring(template_string, text);
  for (auto& pos : substring_positions) {
    std::cout << pos << " ";
  }
  return 0;
}
