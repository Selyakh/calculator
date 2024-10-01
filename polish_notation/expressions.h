// Copyright (c) 2024 Bulat Ibragimov, Irina Selyakh
//
// Данное программное обеспечение распространяется на условиях лицензии MIT.
// Подробности смотрите в файле LICENSE

#ifndef MY_EXPRESSIONS_H  // объявление классов операций
#define MY_EXPRESSIONS_H

#include <memory>

class IExpression {  // абстрактный базовый класс для всех выражений
 public:
  virtual int Calculate() const = 0;  // чисто виртуальная функция для вычисления значения выражения

  virtual ~IExpression() = default;
};

class Constant final : public IExpression {  // класс для представления константных значений.
  int value_;

 public:
  explicit Constant(int value) : value_(value) {
  }

  int Calculate() const final {
    return value_;
  }
};

class IUnaryOperation
    : public IExpression {  // абстрактный класс, наследник `IExpression`, представляющий произвольную унарную операцию
  std::unique_ptr<IExpression> operand_;

 public:
  explicit IUnaryOperation(std::unique_ptr<IExpression> operand) : operand_(std::move(operand)) {
  }

  int Calculate() const final {
    const auto x = operand_->Calculate();  // вычисление операнда
    return Operation(x);
  }

  virtual int Operation(int x) const = 0;  // чисто виртуальная функция для выполнения операции
};

class Square final : public IUnaryOperation {
 public:
  using IUnaryOperation::IUnaryOperation;  // перенос конструктора

  int Operation(int x) const final {
    return x * x;
  }
};

class AbsoluteValue final : public IUnaryOperation {  // возвращает абсолютное значение
 public:
  using IUnaryOperation::IUnaryOperation; // перенос конструктора

  int Operation(int x) const final {
    return x < 0 ? -x : x;
  }
};

class Minus final : public IUnaryOperation {
 public:
  using IUnaryOperation::IUnaryOperation; // перенос конструктора

  int Operation(int x) const final {
    return -x;
  }
};

class Plus final : public IUnaryOperation {
 public:
  using IUnaryOperation::IUnaryOperation; // перенос конструктора

  int Operation(int x) const final {
    return x;
  }
};

class IBinaryOperation
    : public IExpression {  // абстрактный класс, наследник `IExpression`, представляющий произвольную бинарную операцию
  std::unique_ptr<IExpression> left_;
  std::unique_ptr<IExpression> right_;

 public:
  IBinaryOperation(std::unique_ptr<IExpression> left, std::unique_ptr<IExpression> right)
      : left_(std::move(left)), right_(std::move(right)) {
  }

  int Calculate() const final {
    const auto x = left_->Calculate();   // вычисление левого операнда
    const auto y = right_->Calculate();  // вычисление правого операнда
    return Operation(x, y);              // выполнение операции
  }

  virtual int Operation(int x, int y) const = 0;  // чисто виртуальная функция для выполнения операции
};

class Multiply final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operation(int x, int y) const final {
    return x * y;
  }
};

class Sum final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operation(int x, int y) const final {
    return x + y;
  }
};

class Subtract final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operation(int x, int y) const final {
    return x - y;
  }
};

class Divide final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operation(int x, int y) const final {
    return x / y;
  }
};

class Residual final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operation(int x, int y) const final {
    return x % y;
  }
};

class Maximum final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operation(int x, int y) const final {
    return x > y ? x : y;
  }
};

class Minimum final : public IBinaryOperation {
 public:
  using IBinaryOperation::IBinaryOperation;

  int Operation(int x, int y) const final {
    return x < y ? x : y;
  }
};

#endif  // MY_EXPRESSIONS_H