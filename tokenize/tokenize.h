// Copyright (c) 2024 Bulat Ibragimov, Irina Selyakh
//
// Данное программное обеспечение распространяется на условиях лицензии MIT.
// Подробности смотрите в файле LICENSE

#ifndef MY_TOKENIZE_H
#define MY_TOKENIZE_H

#include <vector>
#include <string>
#include <string_view>
#include <variant>
#include <iostream>

struct PlusToken {};
struct MinusToken {};
struct MultiplyToken {};
struct DivideToken {};
struct ResidualToken {};
struct OpeningBracketToken {};
struct ClosingBracketToken {};
struct MinToken {};
struct MaxToken {};
struct AbsToken {};
struct SqrToken {};

struct NumberToken {
  int value;
};

struct UnknownToken {
  std::string value;
};

inline bool operator==(PlusToken, PlusToken) {
  return true;
}
inline bool operator==(MinusToken, MinusToken) {
  return true;
}
inline bool operator==(MultiplyToken, MultiplyToken) {
  return true;
}
inline bool operator==(DivideToken, DivideToken) {
  return true;
}
inline bool operator==(ResidualToken, ResidualToken) {
  return true;
}
inline bool operator==(OpeningBracketToken, OpeningBracketToken) {
  return true;
}
inline bool operator==(ClosingBracketToken, ClosingBracketToken) {
  return true;
}
inline bool operator==(MinToken, MinToken) {
  return true;
}
inline bool operator==(MaxToken, MaxToken) {
  return true;
}
inline bool operator==(AbsToken, AbsToken) {
  return true;
}
inline bool operator==(SqrToken, SqrToken) {
  return true;
}
inline bool operator==(const NumberToken& lhs, const NumberToken& rhs) {
  return lhs.value == rhs.value;
}
inline bool operator==(const UnknownToken& lhs, const UnknownToken& rhs) {
  return lhs.value == rhs.value;
}

// Определение типа Token с использованием std::variant
using Token = std::variant<PlusToken, MinusToken, MultiplyToken, DivideToken, ResidualToken, OpeningBracketToken,
                           ClosingBracketToken, MinToken, MaxToken, AbsToken, SqrToken, NumberToken, UnknownToken>;

void PrintToken(const Token& token);

std::vector<Token> Tokenize(std::string_view input);
#endif  // MY_TOKENIZE_H