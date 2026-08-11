#ifndef SERIALIZATION_HPP
#define SERIALIZATION_HPP

// ==========================================
// Questão 4 (A)(B) — Serialização JSON
//
// Serialização/desserialização não-intrusiva
// usando nlohmann/json. Inclui campo "version"
// e campo "type" para hierarquia polimórfica.
// ==========================================

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <memory>
#include "domain.hpp"

using json = nlohmann::json;

// Versão do formato de serialização
constexpr int SERIALIZATION_VERSION = 1;

// ==========================================
// to_json / from_json para Person
// ==========================================
inline void to_json(json& j, const Person& p) {
    j = json{
        {"type", "Person"},
        {"id", p.id()},
        {"name", p.get_name()},
        {"age", p.get_age()}
    };
}

inline void from_json(const json& j, Person& p) {
    p = Person(
        j.at("id").get<int>(),
        j.at("name").get<std::string>(),
        j.at("age").get<int>()
    );
}

// ==========================================
// to_json / from_json para WaterBill
// ==========================================
inline void to_json(json& j, const WaterBill& b) {
    j = json{
        {"type", "WaterBill"},
        {"value", b.get_value()},
        {"taxa_esgoto", b.calcular_total() - b.get_value()}
    };
}

inline void from_json(const json& j, WaterBill& b) {
    b = WaterBill(
        j.at("value").get<float>(),
        j.at("taxa_esgoto").get<float>()
    );
}

// ==========================================
// to_json / from_json para EnergyBill
// ==========================================
inline void to_json(json& j, const EnergyBill& b) {
    j = json{
        {"type", "EnergyBill"},
        {"value", b.get_value()},
        {"taxa_iluminacao", b.calcular_total() - b.get_value()}
    };
}

inline void from_json(const json& j, EnergyBill& b) {
    b = EnergyBill(
        j.at("value").get<float>(),
        j.at("taxa_iluminacao").get<float>()
    );
}

// ==========================================
// Estado completo do sistema
//
// Agrupa todas as entidades para serialização
// em um unico documento JSON versionado.
// ==========================================
struct home_state {
    std::vector<Person> people;
    std::vector<WaterBill> water_bills;
    std::vector<EnergyBill> energy_bills;

    // Operador de igualdade para verificação round-trip
    bool operator==(const home_state& other) const {
        if (people.size() != other.people.size()) return false;
        if (water_bills.size() != other.water_bills.size()) return false;
        if (energy_bills.size() != other.energy_bills.size()) return false;

        for (std::size_t i = 0; i < people.size(); ++i) {
            if (!(people[i] == other.people[i])) return false;
        }
        for (std::size_t i = 0; i < water_bills.size(); ++i) {
            if (water_bills[i].calcular_total() != other.water_bills[i].calcular_total())
                return false;
        }
        for (std::size_t i = 0; i < energy_bills.size(); ++i) {
            if (energy_bills[i].calcular_total() != other.energy_bills[i].calcular_total())
                return false;
        }
        return true;
    }
};

// Serialização do estado completo com versão
inline void to_json(json& j, const home_state& s) {
    j = json{
        {"version", SERIALIZATION_VERSION},
        {"people", s.people},
        {"water_bills", s.water_bills},
        {"energy_bills", s.energy_bills}
    };
}

inline void from_json(const json& j, home_state& s) {
    // Tratamento por versão (espaco para evolucao)
    int version = j.at("version").get<int>();
    if (version == 1) {
        s.people = j.at("people").get<std::vector<Person>>();
        s.water_bills = j.at("water_bills").get<std::vector<WaterBill>>();
        s.energy_bills = j.at("energy_bills").get<std::vector<EnergyBill>>();
    }
    // Versoes futuras podem ser tratadas aqui:
    // else if (version == 2) { ... }
}

#endif // SERIALIZATION_HPP
