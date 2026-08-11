#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <vector>
#include "domain.hpp"
#include "errors.hpp"
#include "repository.hpp"

// ==========================================
// TEST_CASE 1 (Q1): Template registry + concept
// ==========================================
TEST_CASE("Template registry e concept calculavel", "[q1]") {
    // Template instanciado com WaterBill
    registry<WaterBill> reg;
    reg.add(WaterBill(50.0f, 10.0f));
    reg.add(WaterBill(80.0f, 20.0f));
    REQUIRE(reg.size() == 2);
    REQUIRE(reg.at(0).calcular_total() == Catch::Approx(60.0f));
    REQUIRE(reg.at(1).calcular_total() == Catch::Approx(100.0f));

    // Template instanciado com EnergyBill
    registry<EnergyBill> reg_energy;
    reg_energy.add(EnergyBill(100.0f, 20.0f));
    REQUIRE(reg_energy.size() == 1);

    // Concept calculavel: soma_total funciona com WaterBill
    std::vector<WaterBill> contas;
    contas.emplace_back(40.0f, 10.0f);   // total = 50
    contas.emplace_back(60.0f, 15.0f);   // total = 75
    REQUIRE(soma_total(contas) == Catch::Approx(125.0));

    // Concept calculavel: soma_total funciona com EnergyBill
    std::vector<EnergyBill> energias;
    energias.emplace_back(100.0f, 20.0f);   // total = 120
    REQUIRE(soma_total(energias) == Catch::Approx(120.0));
}

// ==========================================
// TEST_CASE 2 (Q1): CRTP counted
// ==========================================
TEST_CASE("CRTP counted contagem de instancias", "[q1]") {
    int antes = Person::alive();
    {
        Person p1(100, "Teste1", 20);
        Person p2(101, "Teste2", 25);
        REQUIRE(Person::alive() == antes + 2);
    }
    REQUIRE(Person::alive() == antes);
}

// ==========================================
// TEST_CASE 3 (Q2): Exceções + optional
// ==========================================
TEST_CASE("Excecao do dominio capturada pela base", "[q2]") {
    // invalid_bill capturada como domain_error (base)
    REQUIRE_THROWS_AS(validar_valor_conta(-10.0f), domain_error);

    // person_not_found capturada como domain_error (base)
    REQUIRE_THROWS_AS(validar_nome_pessoa(""), domain_error);

    // Valor valido nao lanca excecao
    REQUIRE_NOTHROW(validar_valor_conta(100.0f));
    REQUIRE_NOTHROW(validar_nome_pessoa("Joao"));
}

TEST_CASE("Busca com optional", "[q2]") {
    std::vector<Person> pessoas;
    pessoas.emplace_back(1, "Joao", 20);
    pessoas.emplace_back(2, "Maria", 25);

    // Encontra pessoa existente
    auto encontrado = find_by_name(pessoas, "Joao");
    REQUIRE(encontrado.has_value());
    REQUIRE(encontrado->get_name() == "Joao");

    // Nao encontra pessoa inexistente
    auto nao_encontrado = find_by_name(pessoas, "Pedro");
    REQUIRE_FALSE(nao_encontrado.has_value());
}

// ==========================================
// TEST_CASE 4 (Q2): Variant
// ==========================================
TEST_CASE("Variant resultado sucesso ou erro", "[q2]") {
    // Sucesso: valor positivo
    auto ok = try_calculate(100.0f, 20.0f);
    REQUIRE(std::holds_alternative<double>(ok));
    REQUIRE(std::get<double>(ok) == Catch::Approx(120.0));

    // Erro: valor negativo
    auto erro = try_calculate(-50.0f, 10.0f);
    REQUIRE(std::holds_alternative<std::string>(erro));
}

// ==========================================
// TEST_CASE 5 (Q4): Serialização round-trip
// ==========================================
TEST_CASE("Serializacao round-trip com memory_repository", "[q4]") {
    home_state original;
    original.people.emplace_back(1, "Joao", 20);
    original.people.emplace_back(2, "Maria", 25);
    original.water_bills.emplace_back(100.0f, 20.0f);
    original.energy_bills.emplace_back(80.0f, 15.0f);

    // Salva e carrega usando memory_repository (sem disco)
    memory_repository repo;
    repo.save(original);
    auto carregado = repo.load();

    // Verifica igualdade campo a campo
    REQUIRE(original == carregado);
    REQUIRE(carregado.people.size() == 2);
    REQUIRE(carregado.water_bills.size() == 1);
    REQUIRE(carregado.energy_bills.size() == 1);
    REQUIRE(carregado.people[0].get_name() == "Joao");
    REQUIRE(carregado.people[1].get_name() == "Maria");
}

// ==========================================
// TEST_CASE 6 (Q4): DIP com memory_repository
// ==========================================
TEST_CASE("DIP com memory_repository sem tocar disco", "[q4]") {
    memory_repository repo;
    home_manager manager(repo);

    // Adiciona dados e salva
    manager.add_person(Person(1, "Joao", 20));
    manager.add_water_bill(WaterBill(80.0f, 15.0f));
    manager.add_energy_bill(EnergyBill(60.0f, 10.0f));
    manager.save();

    // Carrega em outro manager (mesma abstração)
    home_manager manager2(repo);
    manager2.load();

    // Verifica que o estado carregado é igual
    REQUIRE(manager.state() == manager2.state());
    REQUIRE(manager2.state().people.size() == 1);
    REQUIRE(manager2.state().water_bills.size() == 1);
    REQUIRE(manager2.state().energy_bills.size() == 1);
}
