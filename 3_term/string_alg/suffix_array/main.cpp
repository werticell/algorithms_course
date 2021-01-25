#include <iostream>
#include <numeric>
#include <vector>

static const uint16_t alphabet_size = 256;

template <typename T>
void CountingSort(const T& source, std::vector<uint32_t>& dst) {
  std::vector<uint32_t> bins(alphabet_size);

  for (auto &element : source) {
    ++bins[element];
  }
  std::partial_sum(bins.begin(), bins.end(), bins.begin());
  for (int32_t i = 0; i < dst.size(); ++i) {
    dst[--bins[source[i]]] = i;
  }
}

void CountNewClasses(std::vector<int32_t>& temp_classes,
                     const std::vector<uint32_t>& suffix_array,
                     std::vector<uint32_t>& old_classes,
                     uint32_t shift) {

  temp_classes[suffix_array[0]] = 0;
  for (int32_t string_class = 0, i = 1; i < suffix_array.size(); ++i) {
    int32_t first_suffix_second_part =
        old_classes[(suffix_array[i - 1] + (1u << shift)) % suffix_array.size()];
    int32_t second_suffix_second_part =
        old_classes[(suffix_array[i] + (1u << shift)) % suffix_array.size()];

    if (old_classes[suffix_array[i]] != old_classes[suffix_array[i - 1]] ||
        first_suffix_second_part != second_suffix_second_part) {
      ++string_class;
    }
    temp_classes[suffix_array[i]] = string_class;
  }
  std::copy(temp_classes.begin(), temp_classes.end(), old_classes.begin());
}

std::vector<uint32_t> buildSuffixArray(std::string_view str) {
  std::vector<uint32_t> suffix_array(str.size());
  std::vector<uint32_t> bins(alphabet_size);
  std::vector<uint32_t> classes(str.size());

  CountingSort(str, suffix_array);

  classes[suffix_array[0]] = 0;
  for (int32_t string_class = 0, i = 1; i < suffix_array.size(); ++i) {
    if (str[suffix_array[i]] != str[suffix_array[i - 1]]) {
      ++string_class;
    }
    classes[suffix_array[i]] = string_class;
  }

  std::vector<int32_t> temp_suffix_array(str.size());
  std::vector<int32_t> temp_classes(str.size());
  for (uint32_t shift = 0; (1u << shift) < str.size(); ++shift) {
    bins.assign(bins.size(), 0);

    for (int32_t i = 0; i < suffix_array.size(); ++i) {
      temp_suffix_array[i] = suffix_array[i] - (1u << shift);
      if (temp_suffix_array[i] < 0) {
        temp_suffix_array[i] += str.size();
      }
    }

    for (int32_t suffix_number : temp_suffix_array) {
      ++bins[classes[suffix_number]];
    }

    std::partial_sum(bins.begin(), bins.end(), bins.begin());

    for (auto it = temp_suffix_array.crbegin(); it != temp_suffix_array.crend();
         ++it) {
      suffix_array[--bins[classes[*it]]] = *it;
    }


    CountNewClasses(temp_classes, suffix_array, classes, shift);
  }
  return suffix_array;
}

std::vector<int32_t> buildLCP(std::string_view str,
                              const std::vector<uint32_t> &suffix_array) {
  std::vector<int32_t> lcp_array(str.size());

  std::vector<uint32_t> suffix_pos_in_sufarray(str.size());
  for (int32_t i = 0; i < str.size(); ++i) {
    suffix_pos_in_sufarray[suffix_array[i]] = i;
  }

  for (int32_t k = 0, i = 0; i < str.size(); ++i) {
    if (k > 0) {
      --k;
    }
    if (suffix_pos_in_sufarray[i] != str.size() - 1) {
      int32_t next_suf_in_sufarray = suffix_array[suffix_pos_in_sufarray[i] + 1];
      while (std::max(i + k, next_suf_in_sufarray + k) < str.size() &&
             str[i + k] == str[next_suf_in_sufarray + k]) {
        ++k;
      }
      lcp_array[suffix_pos_in_sufarray[i]] = k;
    } else {
      lcp_array[str.size() - 1] = -1;
      k = 0;
      continue;
    }
  }
  return lcp_array;
}

int64_t CountNumberOfDifferentSubstrings(const std::string &str) {
  std::string temp_str(str + '#');
  std::vector<uint32_t> suffix_array = buildSuffixArray(temp_str);
  std::vector<int32_t> lcp_array = buildLCP(temp_str, suffix_array);

  int64_t result = temp_str.size() - suffix_array[0] - 1;
  for (int32_t i = 1; i < suffix_array.size(); ++i) {
    result += temp_str.size() - suffix_array[i] - lcp_array[i - 1] - 1;
  }
  return result;
}

int main() {
  std::string text;
  std::cin >> text;
  std::cout << CountNumberOfDifferentSubstrings(text);
  return 0;
}
