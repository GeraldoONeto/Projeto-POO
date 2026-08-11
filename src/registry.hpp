#ifndef REGISTRY_HPP
#define REGISTRY_HPP

// ==========================================
// Questão 1 — Programação Genérica
// Templates, CRTP, Concepts e Ranges (C++20)
// ==========================================

#include <vector>
#include <optional>
#include <functional>
#include <concepts>
#include <ranges>
#include <iostream>
#include <string>
#include <algorithm>

// ==========================================
// (A) Template reutilizavel: registry<T>
//
// Registro genérico indexado que armazena itens
// de qualquer tipo. Reutilizável com qualquer
// classe do domínio (Bill, Person, Task, etc.).
// ==========================================
template <typename T>
class registry {
    std::vector<T> items_;

public:
    // Adiciona um item ao registro
    void add(T item) { items_.push_back(std::move(item)); }

    // Acessa item por indice (com verificacao de limites)
    const T& at(std::size_t i) const { return items_.at(i); }

    // Retorna o numero de itens registrados
    std::size_t size() const { return items_.size(); }

    // Verifica se o registro esta vazio
    bool empty() const { return items_.empty(); }

    // Acesso ao vetor interno para iteracao
    const std::vector<T>& all() const { return items_; }

    // Iteradores para uso em ranges e for-each
    auto begin() const { return items_.begin(); }
    auto end() const { return items_.end(); }
    auto begin() { return items_.begin(); }
    auto end() { return items_.end(); }
};

// ==========================================
// (B) CRTP: counted<Derived>
//
// Mixin de contagem de instancias vivas, sem
// custo de vtable. Cada classe derivada tem
// seu proprio contador estatico independente.
// ==========================================
template <typename Derived>
class counted {
    static inline int count_ = 0;

public:
    counted() { ++count_; }
    counted(const counted&) { ++count_; }
    counted(counted&&) { ++count_; }
    ~counted() { --count_; }

    counted& operator=(const counted&) = default;
    counted& operator=(counted&&) = default;

    // Retorna quantas instancias do tipo Derived estao vivas
    static int alive() { return count_; }
};

// ==========================================
// (C) Concept: calculavel
//
// Restringe templates a tipos que possuem
// o metodo calcular_total() const, retornando
// algo conversivel para double.
// ==========================================
template <typename T>
concept calculavel = requires(const T& t) {
    { t.calcular_total() } -> std::convertible_to<double>;
};

// ==========================================
// (D) Funcao template restringida pelo concept
//
// Soma os totais de todos os itens calculaveis.
// Violar o concept (ex: usar int) gera um erro
// de compilacao claro, nao um erro obscuro de
// template com centenas de linhas.
// ==========================================
template <calculavel T>
double soma_total(const std::vector<T>& v) {
    double s = 0.0;
    for (const auto& x : v) s += x.calcular_total();
    return s;
}

// ==========================================
// (E) Pipeline de ranges (C++20)
//
// Filtra e transforma colecoes do dominio
// usando adaptadores encadeados, substituindo
// lacos manuais por codigo declarativo.
// ==========================================
namespace rv = std::ranges::views;

// Filtra contas com total acima de um limite e retorna os totais
// Pipeline: filter (seleciona caras) | transform (extrai valor)
template <calculavel T>
void exibir_contas_caras(const std::vector<T>& contas, double limite) {
    std::cout << "  Contas com total > R$" << limite << ":\n";

    // Pipeline de ranges com 2 adaptadores encadeados:
    // 1. filter: seleciona apenas contas acima do limite
    // 2. transform: extrai o valor total de cada conta
    auto totais_caros = contas
        | rv::filter([limite](const auto& c) { return c.calcular_total() > limite; })
        | rv::transform([](const auto& c) { return c.calcular_total(); });

    for (double total : totais_caros) {
        std::cout << "    R$" << total << "\n";
    }
}

#endif // REGISTRY_HPP
