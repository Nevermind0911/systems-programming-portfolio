#ifndef LAB10_EMPLOYEES_H_INCLUDED
#define LAB10_EMPLOYEES_H_INCLUDED

#include "bin_manip.h"

const int32_t NAME_LEN = 102;

enum Type {
  Developer_type = 1,
  SalesManager_type = 2,
};

class Employee {
public:
  Employee();
  virtual ~Employee();
  virtual int salary() const = 0;
  friend std::ostream &operator<<(std::ostream &os, const Employee &employee);
  friend std::istream &operator>>(std::istream &is, Employee &employee);
  friend std::ofstream &operator<<(std::ofstream &ofs,
                                   const Employee &employee);
  friend std::ifstream &operator>>(std::ifstream &ifs, Employee &employee);

  virtual void write(std::ostream &os) const {}
  virtual void read(std::istream &is) {}
  virtual void write_bin(std::ofstream &ofs) const {}
  virtual void read_bin(std::ifstream &ifs) {}

protected:
  char *_name;
  int32_t _base_salary;
};

class Developer : public Employee {
public:
  Developer() = default;
  ~Developer() = default;
  int salary() const override;

protected:
  void write(std::ostream &os) const override;
  void read(std::istream &is) override;
  void write_bin(std::ofstream &ofs) const override;
  void read_bin(std::ifstream &ifs) override;

private:
  bool _has_bonus;
};

class SalesManager : public Employee {
public:
  SalesManager() = default;
  ~SalesManager() = default;
  int salary() const override;

protected:
  void write(std::ostream &os) const override; 
  void read(std::istream &is) override;
  void write_bin(std::ofstream &ofs) const override;
  void read_bin(std::ifstream &ifs) override;

private:
  int32_t _sold_nm, _price;
};

class EmployeesArray {
public:
  EmployeesArray(int32_t _capacity = 5);
  ~EmployeesArray();
  void add(Employee *employee);
  int total_salary() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const EmployeesArray &employee_array);
  friend std::istream &operator>>(std::istream &is,
                                  EmployeesArray &employee_array);
  friend std::ofstream &operator<<(std::ofstream &ofs,
                                   const EmployeesArray &employee_array);
  friend std::ifstream &operator>>(std::ifstream &ifs,
                                   EmployeesArray &employee_array);

private:
  Employee **_employees;
  int32_t _size, _capacity;
};

#endif
