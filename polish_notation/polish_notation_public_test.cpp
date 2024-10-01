// Copyright (c) 2024 MIPT-school of applied mathematics and computer science
// Author: Bulat Ibragimov
//
// Данное программное обеспечение распространяется на условиях лицензии MIT.
// Подробности смотрите в файле LICENSE

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <type_traits>
#include "polish_notation.h"
#include "expressions.h"

using ExpressionPtr = std::unique_ptr<IExpression>;

TEST_CASE("IsAbstract1", "[IExpression]") {
  REQUIRE(std::is_abstract_v<IExpression>);
}

TEST_CASE("Constant", "[IExpression]") {
  REQUIRE((std::is_base_of_v<IExpression, Constant>));

  const ExpressionPtr number = std::make_unique<Constant>(11);
  REQUIRE(number->Calculate() == 11);
}

TEST_CASE("IsAbstract2", "[IUnaryOperation]") {
  REQUIRE(std::is_abstract_v<IUnaryOperation>);
  REQUIRE((std::is_base_of_v<IExpression, IUnaryOperation>));
}

TEST_CASE("Plus", "[IUnaryOperation]") {
  REQUIRE((std::is_base_of_v<IUnaryOperation, Plus>));

  {
    const ExpressionPtr plus = std::make_unique<Plus>(std::make_unique<Constant>(2));
    REQUIRE(plus->Calculate() == 2);
  }

  {
    const ExpressionPtr plus = std::make_unique<Plus>(std::make_unique<Constant>(-3));
    REQUIRE(plus->Calculate() == -3);
  }

  {
    const ExpressionPtr plus = std::make_unique<Plus>(std::make_unique<Plus>(std::make_unique<Constant>(0)));
    REQUIRE(plus->Calculate() == 0);
  }
}

TEST_CASE("Minus", "[IUnaryOperation]") {
  REQUIRE((std::is_base_of_v<IUnaryOperation, Minus>));

  {
    const ExpressionPtr minus = std::make_unique<Minus>(std::make_unique<Constant>(2));
    REQUIRE(minus->Calculate() == -2);
  }

  {
    const ExpressionPtr minus = std::make_unique<Minus>(std::make_unique<Constant>(-3));
    REQUIRE(minus->Calculate() == 3);
  }

  {
    const ExpressionPtr minus = std::make_unique<Minus>(std::make_unique<Constant>(0));
    REQUIRE(minus->Calculate() == 0);
  }

  {
    const ExpressionPtr minus = std::make_unique<Minus>(std::make_unique<Minus>(std::make_unique<Constant>(-5)));
    REQUIRE(minus->Calculate() == -5);
  }
}

TEST_CASE("Square", "[IUnaryOperation]") {
  REQUIRE((std::is_base_of_v<IUnaryOperation, Square>));

  {
    const ExpressionPtr square = std::make_unique<Square>(std::make_unique<Constant>(1));
    REQUIRE(square->Calculate() == 1);
  }

  {
    const ExpressionPtr square = std::make_unique<Square>(std::make_unique<Constant>(2));
    REQUIRE(square->Calculate() == 4);
  }

  {
    const ExpressionPtr square = std::make_unique<Square>(std::make_unique<Constant>(-3));
    REQUIRE(square->Calculate() == 9);
  }

  {
    const ExpressionPtr square = std::make_unique<Square>(std::make_unique<Square>(std::make_unique<Constant>(4)));
    REQUIRE(square->Calculate() == 256);
  }
}

TEST_CASE("AbsoluteValue", "[IUnaryOperation]") {
  REQUIRE((std::is_base_of_v<IUnaryOperation, AbsoluteValue>));

  {
    const ExpressionPtr abs = std::make_unique<AbsoluteValue>(std::make_unique<Constant>(-1));
    REQUIRE(abs->Calculate() == 1);
  }

  {
    const ExpressionPtr abs = std::make_unique<AbsoluteValue>(std::make_unique<Constant>(2));
    REQUIRE(abs->Calculate() == 2);
  }

  {
    const ExpressionPtr abs = std::make_unique<AbsoluteValue>(std::make_unique<Constant>(-3));
    REQUIRE(abs->Calculate() == 3);
  }

  {
    const ExpressionPtr abs = std::make_unique<AbsoluteValue>(std::make_unique<Square>(std::make_unique<Constant>(4)));
    REQUIRE(abs->Calculate() == 16);
  }
}

TEST_CASE("IsAbstract3", "[IBinaryOperation]") {
  REQUIRE(std::is_abstract_v<IBinaryOperation>);
  REQUIRE((std::is_base_of_v<IExpression, IBinaryOperation>));
}

