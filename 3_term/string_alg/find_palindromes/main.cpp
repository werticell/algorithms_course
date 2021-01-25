#include <iostream>
#include <numeric>
#include <vector>

/*
 * By default counts the numbers of odd palindromes in the text if the second
 * parameter is not stated. If the second parameter is false then counts the
 * number of even palindromes in the text.
 */
uint64_t CountPalindromes(std::string_view text, bool odd = true) {
  std::vector<int32_t> palindromes_count(text.size());
  int32_t r = -1;
  int32_t l = 0;
  const uint8_t shift = odd ? 0 : 1;

  for (int32_t i = 0; i < text.size(); ++i) {
    int32_t k =
        (i > r) ? 0
                : std::min(r - i + 1, palindromes_count[l + (r - i) + shift]);

    while (i + k + 1 - shift < text.size() && i - k - 1 >= 0 &&
           text[i + k + 1 - shift] == text[i - k - 1]) {
      ++k;
    }

    palindromes_count[i] = k;
    if (i + k - 1 > r) {
      l = i - k + 1 - shift;
      r = i + k - 1;
    }
  }
  return std::accumulate(palindromes_count.begin(), palindromes_count.end(),
                         uint64_t());
}

uint64_t CountAllPalindromes(std::string_view text) {
  return CountPalindromes(text, true) + CountPalindromes(text, false);
}

int main() {
  std::string text;
  std::cin >> text;
  std::cout << CountAllPalindromes(text);
  return 0;
}
