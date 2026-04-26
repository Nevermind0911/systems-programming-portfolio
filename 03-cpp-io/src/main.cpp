#include "bin_manip.h"
#include "employees.h"

int main() {
  std::string string;
  EmployeesArray employees_array;

  while (true) {
    std::cin >> string;

    if (!string.compare("exit")) {
      break;
    } else if (!string.compare("load")) {
      std::string filename;
      std::cin >> filename;
      std::ifstream ifs(filename);
      ifs >> employees_array;
    } else if (!string.compare("save")) {
      std::string filename;
      std::cin >> filename;
      std::ofstream ofs(filename);
      ofs << employees_array;
    } else if (!string.compare("add")) {
      std::cin >> employees_array;
    } else if (!string.compare("list")) {
      std::cout << employees_array;
    }
  }
  return 0;
}
