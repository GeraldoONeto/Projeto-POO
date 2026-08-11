cmake_minimum_required(VERSION 3.14)
project(TrabalhoPratico3)

# Exigência do roteiro: C++20 (necessario para concepts e ranges)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Teste 1: Esperado zero erros, zero warnings com -Wall -Wextra
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Teste 9: AddressSanitizer em modo Debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

# ==========================================
# Dependencias externas via FetchContent
# ==========================================
include(FetchContent)

# nlohmann/json — serialização JSON (Questão 4)
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)

# Catch2 v3 — framework de testes (Questão 5)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
)

FetchContent_MakeAvailable(json Catch2)

# ==========================================
# Executável principal
# ==========================================
add_executable(meu_exe src/main.cpp)
target_include_directories(meu_exe PRIVATE src)
target_link_libraries(meu_exe PRIVATE nlohmann_json::nlohmann_json)

# ==========================================
# Testes TP2 (legado)
# ==========================================
add_executable(testes tests/test_tp2.cpp)
target_include_directories(testes PRIVATE src)
target_link_libraries(testes PRIVATE Catch2::Catch2WithMain nlohmann_json::nlohmann_json)

# ==========================================
# Testes TP3 (Questão 5)
# ==========================================
add_executable(testes_tp3 tests/test_tp3.cpp)
target_include_directories(testes_tp3 PRIVATE src)
target_link_libraries(testes_tp3 PRIVATE Catch2::Catch2WithMain nlohmann_json::nlohmann_json)

include(CTest)
include(Catch)
catch_discover_tests(testes)
catch_discover_tests(testes_tp3)

# ==========================================
# Interface Gráfica Qt (Questão 6 — extra)
# ==========================================
find_package(Qt6 QUIET COMPONENTS Widgets)
if(Qt6_FOUND)
    message(STATUS "Qt6 encontrado — target 'gui' habilitado")
    set(CMAKE_AUTOMOC ON)
    qt_add_executable(gui src/gui_main.cpp src/main_window.hpp)
    target_include_directories(gui PRIVATE src)
    target_link_libraries(gui PRIVATE Qt6::Widgets nlohmann_json::nlohmann_json)
else()
    message(STATUS "Qt6 nao encontrado — target 'gui' desabilitado (instale qt6-base-dev)")
endif()

