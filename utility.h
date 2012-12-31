#include <string>
using namespace std;


/*
// 現在、テンプレートをうまくコンパイルする方法が不明
  template <typename T>
    char* get_class_name(const T &_class);
*/

string atos(char* c);

#define TMP_FILE "/tmp/myCppLib_tmp.txt"
class command{
 public:
  static string pwd();
  /*
  static void cd(char* c);
  static void cd(string s);
  */
};
