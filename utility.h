#include <string>
using namespace std;


/*
// 現在、テンプレートをうまくコンパイルする方法が不明
  template <typename T>
    char* get_class_name(const T &_class);
*/

string atos(char* c);
int stoi(string s);
string itos(int i);


#define TMP_FILE "/tmp/myCppLib_tmp.txt"
class command{
 public:
  static string pwd();
  static void mkdir(char* newdir);
  static void mkdir(string newdir);
};


#include <map>
class setting{
 protected:
  setting();
  bool read();
  string setting_filename;
  map<string,string> map_string;
  map<string,int>    map_int;

 public:
  /* if setting_filename exist, read setting   */
  setting(string setting_filename);
  setting(char* setting_filename);

  bool write();

  void set_string(const string key, const string value);
  string get_string(const string key, const string default_value="");

  void set_int(const string key, const int value);
  int get_int(const string key, const int default_value=0);
};
