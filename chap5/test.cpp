#include <iostream>
#include <openssl/sha.h>
#include <stdio.h>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
  // Start
  std::cout << "Consistent Hashing Sample" << std::endl;

  // Characters
  std::vector<char> chars;
  for (auto i = 'A'; i <= 'Z'; i++) {
    chars.push_back(i);
    std::cout << i << std::endl;
  }

  // Nodes
  // TODO:nodes数は外部から与えたい
  int nodes=4;
  std::vector<std::vector<char>> nodes;

  // 仮想ノードを導入することで，ノード間の間隔を一定にしレコードの分散を抑える

  return 0;
}