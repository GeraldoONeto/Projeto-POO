cmake_minimum_required(VERSION 3.10)
project(TrabalhoPratico1)

# Exigência do roteiro: C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Teste 1: Esperado zero erros, zero warnings com -Wall -Wextra
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Teste 7: AddressSanitizer em modo Debug (Questão 4)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

# Cria o executável exigido (meu_exe) apontando para o src/main.cpp
add_executable(meu_exe src/main.cpp)
