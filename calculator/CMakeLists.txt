set(TOKENIZE_SRC ${CMAKE_SOURCE_DIR}/tokenize/tokenize.cpp)
set(CALCULATOR_SRC calculator.cpp parser.cpp)

add_executable(calculator_public_test ${TOKENIZE_SRC} ${CALCULATOR_SRC} calculator_public_test.cpp)
