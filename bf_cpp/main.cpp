#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <cstdio>
#include <stack>

using namespace std;

constexpr int MEMORY_SIZE = 1024;

// 命令をコンパイル時定数として定義
constexpr char INCREMENT = '+';
constexpr char DECREMENT = '-';
constexpr char RIGHT = '>';
constexpr char LEFT = '<';
constexpr char LOOP_START = '[';
constexpr char LOOP_END = ']';
constexpr char OUTPUT = '.';
constexpr char INPUT = ',';

int main(int argc, char* argv[]) {

  /* 引数チェック */

  if (argc < 2) {
    cerr << "Error: A Brainfuck code is not passed as a command-line argument." << endl;
    cerr << "Please pass an argument as the form, $ brainfuck [FILENAME]." << endl;
    return -1;
  }

  /* ファイル読み込み */

  ifstream file(argv[1]);
  if (!file) {
    // ファイル読み込み失敗
    cerr << "Error: The file, " << argv[1] << ", cannot be opened." << endl;
    return -1;
  }

  unsigned char memory[MEMORY_SIZE];    // 1Byte=8Bit (0-255) で構成されるメモリを定義
  unsigned int ptr = 0;                 // メモリポインタ (負の値は取らないので unsigned)
  unsigned int code_ptr = 0;            // Brainfuck コードのポインタ
  unsigned int code_len = 0;            // Brainfuck コードの終端 (長さ)

  stack<int> loops;                     // ループのネスト用スタック: "[" のコードポインタが格納される

  // メモリの初期化
  memset(memory, 0, sizeof(memory));

  /* ファイルの中身を取得 */

  stringstream buffer;          // stringstream 用のバッファ
  buffer << file.rdbuf();       // ファイルをバッファとして取得
  string code(buffer.str());    // code を string として取得
  code_len = code.size();       // コードサイズ

  /* Brainfuck 解析 */

  while (code_ptr < code_len) {
    switch (code[code_ptr]) {
      case INCREMENT:
        memory[ptr]++;
        break;
      case DECREMENT:
        memory[ptr]--;
        break;
      case RIGHT:
        ptr = (ptr >= MEMORY_SIZE-1) ? 0 : ptr+1;
        break;
      case LEFT:
        ptr = (ptr <= 0) ? MEMORY_SIZE-1 : ptr-1;
        break;
      case LOOP_START:
        loops.push(code_ptr);

        // ポインタの値が 0 ならば "]" に飛ぶ
        if (memory[ptr] == 0) {
          int depth = 1;  // ループの深さ

          while (depth > 0) {
            code_ptr++; // コードポインタを進める

            // もしコード終端に来てしまったらエラー
            if (code_ptr >= code_len) {
              cerr << "Error: Cannot find \"]\"." << endl;
              return -1;
            }

            // "[" と "]" を見つけたら深さを修正
            if (code[code_ptr] == LOOP_START)
              depth++;
            if (code[code_ptr] == LOOP_END)
              depth--;
          } // もし depth=0 であれば、対応する "]" が見つかったので抜ける

          loops.pop();
        }
        break;
      case LOOP_END:
        if (loops.empty()) {
          cerr << "Error: Loop start order, " << LOOP_START << ", is nod found." << endl;
          return -1;
        }
        code_ptr = loops.top() - 1;   // 対応する "[" に移動
        loops.pop();                  // 飛んだらまたスタックされるのでポップしておく
        break;
      case OUTPUT:
        putchar(memory[ptr]);
        break;
      case INPUT:
        memory[ptr] = getchar();
        break;
      default:
        // 上記以外はコメント扱い
        break;
    }
    code_ptr++;
  }

  return 0;
}
