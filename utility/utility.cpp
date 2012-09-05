#include <typeinfo>
#include <cxxabi.h>
#include "utility.h"
using namespace std;

namespace utility{
  template <typename T>
  char* get_class_name(T _class){
    int status;
    const type_info & id_p = typeid(_class);
    return abi::__cxa_demangle(id_p.name(), 0, 0, &status);
  }
}
