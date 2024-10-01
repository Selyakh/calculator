// Copyright (c) 2024 Bulat Ibragimov, Irina Selyakh
//
// Данное программное обеспечение распространяется на условиях лицензии MIT.
// Подробности смотрите в файле LICENSE

#include "../calculator/parser.h"

std::unique_ptr<IExpression> ParseExpression(const std::vector<Token>& tokens, size_t& pos) { // обрабатывает выр-ия с операциями сложения и вычитания
  if (pos >= tokens.size()) {
    throw WrongExpressionError("too few arguments");
  }
  auto expression = ParseTerm(tokens, pos); // анализ компоненты выражения
  while (pos < tokens.size()) {
    const auto& token = tokens[pos];
    if (std::holds_alternative<PlusToken>(token)) {
      expression = std::make_unique<Sum>(std::move(expression), ParseTerm(tokens, ++pos));
    } else if (std::holds_alternative<MinusToken>(token)) {
      expression = std::make_unique<Subtract>(std::move(expression), ParseTerm(tokens, ++pos));
    } else if (std::holds_alternative<UnknownToken>(token)) {
      throw UnknownSymbolError("Unknown symbol: " + std::get<UnknownToken>(token).value);
    } else {
      break;
    }
  }
  return expression;
}

std::unique_ptr<IExpression> ParseTerm(const std::vector<Token>& tokens, size_t& pos) {  // обрабатывает *, /, %
  if (pos >= tokens.size()) {
    throw WrongExpressionError("too few arguments");
  }
  auto summand = ParseFactor(tokens, pos);
  while (pos < tokens.size()) {
    const auto& token = tokens[pos];
    if (std::holds_alternative<MultiplyToken>(token)) {
      summand = std::make_unique<Multiply>(std::move(summand), ParseFactor(tokens, ++pos));
    } else if (std::holds_alternative<DivideToken>(token)) {
      summand = std::make_unique<Divide>(std::move(summand), ParseFactor(tokens, ++pos));
    } else if (std::holds_alternative<ResidualToken>(token)) {
      summand = std::make_unique<Residual>(std::move(summand), ParseFactor(tokens, ++pos));
    } else if (std::holds_alternative<UnknownToken>(token)) {
      throw UnknownSymbolError("Unknown symbol: " + std::get<UnknownToken>(token).value);
    } else {
      break;
    }
  }
  return summand;
}

std::unique_ptr<IExpression> ParseFactor(const std::vector<Token>& tokens, size_t& pos) { // обрабатывает все остальное
  if (pos >= tokens.size()) {
    throw WrongExpressionError("too few arguments");
  }
  const auto& token = tokens[pos++];

  if (std::holds_alternative<UnknownToken>(token)) {
    throw UnknownSymbolError("Unknown token: " + std::get<UnknownToken>(token).value);
  }

  if (std::holds_alternative<ClosingBracketToken>(token)) {
    throw WrongExpressionError("No matching (");
  }

  if (std::holds_alternative<OpeningBracketToken>(token)) {
    auto sub_expression = ParseExpression(tokens, pos);
    if (pos >= tokens.size() || !std::holds_alternative<ClosingBracketToken>(tokens[pos++])) {
      throw WrongExpressionError("No matching )");
    }
    return sub_expression;
  }

  if (std::holds_alternative<NumberToken>(token)) {
    return std::make_unique<Constant>(std::get<NumberToken>(token).value);
  }

  if (std::holds_alternative<SqrToken>(token)) {
    return ParseFactor(tokens, pos);
  }

  if (std::holds_alternative<AbsToken>(token)) {
    return ParseFactor(tokens, pos);
  }

  if (std::holds_alternative<PlusToken>(token)) {
    return ParseFactor(tokens, pos);
  }

  if (std::holds_alternative<MinusToken>(token)) {
    auto arg = ParseFactor(tokens, pos);
    return std::make_unique<Minus>(std::move(arg));
  }
  throw WrongExpressionError("Invalid token");
}