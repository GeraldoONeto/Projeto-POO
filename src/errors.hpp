#ifndef ERRORS_HPP
#define ERRORS_HPP

// ==========================================
// Questão 2 — Tratamento de Erros
// Exceções, std::optional e std::variant
// ==========================================

#include <stdexcept>
#include <string>
#include <optional>
#include <variant>
#include <vector>
#include <iostream>

// ==========================================
// (A) Hierarquia de exceções própria
//
// Base: domain_error herda de std::runtime_error
// Especifica 1: invalid_bill (valor negativo)
// Especifica 2: person_not_found (busca falhou)
// ==========================================
class domain_error : public std::runtime_error {
public:
    using std::runtime_error::runtime_error; // herda o construtor
};

// Exceção para contas com dados inválidos
class invalid_bill : public domain_error {
public:
    explicit invalid_bill(const std::string& msg)
        : domain_error{"conta invalida: " + msg} {}
};

// Exceção para pessoa não encontrada
class person_not_found : public domain_error {
public:
    explicit person_not_found(const std::string& msg)
        : domain_error{"pessoa nao encontrada: " + msg} {}
};

// ==========================================
// (A) Funcao de validacao que lanca excecao
//
// Valida o valor de uma conta — se negativo,
// lanca invalid_bill. Se nome vazio, lanca
// person_not_found.
// ==========================================
inline void validar_valor_conta(float valor) {
    if (valor < 0) {
        throw invalid_bill("valor negativo: " + std::to_string(valor));
    }
}

inline void validar_nome_pessoa(const std::string& nome) {
    if (nome.empty()) {
        throw person_not_found("nome nao pode ser vazio");
    }
}

// ==========================================
// (B) std::optional para busca que pode falhar
//
// Retorna std::nullopt quando nao encontra,
// em vez de lancar excecao ou devolver nullptr.
// ==========================================
template <typename T>
std::optional<T> find_by_name(const std::vector<T>& items, const std::string& name) {
    for (const auto& item : items) {
        if (item.get_name() == name) return item; // achou
    }
    return std::nullopt; // nao achou
}

// ==========================================
// (C) std::variant: resultado de operacao
//
// Representa sucesso (valor double) ou erro
// (mensagem string). Tratado com std::visit.
// ==========================================
using operation_result = std::variant<double, std::string>;

// Tenta calcular o total de uma conta — retorna o valor ou uma mensagem de erro
inline operation_result try_calculate(float valor, float taxa) {
    if (valor < 0) {
        return std::string{"erro: valor base negativo (" + std::to_string(valor) + ")"};
    }
    return static_cast<double>(valor + taxa); // sucesso
}

// Visitor para tratar o variant — imprime resultado ou erro
struct result_visitor {
    void operator()(double valor) const {
        std::cout << "    Sucesso: total = R$" << valor << "\n";
    }
    void operator()(const std::string& erro) const {
        std::cout << "    Falha: " << erro << "\n";
    }
};

#endif // ERRORS_HPP
