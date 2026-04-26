#ifndef LAB10_BINMANIP_H_INCLUDED
#define LAB10_BINMANIP_H_INCLUDED

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <istream>
#include <ostream>
#include <stdint.h>
#include <string.h>

class write_le_int32 {
public:
  explicit write_le_int32(const int32_t n);
  int32_t _number;
};

class read_le_int32 {
public:
  explicit read_le_int32(int32_t &n);
  int32_t &_number;
};

class write_bool {
public:
  explicit write_bool(const bool n);
  bool _number;
};

class read_bool {
public:
  explicit read_bool(bool &n);
  bool &_number;
};

class write_c_str {
public:
  explicit write_c_str(char *s);
  char *_string;
};

class read_c_str {
public:
  explicit read_c_str(char *s, int l);
  char *_string;
  int _len;
};

std::basic_ostream<char> &operator<<(std::basic_ostream<char> &os,
                                     const write_le_int32 &element);
std::basic_istream<char> &operator>>(std::basic_istream<char> &is,
                                     read_le_int32 &element);
std::basic_ostream<char> &operator<<(std::basic_ostream<char> &os,
                                     const write_bool &element);
std::basic_istream<char> &operator>>(std::basic_istream<char> &is,
                                     read_bool &element);
std::basic_ostream<char> &operator<<(std::basic_ostream<char> &os,
                                     const write_c_str &element);
std::basic_istream<char> &operator>>(std::basic_istream<char> &is,
                                     read_c_str &element);

#endif // LAB10_BINMANIP_H_INCLUDED
