// Copyright (c) 2024 Bulat Ibragimov, Irina Selyakh
//
// Данное программное обеспечение распространяется на условиях лицензии MIT.
// Подробности смотрите в файле LICENSE

#ifndef MY_POLISH_NOTATION_H
#define MY_POLISH_NOTATION_H

#include <string>
#include <string_view>
#include <stdexcept>

class UnknownSymbolError : public std::runtime_error { // если неизвестный символ
 public:
  explicit UnknownSymbolError(const std::string& symbol = "") : std::runtime_error("UnknownSymbolError: " + symbol) {
  }
};

class WrongExpressionError : public std::runtime_error { // если неверная структура выражения
 public:
  explicit WrongExpressionError(const std::string& msg = "") : std::runtime_error("WrongExpressionError: " + msg) {
  }
};

int CalculatePolishNotation(std::string_view input);

#endif  // MY_POLISH_NOTATION_H
