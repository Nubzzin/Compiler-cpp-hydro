cmake_minimum_required(VERSION 3.20)
project(hydro-compiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(hydro src/main.cpp)

# flag
# add_compile_options()

# Adicione seus alvos e bibliotecas aqui

# Exemplo:
# add_library(MyLibrary STATIC src/library.cpp)
# target_link_libraries(MyExecutable PRIVATE MyLibrary)

# Para mais informações sobre CMake, consulte a documentação oficial:
# https://cmake.org/documentation/

