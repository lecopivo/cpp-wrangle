#pragma once

#include <iostream>

class Screamer {
public:
  Screamer() {
    id = counter++;
    std::cout << "Construction " << id << "!" << std::endl;
  }

  ~Screamer() { std::cout << "Destruction " << id << "!" << std::endl; }

  Screamer(Screamer const &s) {
    id = counter++;
    std::cout << "Construction copy " << s.id << " to " << id << "!"
              << std::endl;
  }

  Screamer(Screamer &&s) {
    id = counter++;
    std::cout << "Construction move " << s.id << " to " << id << "!"
              << std::endl;
  }

  Screamer &operator=(Screamer const &s) {
    std::cout << "Assignment of " << s.id << " to " << id << "!" << std::endl;
    return *this;
  }

  Screamer &operator=(Screamer &&s) {
    std::cout << "Move assignment of " << s.id << " to " << id << "!"
              << std::endl;
    return *this;
  }

  static int counter;
  int        id = 0;
};

int Screamer::counter = 0;
