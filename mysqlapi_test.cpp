#include <iostream>
#include <string>
#include "mysqlapi/Field.h"

int main() {
  std::string str = "FUCK";
  toLowerString(str);
  std::cout << str << std::endl;

  Field field{"123", "test", Field::DB_TYPE_INTEGER};

  std::cout << field.getName() << std::endl;
  std::cout << field.getType() << std::endl;
  std::cout << field.getString() << std::endl;

  Field field_cp = field;
  std::cout << field_cp.getName() << std::endl;
  std::cout << field_cp.getType() << std::endl;
  std::cout << field_cp.getString() << std::endl;

}