TEST_CASE("Sum", "[IBinaryOperation]") {
  REQUIRE((std::is_base_of_v<IBinaryOperation, Sum>));

  {
    const ExpressionPtr sum = std::make_unique<Sum>(std::make_unique<Constant>(-1), std::make_unique<Constant>(3));
    REQUIRE(sum->Calculate() == 2);
  }

  {
    const ExpressionPtr sum = std::make_unique<Sum>(std::make_unique<Constant>(0), std::make_unique<Constant>(3));
    REQUIRE(sum->Calculate() == 3);
  }

  {
    const ExpressionPtr sum = std::make_unique<Sum>(std::make_unique<Constant>(-1), std::make_unique<Constant>(-3));
    REQUIRE(sum->Calculate() == -4);
  }

  {
    const ExpressionPtr sum = std::make_unique<Sum>(std::make_unique<AbsoluteValue>(std::make_unique<Constant>(-1)),
                                                    std::make_unique<Square>(std::make_unique<Constant>(-3)));
    REQUIRE(sum->Calculate() == 10);
  }
}

TEST_CASE("Subtract", "[IBinaryOperation]") {
  REQUIRE((std::is_base_of_v<IBinaryOperation, Subtract>));

  {
    const ExpressionPtr subtract =
        std::make_unique<Subtract>(std::make_unique<Constant>(-1), std::make_unique<Constant>(3));
    REQUIRE(subtract->Calculate() == -4);
  }

  {
    const ExpressionPtr subtract =
        std::make_unique<Subtract>(std::make_unique<Constant>(0), std::make_unique<Constant>(3));
    REQUIRE(subtract->Calculate() == -3);
  }

  {
    const ExpressionPtr subtract =
        std::make_unique<Subtract>(std::make_unique<Constant>(-1), std::make_unique<Constant>(-3));
    REQUIRE(subtract->Calculate() == 2);
  }

  {
    const ExpressionPtr subtract = std::make_unique<Subtract>(  // (|-1| + 5) - (-3)^2
        std::make_unique<Sum>(std::make_unique<AbsoluteValue>(std::make_unique<Constant>(-1)),
                              std::make_unique<Constant>(5)),
        std::make_unique<Square>(std::make_unique<Constant>(-3)));
    REQUIRE(subtract->Calculate() == -3);
  }
}

TEST_CASE("Multiply", "[IBinaryOperation]") {
  REQUIRE((std::is_base_of_v<IBinaryOperation, Multiply>));

  {
    const ExpressionPtr multiply =
        std::make_unique<Multiply>(std::make_unique<Constant>(-1), std::make_unique<Constant>(3));
    REQUIRE(multiply->Calculate() == -3);
  }

  {
    const ExpressionPtr multiply =
        std::make_unique<Multiply>(std::make_unique<Constant>(0), std::make_unique<Constant>(3));
    REQUIRE(multiply->Calculate() == 0);
  }

  {
    const ExpressionPtr multiply =
        std::make_unique<Multiply>(std::make_unique<Constant>(-1), std::make_unique<Constant>(-3));
    REQUIRE(multiply->Calculate() == 3);
  }

  {
    const ExpressionPtr multiply = std::make_unique<Multiply>(  // (|-1| + 5) * ((-3)^2 - 4)
        std::make_unique<Sum>(std::make_unique<AbsoluteValue>(std::make_unique<Constant>(-1)),
                              std::make_unique<Constant>(5)),
        std::make_unique<Subtract>(std::make_unique<Square>(std::make_unique<Constant>(-3)),
                                   std::make_unique<Constant>(4)));
    REQUIRE(multiply->Calculate() == 30);
  }
}

TEST_CASE("Divide", "[IBinaryOperation]") {
  REQUIRE((std::is_base_of_v<IBinaryOperation, Divide>));

  {
    const ExpressionPtr divide =
        std::make_unique<Divide>(std::make_unique<Constant>(-11), std::make_unique<Constant>(3));
    REQUIRE(divide->Calculate() == -3);
  }

  {
    const ExpressionPtr divide = std::make_unique<Divide>(std::make_unique<Constant>(0), std::make_unique<Constant>(3));
    REQUIRE(divide->Calculate() == 0);
  }

  {
    const ExpressionPtr divide =
        std::make_unique<Divide>(std::make_unique<Constant>(-11), std::make_unique<Constant>(-3));
    REQUIRE(divide->Calculate() == 3);
  }

  {
    const ExpressionPtr divide =  // (|-1| + 5) / ((-3)^2 - 4)
        std::make_unique<Divide>(std::make_unique<Sum>(std::make_unique<AbsoluteValue>(std::make_unique<Constant>(-1)),
                                                       std::make_unique<Constant>(5)),
                                 std::make_unique<Subtract>(std::make_unique<Square>(std::make_unique<Constant>(-3)),
                                                            std::make_unique<Constant>(4)));
    REQUIRE(divide->Calculate() == 1);
  }
}

