#include "bin_manip.h"

write_le_int32::write_le_int32(const int32_t n) : _number(n) {}

read_le_int32::read_le_int32(int32_t &n) : _number(n) {}

write_bool::write_bool(const bool n) : _number(n) {}

read_bool::read_bool(bool &n) : _number(n) {}

write_c_str::write_c_str(char *s) : _string(s) {}

read_c_str::read_c_str(char *s, int l) : _string(s), _len(l) {}

std::basic_ostream<char> &operator<<(std::basic_ostream<char> &ofs,
                                     const write_le_int32 &element) {
  ofs.write((char *)(&element._number), 4);
  return ofs;
}

std::basic_istream<char> &operator>>(std::basic_istream<char> &ifs,
                                     read_le_int32 &element) {
  ifs.read((char *)(&element._number), 4);
  return ifs;
}

std::basic_ostream<char> &operator<<(std::basic_ostream<char> &ofs,
                                     const write_bool &element) {
  ofs.write((char *)(&element._number), 1);
  return ofs;
}

std::basic_istream<char> &operator>>(std::basic_istream<char> &ifs,
                                     read_bool &element) {
  ifs.read((char *)(&element._number), 1);
  return ifs;
}

std::basic_ostream<char> &operator<<(std::basic_ostream<char> &ofs,
                                     const write_c_str &element) {
  int _len = strlen(element._string) + 1;
  ofs.write(element._string, _len);
  return ofs;
}

std::basic_istream<char> &operator>>(std::basic_istream<char> &ifs,
                                     read_c_str &element) {
  char symbol;
  int len = 0;

  while (len < element._len - 1 && ifs.read(&symbol, 1)) {
    element._string[len++] = symbol;
    if (symbol == 0) {
      break;
    }
  }
  element._string[len] = '\0';
  return ifs;
}
