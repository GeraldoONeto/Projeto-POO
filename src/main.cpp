#include <iostream>
#include <vector>
#include <memory>
#include "domain.hpp"

int main() {
    std::cout << "--- Inicio do Trabalho Pratico 2 ---\n\n";

    // ==========================================
    // Questão 1 (C): Destrutor Virtual em Cadeia
    // ==========================================
    std::cout << "[Q1] Destrutor virtual:\n";
    {
        Bill* base_ptr = new WaterBill(50.0f, 15.0f);
        delete base_ptr;
    }
    std::cout << "\n";

    // ==========================================
    // Questão 2 (A, B, C, D): Polimorfismo Dinâmico
    // ==========================================
    std::cout << "[Q2] Polimorfismo e Vetor de unique_ptr:\n";
    {
        std::vector<std::unique_ptr<Bill>> contas;
        contas.push_back(std::make_unique<WaterBill>(40.0f, 10.0f));
        contas.push_back(std::make_unique<EnergyBill>(80.0f, 15.0f));
        
        for (const auto& conta : contas) {
            conta->exibir();
            std::cout << "  - Total Calculado: R$" << conta->calcular_total() << "\n";
        }
        
        const Bill* maior = get_max_bill(contas);
        if (maior) {
            std::cout << "  - Maior valor: R$" << maior->calcular_total() << "\n";
        }
    }
    std::cout << "\n";

    // ==========================================
    // Questão 3 (D): Interface Pura por Referência
    // ==========================================
    std::cout << "[Q3] Interface Pura (IPayable):\n";
    {
        WaterBill wb(100.0f, 30.0f);
        pay_bill(wb);
    }
    std::cout << "\n";
    
    // ==========================================
    // Legado do TP1 (Composição / Agregação)
    // ==========================================
    std::cout << "[LEGADO] Classes basicas:\n";
    {
        auto pessoa = std::make_shared<Person>("Joao", 20);
        pessoa->celebrate_birthday();
        
        auto conta = std::make_unique<WaterBill>(100.0f, 10.0f);
        Home my_home(std::move(conta), pessoa);
        my_home.process_home();
    }
    
    std::cout << "\n--- Fim do Programa ---\n";
    return 0;
}