TEST_CASE("Residual", "[IBinaryOperation]") {
  REQUIRE((std::is_base_of_v<IBinaryOperation, Residual>));

  {
    const ExpressionPtr residual =
        std::make_unique<Residual>(std::make_unique<Constant>(-11), std::make_unique<Constant>(3));
    REQUIRE(residual->Calculate() == -2);
  }

  {
    const ExpressionPtr residual =
        std::make_unique<Residual>(std::make_unique<Constant>(0), std::make_unique<Constant>(3));
    REQUIRE(residual->Calculate() == 0);
  }

  {
    const ExpressionPtr residual =
        std::make_unique<Residual>(std::make_unique<Constant>(-11), std::make_unique<Constant>(-3));
    REQUIRE(residual->Calculate() == -2);
  }

  {
    const ExpressionPtr residual = std::make_unique<Residual>(
        std::make_unique<Sum>(std::make_unique<AbsoluteValue>(std::make_unique<Constant>(-1)),
                              std::make_unique<Constant>(5)),
        std::make_unique<Subtract>(std::make_unique<Square>(std::make_unique<Constant>(-3)),
                                   std::make_unique<Constant>(4)));
    REQUIRE(residual->Calculate() == 1);
  }
}

TEST_CASE("Minimum", "[IBinaryOperation]") {
  REQUIRE((std::is_base_of_v<IBinaryOperation, Minimum>));

  {
    const ExpressionPtr min = std::make_unique<Minimum>(std::make_unique<Constant>(-1), std::make_unique<Constant>(3));
    REQUIRE(min->Calculate() == -1);
  }

  {
    const ExpressionPtr min = std::make_unique<Minimum>(std::make_unique<Constant>(0), std::make_unique<Constant>(3));
    REQUIRE(min->Calculate() == 0);
  }

  {
    const ExpressionPtr min = std::make_unique<Minimum>(std::make_unique<Constant>(-1), std::make_unique<Constant>(-3));
    REQUIRE(min->Calculate() == -3);
  }

  {
    const ExpressionPtr min =
        std::make_unique<Minimum>(std::make_unique<Sum>(std::make_unique<AbsoluteValue>(std::make_unique<Constant>(-1)),
                                                        std::make_unique<Constant>(5)),
                                  std::make_unique<Subtract>(std::make_unique<Square>(std::make_unique<Constant>(-3)),
                                                             std::make_unique<Constant>(4)));
    REQUIRE(min->Calculate() == 5);
  }
}

TEST_CASE("Maximum", "[IBinaryOperation]") {
  REQUIRE((std::is_base_of_v<IBinaryOperation, Maximum>));

  {
    const ExpressionPtr max = std::make_unique<Maximum>(std::make_unique<Constant>(-1), std::make_unique<Constant>(3));
    REQUIRE(max->Calculate() == 3);
  }

  {
    const ExpressionPtr max = std::make_unique<Maximum>(std::make_unique<Constant>(0), std::make_unique<Constant>(3));
    REQUIRE(max->Calculate() == 3);
  }

  {
    const ExpressionPtr max = std::make_unique<Maximum>(std::make_unique<Constant>(-1), std::make_unique<Constant>(-3));
    REQUIRE(max->Calculate() == -1);
  }

  {
    const ExpressionPtr max =
        std::make_unique<Maximum>(std::make_unique<Sum>(std::make_unique<AbsoluteValue>(std::make_unique<Constant>(-1)),
                                                        std::make_unique<Constant>(5)),
                                  std::make_unique<Subtract>(std::make_unique<Square>(std::make_unique<Constant>(-3)),
                                                             std::make_unique<Constant>(4)));
    REQUIRE(max->Calculate() == 6);
  }
}

TEST_CASE("ConstantExpression", "[PolishNotation]") {
  REQUIRE(CalculatePolishNotation("5") == 5);
  REQUIRE(CalculatePolishNotation("  5 ") == 5);
}

TEST_CASE("UnaryOperations", "[PolishNotation]") {
  REQUIRE(CalculatePolishNotation("+ 5 ") == 5);
  REQUIRE(CalculatePolishNotation(" -5") == -5);
  REQUIRE(CalculatePolishNotation(" sqr 5 ") == 25);
  REQUIRE(CalculatePolishNotation("abs   5") == 5);
  REQUIRE(CalculatePolishNotation("abs -   5 ") == 5);
  REQUIRE(CalculatePolishNotation("  sqr   +5") == 25);
  REQUIRE(CalculatePolishNotation("+-+ -5") == 5);
  REQUIRE(CalculatePolishNotation(" +  - +- -5") == -5);
}

