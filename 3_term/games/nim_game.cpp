#include <iostream>
#include <vector>


uint32_t FindMex(const std::vector<uint32_t>& nim_numbers) {
  std::vector<bool> used(nim_numbers.size(), false);
  for (uint32_t number : nim_numbers) {
    if (number < used.size()) {
      used[number] = true;
    }
  }

  for (uint32_t i = 0; i < used.size(); ++i) {
    if (!used[i]) {
      return i;
    }
  }
  return used.size();
}


void FindWinner(int32_t people_count) {
  std::vector<uint32_t> nim_numbers(people_count + 1, false);
  nim_numbers[0] = 0; nim_numbers[1] = 0;
  nim_numbers[2] = 2;

  for (int32_t i = 2; i <= people_count; ++i) {

    std::vector<uint32_t> tmp(i);
    // checking 2 set partitions, in terms of tasks we choose the human to be killed
    for (int32_t j = 1; j <= i; ++j) {
      tmp[j - 1] = nim_numbers[j - 1] ^ nim_numbers[i - j];
    }
    nim_numbers[i] = FindMex(tmp);
  }

  if (nim_numbers[people_count] != 0) {
    std::cout << "Schtirlitz" << '\n';
    for (int32_t i = 1; i <= people_count; ++i) {
      if ((nim_numbers[i - 1] ^ nim_numbers[people_count - i]) == 0) {
        std::cout << i << "\n";
      }
    }
  } else {
    std::cout << "Mueller";
  }

}

int main() {
  int32_t n = 0;
  std::cin >> n;
  FindWinner(n);
  return 0;
}
