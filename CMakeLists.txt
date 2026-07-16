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

# Cria o executável principal
add_executable(meu_exe src/main.cpp)

# ==========================================
# TESTES (Questão 4) - Catch2
# ==========================================
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
)
FetchContent_MakeAvailable(Catch2)

# As classes foram implementadas de modo header-only (domain.hpp), portanto só precisamos do arquivo de teste
add_executable(testes tests/test_tp2.cpp)
target_include_directories(testes PRIVATE src)
target_link_libraries(testes PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(testes)
