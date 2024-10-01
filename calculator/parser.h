// Copyright (c) 2024 Bulat Ibragimov, Irina Selyakh
//
// Данное программное обеспечение распространяется на условиях лицензии MIT.
// Подробности смотрите в файле LICENSE

#ifndef MY_PARSER_H
#define MY_PARSER_H

#include "../tokenize/tokenize.h"
#include "../polish_notation/expressions.h"
#include <string>
#include <stdexcept>

class UnknownSymbolError : public std::runtime_error {  // если неизвестный символ
 public:
  explicit UnknownSymbolError(const std::string& symbol = "") : std::runtime_error("UnknownSymbolError: " + symbol) {
  }
};

class WrongExpressionError : public std::runtime_error {  // если неверная структура выражения
 public:
  explicit WrongExpressionError(const std::string& msg = "") : std::runtime_error("WrongExpressionError: " + msg) {
  }
};

std::unique_ptr<IExpression> ParseExpression(const std::vector<Token>& tokens, size_t& pos);

std::unique_ptr<IExpression> ParseTerm(const std::vector<Token>& tokens, size_t& pos);

std::unique_ptr<IExpression> ParseFactor(const std::vector<Token>& tokens, size_t& pos);

#endif  // MY_PARSER_H
