#include "employees.h"

Employee::Employee() { _name = new char[NAME_LEN]; }

Employee::~Employee() { delete[] _name; }

std::ostream &operator<<(std::ostream &os, const Employee &employee) {
  employee.write(os);
  return os;
}

std::istream &operator>>(std::istream &is, Employee &employee) {
  employee.read(is);
  return is;
}

std::ofstream &operator<<(std::ofstream &ofs, const Employee &employee) {
  employee.write_bin(ofs);
  return ofs;
}

std::ifstream &operator>>(std::ifstream &ifs, Employee &employee) {
  employee.read_bin(ifs);
  return ifs;
}

void Developer::write(std::ostream &os) const {
  os << "Developer"
     << "\n"
     << "Name: " << _name << "\n"
     << "Base Salary: " << _base_salary << "\n"
     << "Has bonus: ";
  if (_has_bonus) {
    os << "+";
  } else {
    os << "-";
  }
}

void Developer::read(std::istream &is) {
  is >> _name >> _base_salary >> _has_bonus;
}

void Developer::write_bin(std::ofstream &ofs) const {
  write_le_int32 dev_type(Developer_type);
  write_c_str name(_name);
  write_le_int32 salary(_base_salary);
  write_bool bonus(_has_bonus);

  ofs << dev_type << name << salary << bonus;
}

void Developer::read_bin(std::ifstream &ifs) {
  read_c_str name(_name, NAME_LEN);
  read_le_int32 salary(_base_salary);
  read_bool bonus(_has_bonus);

  ifs >> name >> salary >> bonus;
}

int Developer::salary() const {
  int salary = _base_salary;
  if (_has_bonus) {
    salary += 1000;
  }
  return salary;
}

int SalesManager::salary() const {
  return _base_salary + _sold_nm * _price * 0.01;
}

void SalesManager::write(std::ostream &os) const {
  os << "Sales Manager"
     << "\n"
     << "Name: " << _name << "\n"
     << "Base Salary: " << _base_salary << "\n"
     << "Sold items: " << _sold_nm << '\n'
     << "Item price: " << _price;
}

void SalesManager::read(std::istream &is) {
  is >> _name >> _base_salary >> _sold_nm >> _price;
}

void SalesManager::write_bin(std::ofstream &ofs) const {
  write_le_int32 type(SalesManager_type);
  write_c_str name(_name);
  write_le_int32 salary(_base_salary);
  write_le_int32 sold_nm(_sold_nm);
  write_le_int32 price(_price);

  ofs << type << name << salary << sold_nm << price;
}

void SalesManager::read_bin(std::ifstream &ifs) {
  read_c_str name(_name, NAME_LEN);
  read_le_int32 salary(_base_salary);
  read_le_int32 sold_nm(_sold_nm);
  read_le_int32 price(_price);

  ifs >> name >> salary >> sold_nm >> price;
}

EmployeesArray::EmployeesArray(int32_t capacity)
    : _size(0), _capacity(capacity) {
  _employees = new Employee *[_capacity];
}

EmployeesArray::~EmployeesArray() {
  for (int32_t i = 0; i < _size; ++i) {
    delete _employees[i];
  }
  delete[] _employees;
}

void EmployeesArray::add(Employee *employee) {
  if (_size == _capacity) {
    _capacity *= 2;
    Employee **employees_ = new Employee *[_capacity];
    memcpy(employees_, _employees, _size * sizeof(Employee *));
    delete[] _employees;
    _employees = employees_;
  }
  _employees[_size++] = employee;
}

int EmployeesArray::total_salary() const {
  int total_salary = 0;
  for (int32_t i = 0; i < _size; ++i) {
    total_salary += _employees[i]->salary();
  }
  return total_salary;
}

std::ostream &operator<<(std::ostream &os,
                         const EmployeesArray &employees_array) {
  for (int32_t i = 0; i < employees_array._size; ++i) {
    os << i + 1 << ". " << *employees_array._employees[i] << "\n";
  }
  os << "== Total salary: " << employees_array.total_salary() << "\n\n";
  return os;
}

std::istream &operator>>(std::istream &is, EmployeesArray &employees_array) {
  int type;
  Employee *employee = nullptr;
  is >> type;
  if (type == Developer_type) {
    employee = new Developer;
  } else if (type == SalesManager_type) {
    employee = new SalesManager;
  } else {
    std::cerr << "Only Developers or Sales Managers" << std::endl;
  }
  is >> *employee;
  employees_array.add(employee);
  return is;
}

std::ofstream &operator<<(std::ofstream &ofs,
                          const EmployeesArray &employees_array) {
  write_le_int32 size(employees_array._size);

  ofs << size;
  /*ofs << write_le_int32(employees_array._size);*/
  for (int32_t i = 0; i < employees_array._size; ++i) {
    ofs << *employees_array._employees[i];
  }
  return ofs;
}

std::ifstream &operator>>(std::ifstream &ifs, EmployeesArray &employees_array) {
  int32_t count;
  read_le_int32 count_(count);
  ifs >> count_;
  for (int32_t i = 0; i < count; ++i) {
    int32_t type;
    read_le_int32 type_(type);
    Employee *employee = nullptr;
    ifs >> type_;
    if (type == Developer_type) {
      employee = new Developer;
    } else if (type == SalesManager_type) {
      employee = new SalesManager;
    } else {
      std::cerr << "Only Developers or Sales Managers" << std::endl;
      break;
    }
    ifs >> *employee;
    employees_array.add(employee);
  }
  return ifs;
}
