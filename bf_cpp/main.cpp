#include <iostream>
#include <string> // 文字列型
using namespace std;

void say_hello(string str) { // std::string
  cout << "Hello, " + str + "!" << endl;
}

int main() {
  say_hello("Brainfuck");
  return 0;
}
