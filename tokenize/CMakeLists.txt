set(TOKENIZE_SRC ${CMAKE_SOURCE_DIR}/tokenize/tokenize.cpp)

add_executable(tokenize_public_test ${TOKENIZE_SRC} tokenize_public_test.cpp)
