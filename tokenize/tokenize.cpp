// Copyright (c) 2024 Bulat Ibragimov, Irina Selyakh
//
// Данное программное обеспечение распространяется на условиях лицензии MIT.
// Подробности смотрите в файле LICENSE

#include "../tokenize/tokenize.h"
#include <cctype>
#include <unordered_map>
#include <stdexcept>

// Храним ключ - значение
static const std::unordered_map<char, Token> kSymbolToToken{
    {'+', PlusToken{}},     {'-', MinusToken{}},          {'/', DivideToken{}},        {'%', ResidualToken{}},
    {'*', MultiplyToken{}}, {'(', OpeningBracketToken{}}, {')', ClosingBracketToken{}}};

static const std::unordered_map<std::string_view, Token> kWordToToken{
    {"min", MinToken()}, {"max", MaxToken()}, {"abs", AbsToken()}, {"sqr", SqrToken()}};

// Для корректрой работы isspace на некоторых системах
// проверяет, является ли символ пробельным
static bool IsSpace(char symbol) {
  return std::isspace(static_cast<unsigned char>(symbol));
}

// Проверка, является ли символ буквой
static bool IsLetter(char symbol) {
  return ('a' <= symbol && symbol <= 'z') || ('A' <= symbol && symbol <= 'Z');
}

// Проверка, является ли символ цифрой
static bool IsDigit(char symbol) {
  return '0' <= symbol && symbol <= '9';
}

// Парсинг числа
static NumberToken ParseNumber(std::string_view input, size_t& pos) {
  int value = 0;
  while (IsDigit(input[pos])) {
    value = value * 10 + (input[pos] - '0');
    ++pos;
  }
  return {value};
}

// Парсинг букв
static Token ParseWord(std::string_view input, size_t& pos) {
  size_t word_size = 0;
  while (IsLetter(input[pos])) {
    ++word_size;
    ++pos;
  }
  const auto word = input.substr(pos - word_size, word_size);
  if (auto it = kWordToToken.find(word); it != kWordToToken.end()) { // поиск токена
    return it->second;
  }
  return UnknownToken{std::string(word)};
}

std::vector<Token> Tokenize(std::string_view input) {
  std::vector<Token> tokens;
  size_t pos = 0;
  while (pos < input.size()) {
    const auto symbol = input[pos];
    if (IsSpace(symbol)) {
      ++pos;
      continue;
    }
    if (auto it = kSymbolToToken.find(symbol); it != kSymbolToToken.end()) {
      tokens.emplace_back(it->second);
      ++pos;
    } else if (IsDigit(symbol)) {
      tokens.emplace_back(ParseNumber(input, pos));
    } else if (IsLetter(symbol)) {
      tokens.emplace_back(ParseWord(input, pos));
    } else {
      throw std::runtime_error("Unknown symbol");
    }
  }
  return tokens;
}

void PrintToken(const Token& token) {
  std::visit(
      [](const auto& tok) {
        using Type = std::decay_t<decltype(tok)>; // Получаем тип токена
        if constexpr (std::is_same_v<Type, PlusToken>) { // значения вычисляются на этапе компиляции, позволяет повысить производительность
          std::cout << "+\n";
        } else if constexpr (std::is_same_v<Type, MinusToken>) {
          std::cout << "-\n";
        } else if constexpr (std::is_same_v<Type, MultiplyToken>) {
          std::cout << "*\n";
        } else if constexpr (std::is_same_v<Type, DivideToken>) {
          std::cout << "/\n";
        } else if constexpr (std::is_same_v<Type, ResidualToken>) {
          std::cout << "%\n";
        } else if constexpr (std::is_same_v<Type, NumberToken>) {
          std::cout << tok.value << "\n";
        } else if constexpr (std::is_same_v<Type, OpeningBracketToken>) {
          std::cout << "(\n";
        } else if constexpr (std::is_same_v<Type, ClosingBracketToken>) {
          std::cout << ")\n";
        }
      },
      token);
}
