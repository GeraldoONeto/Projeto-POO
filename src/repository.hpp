#ifndef REPOSITORY_HPP
#define REPOSITORY_HPP

// ==========================================
// Questão 4 (C)(D) — DIP: Abstração de Repositório
//
// A classe de alto nivel (home_manager) depende
// da abstração (repository), não da implementação
// concreta. A dependência é injetada pelo construtor.
// ==========================================

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "serialization.hpp"

using json = nlohmann::json;

// ==========================================
// Abstração: repository
//
// Interface pura que define o contrato para
// salvar e carregar o estado do sistema.
// ==========================================
class repository {
public:
    virtual void save(const home_state& state) = 0;
    virtual home_state load() = 0;
    virtual ~repository() = default;
};

// ==========================================
// Implementação de produção: json_repository
//
// Grava e lê o estado em um arquivo JSON no disco.
// ==========================================
class json_repository : public repository {
    std::string filepath_;

public:
    explicit json_repository(const std::string& filepath)
        : filepath_(filepath) {}

    void save(const home_state& state) override {
        json j = state;
        std::ofstream file(filepath_);
        if (!file.is_open()) {
            throw std::runtime_error("nao foi possivel abrir arquivo: " + filepath_);
        }
        file << j.dump(2);
        std::cout << "  [json_repository] Estado salvo em " << filepath_ << "\n";
    }

    home_state load() override {
        std::ifstream file(filepath_);
        if (!file.is_open()) {
            throw std::runtime_error("nao foi possivel abrir arquivo: " + filepath_);
        }
        json j;
        file >> j;
        std::cout << "  [json_repository] Estado carregado de " << filepath_ << "\n";
        return j.get<home_state>();
    }
};

// ==========================================
// Implementação de teste: memory_repository
//
// Armazena o estado em memória, sem efeito
// colateral (sem criar arquivo, sem I/O).
// Ideal para testes unitarios e validacao
// da logica sem tocar o disco.
// ==========================================
class memory_repository : public repository {
    json stored_;
    bool has_data_ = false;

public:
    void save(const home_state& state) override {
        stored_ = state;
        has_data_ = true;
        std::cout << "  [memory_repository] Estado salvo em memoria\n";
    }

    home_state load() override {
        if (!has_data_) {
            throw std::runtime_error("memory_repository: nenhum dado armazenado");
        }
        std::cout << "  [memory_repository] Estado carregado da memoria\n";
        return stored_.get<home_state>();
    }
};

// ==========================================
// Classe de alto nível: home_manager
//
// Depende da abstração repository&, não de
// uma implementação concreta. A dependência
// é recebida por injeção no construtor.
// ==========================================
class home_manager {
    repository& repo_;
    home_state state_;

public:
    // Injeção de dependência: recebe a abstração
    explicit home_manager(repository& repo) : repo_(repo) {}

    // Adiciona uma pessoa ao estado
    void add_person(const Person& p) {
        state_.people.push_back(p);
    }

    // Adiciona uma conta de agua ao estado
    void add_water_bill(const WaterBill& b) {
        state_.water_bills.push_back(b);
    }

    // Adiciona uma conta de energia ao estado
    void add_energy_bill(const EnergyBill& b) {
        state_.energy_bills.push_back(b);
    }

    // Salva o estado atual usando o repositório injetado
    void save() {
        repo_.save(state_);
    }

    // Carrega o estado do repositório injetado
    void load() {
        state_ = repo_.load();
    }

    // Acessa o estado atual (para verificações)
    const home_state& state() const { return state_; }

    // Executa logica de alto nivel (processamento das contas)
    void process() {
        double total = 0.0;
        for (const auto& b : state_.water_bills) {
            total += b.calcular_total();
        }
        for (const auto& b : state_.energy_bills) {
            total += b.calcular_total();
        }
        std::cout << "  Total de todas as contas: R$" << total << "\n";
        std::cout << "  Moradores: " << state_.people.size() << "\n";
    }
};

#endif // REPOSITORY_HPP
