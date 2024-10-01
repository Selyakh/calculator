// Copyright (c) 2024 Bulat Ibragimov, Irina Selyakh
//
// Данное программное обеспечение распространяется на условиях лицензии MIT.
// Подробности смотрите в файле LICENSE

#include "../tokenize/tokenize.h"
#include "expressions.h"
#include "../polish_notation/polish_notation.h"

std::unique_ptr<IExpression> Parse(const std::vector<Token>& tokens, size_t& pos) {  // парс выр-ия польской нотации
  if (pos >= tokens.size()) {  // проверка на недостаток аргументов
    throw WrongExpressionError("too few arguments");
  }
  const auto& token = tokens[pos++];  // получаем текущий токен и увеличиваем позицию.

  if (std::holds_alternative<UnknownToken>(token)) {  // если текущий токен — неизвестный, выбрасываем исключение
    throw UnknownSymbolError("Unknown token: " + std::get<UnknownToken>(token).value);
  }

  if (std::holds_alternative<ClosingBracketToken>(token)) {  // обработка закрывающей скобки
    throw WrongExpressionError("No matching (");
  }

  if (std::holds_alternative<OpeningBracketToken>(token)) {  // обработка открывающейся скобки
    auto sub_expression = Parse(tokens, pos);
    if (pos >= tokens.size() || !std::holds_alternative<ClosingBracketToken>(tokens[pos++])) {
      throw WrongExpressionError("No matching )");
    }
    return sub_expression;
  }

  if (std::holds_alternative<NumberToken>(token)) {
    return std::make_unique<Constant>(std::get<NumberToken>(token).value);
  }

  auto first_arg = Parse(tokens, pos);  // обработка унарных операций
  if (std::holds_alternative<SqrToken>(token)) {
    return std::make_unique<Square>(std::move(first_arg));
  }
  if (std::holds_alternative<AbsToken>(token)) {
    return std::make_unique<AbsoluteValue>(std::move(first_arg));
  }
  if (pos >= tokens.size() || std::holds_alternative<ClosingBracketToken>(tokens[pos])) {
    if (std::holds_alternative<PlusToken>(token)) {
      return std::make_unique<Plus>(std::move(first_arg));
    }
    if (std::holds_alternative<MinusToken>(token)) {
      return std::make_unique<Minus>(std::move(first_arg));
    }
  }

  auto second_arg = Parse(tokens, pos);  // обработка бинарных операций
  if (std::holds_alternative<PlusToken>(token)) {
    return std::make_unique<Sum>(std::move(first_arg), std::move(second_arg));
  }
  if (std::holds_alternative<MinusToken>(token)) {
    return std::make_unique<Subtract>(std::move(first_arg), std::move(second_arg));
  }
  if (std::holds_alternative<MultiplyToken>(token)) {
    return std::make_unique<Multiply>(std::move(first_arg), std::move(second_arg));
  }
  if (std::holds_alternative<DivideToken>(token)) {
    return std::make_unique<Divide>(std::move(first_arg), std::move(second_arg));
  }
  if (std::holds_alternative<ResidualToken>(token)) {
    return std::make_unique<Residual>(std::move(first_arg), std::move(second_arg));
  }
  if (std::holds_alternative<MinToken>(token)) {
    return std::make_unique<Minimum>(std::move(first_arg), std::move(second_arg));
  }
  if (std::holds_alternative<MaxToken>(token)) {
    return std::make_unique<Maximum>(std::move(first_arg), std::move(second_arg));
  }
  throw WrongExpressionError("Invalid token");
}

int CalculatePolishNotation(std::string_view input) {
  const auto tokens = Tokenize(input);  // токенизируем, возвращая вектор токенов
  size_t pos = 0;
  const auto expression = Parse(tokens, pos);
  if (pos < tokens.size()) {
    throw WrongExpressionError("extra tokens detected");
  }
  return expression->Calculate();
}
