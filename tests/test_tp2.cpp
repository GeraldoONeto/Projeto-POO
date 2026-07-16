#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <memory>
#include <vector>
#include "../src/domain.hpp"

// ==========================================
// TEST_CASE 1 (Q1): Construtores e destrutores
// ==========================================
TEST_CASE("Hierarquia - Destrutores e tipos", "[q1]") {
    Bill* temp = new WaterBill(50.0f, 15.0f);
    REQUIRE(temp->calcular_total() == Catch::Approx(65.0f));
    delete temp;

    Bill* e = new EnergyBill(100.0f, 20.0f);
    REQUIRE(e->calcular_total() == Catch::Approx(120.0f));
    delete e;
}

// ==========================================
// TEST_CASE 2 (Q2): Polimorfismo
// ==========================================
TEST_CASE("Polimorfismo com std::unique_ptr", "[q2]") {
    std::vector<std::unique_ptr<Bill>> vetor;
    vetor.push_back(std::make_unique<WaterBill>(40.0f, 10.0f));
    vetor.push_back(std::make_unique<EnergyBill>(80.0f, 5.0f));

    REQUIRE(vetor.size() == 2);
    REQUIRE(vetor[0]->calcular_total() == Catch::Approx(50.0f));
    REQUIRE(vetor[1]->calcular_total() == Catch::Approx(85.0f));
    
    const Bill* max = get_max_bill(vetor);
    REQUIRE(max != nullptr);
    REQUIRE(max->calcular_total() == Catch::Approx(85.0f));
}

// ==========================================
// TEST_CASE 3 (Q3): Interface pura
// ==========================================
TEST_CASE("Interface pura por referencia", "[q3]") {
    WaterBill water(100.0f, 25.0f);
    const IPayable& payable = water;
    
    REQUIRE_NOTHROW(payable.process_payment());
    REQUIRE_NOTHROW(pay_bill(water));
}