TEST_CASE("BinaryOperations", "[PolishNotation]") {
  REQUIRE(CalculatePolishNotation(" +  4 9 ") == 13);
  REQUIRE(CalculatePolishNotation(" -  4   9 ") == -5);
  REQUIRE(CalculatePolishNotation("*  4 9") == 36);
  REQUIRE(CalculatePolishNotation(" /  4 9 ") == 0);
  REQUIRE(CalculatePolishNotation("%  4 9 ") == 4);
  REQUIRE(CalculatePolishNotation("   min  4 9 ") == 4);
  REQUIRE(CalculatePolishNotation(" max  4   9  ") == 9);
}

TEST_CASE("GeneralCases", "[PolishNotation]") {
  REQUIRE(CalculatePolishNotation("  + 3 - 2 -4") == 9);  // 3 + (2 - (-4))
  REQUIRE(CalculatePolishNotation("-   4 abs -6") == -2);
  REQUIRE(CalculatePolishNotation("min max   2 3 sqr abs -3") == 3);
  REQUIRE(CalculatePolishNotation("sqr sqr - 2   + 4 min 5 0") == 16);
  REQUIRE(CalculatePolishNotation(" + + + + + + + 11   22 3 4 5 6 7 8") == 66);
  REQUIRE(CalculatePolishNotation(" min abs sqr abs abs abs   3 abs sqr sqr sqr 10") == 9);
  REQUIRE(CalculatePolishNotation(" max  + + min 2 3 sqr 7 0 19   ") == 51);
}

TEST_CASE("Brackets", "[PolishNotation]") {
  REQUIRE(CalculatePolishNotation("  (+3 4  )") == 7);
  REQUIRE(CalculatePolishNotation(" + (-3) (-4)") == -7);
  REQUIRE(CalculatePolishNotation(" (((+ ((-3)) (+4))))") == 1);
  REQUIRE(CalculatePolishNotation(" + - + (  -3) (* 5 + 1 2  ) 9 / 4 (-2) ") == 1);
  REQUIRE(CalculatePolishNotation("* (+max abs + (-3) / 16 5 1) (-min sqr + 4 % 6 (+2) 100)") == -16);
}

TEST_CASE("UnknownSymbolError", "[Exceptions]") {                                            // unknown symbols
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("whatisthis"), UnknownSymbolError);        // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("+ 3 + 4 Square 6"), UnknownSymbolError);  // NOLINT
}

TEST_CASE("WrongOperandsNumberBase", "[Exceptions]") {
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("sqr"), WrongExpressionError);      // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("abs"), WrongExpressionError);      // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("sqr 1 2"), WrongExpressionError);  // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("abs 1 2"), WrongExpressionError);  // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("* 1"), WrongExpressionError);      // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("/ 1"), WrongExpressionError);      // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("% 1"), WrongExpressionError);      // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("*"), WrongExpressionError);        // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("/"), WrongExpressionError);        // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("%"), WrongExpressionError);        // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("+"), WrongExpressionError);        // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("-"), WrongExpressionError);        // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("min"), WrongExpressionError);      // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("max"), WrongExpressionError);      // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("min 1"), WrongExpressionError);    // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("max 1"), WrongExpressionError);    // NOLINT
}

TEST_CASE("WrongOperandsNumberAdvanced", "[Exceptions]") {
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("sqr abs"), WrongExpressionError);              // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("+ + 1 2 3 4"), WrongExpressionError);          // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("--"), WrongExpressionError);                   // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("min max 1 2 + 3 abs"), WrongExpressionError);  // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("min max 1 + 3 abs 4"), WrongExpressionError);  // NOLINT
}

TEST_CASE("WrongBracketSequence", "[Exceptions]") {
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("1 )"), WrongExpressionError);        // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("( 1"), WrongExpressionError);        // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation(") + 1 2 ("), WrongExpressionError);  // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation(") + 1 2"), WrongExpressionError);    // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("( + 1 2"), WrongExpressionError);    // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation(") + 1 2 ("), WrongExpressionError);  // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation(") + 1 2 )"), WrongExpressionError);  // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("( + 1 2 ("), WrongExpressionError);  // NOLINT
}

TEST_CASE("WrongExpressionError", "[Exceptions]") {
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("3 -3"), WrongExpressionError);                     // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("+ 3 5 4"), WrongExpressionError);                  // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("abs 3 3"), WrongExpressionError);                  // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("+ 1 1 + 1 1"), WrongExpressionError);              // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("sqr 1 sqr 1"), WrongExpressionError);              // NOLINT
  REQUIRE_THROWS_AS((void)CalculatePolishNotation("sqr sqr - 2 + 6 min 5 0 min max 2 3 sqr abs -3"),  // NOLINT
                    WrongExpressionError);
}
