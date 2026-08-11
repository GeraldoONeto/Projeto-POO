#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <numeric>
#include <map>
#include <unordered_set>
#include <future>
#include <mutex>
#include <cmath>
#include "domain.hpp"
#include "errors.hpp"
#include "repository.hpp"

int main() {
    std::cout << "=== Inicio do Trabalho Pratico 3 ===\n\n";

    // ==========================================
    // Questão 1 (A): Template registry<T>
    // Instanciado com dois tipos diferentes
    // ==========================================
    std::cout << "[Q1-A] Template registry<T> com dois tipos:\n";
    {
        // Instancia 1: registry<Person>
        registry<Person> pessoas;
        pessoas.add(Person(1, "Joao", 20));
        pessoas.add(Person(2, "Maria", 25));
        pessoas.add(Person(3, "Ana", 30));
        std::cout << "  Registry de pessoas: " << pessoas.size() << " itens\n";

        // Instancia 2: registry<WaterBill>
        registry<WaterBill> contas_agua;
        contas_agua.add(WaterBill(50.0f, 15.0f));
        contas_agua.add(WaterBill(80.0f, 20.0f));
        std::cout << "  Registry de contas de agua: " << contas_agua.size() << " itens\n";
    }
    std::cout << "\n";

    // ==========================================
    // Questão 1 (B): CRTP counted<Derived>
    // Contagem de instancias sem vtable
    // ==========================================
    std::cout << "[Q1-B] CRTP counted<Derived>:\n";
    {
        std::cout << "  Pessoas vivas antes: " << Person::alive() << "\n";
        Person p1(10, "Carlos", 40);
        Person p2(11, "Laura", 35);
        std::cout << "  Pessoas vivas durante: " << Person::alive() << "\n";

        std::cout << "  WaterBills vivas antes: " << WaterBill::alive() << "\n";
        WaterBill wb1(100.0f, 10.0f);
        std::cout << "  WaterBills vivas durante: " << WaterBill::alive() << "\n";
    }
    std::cout << "  Pessoas vivas apos escopo: " << Person::alive() << "\n";
    std::cout << "  WaterBills vivas apos escopo: " << WaterBill::alive() << "\n";
    std::cout << "\n";

    // ==========================================
    // Questão 1 (C)(D): Concept calculavel
    // Funcao soma_total restringida pelo concept
    // ==========================================
    std::cout << "[Q1-C/D] Concept calculavel + soma_total:\n";
    {
        std::vector<WaterBill> contas;
        contas.emplace_back(40.0f, 10.0f);   // total = 50
        contas.emplace_back(80.0f, 15.0f);   // total = 95
        contas.emplace_back(120.0f, 30.0f);  // total = 150

        double total = soma_total(contas);
        std::cout << "  Soma total de 3 WaterBills: R$" << total << "\n";

        std::vector<EnergyBill> energias;
        energias.emplace_back(60.0f, 10.0f);   // total = 70
        energias.emplace_back(100.0f, 20.0f);  // total = 120

        double total_energia = soma_total(energias);
        std::cout << "  Soma total de 2 EnergyBills: R$" << total_energia << "\n";

        // NOTA: Descomentar a linha abaixo gera erro CLARO de compilacao
        // citando o concept 'calculavel', pois int nao possui calcular_total():
        // soma_total(std::vector<int>{1, 2, 3}); // ERRO: int nao satisfaz calculavel
    }
    std::cout << "\n";

    // ==========================================
    // Questão 1 (E): Pipeline de ranges (C++20)
    // filter + transform encadeados
    // ==========================================
    std::cout << "[Q1-E] Pipeline de ranges:\n";
    {
        std::vector<WaterBill> contas;
        contas.emplace_back(30.0f, 5.0f);    // total = 35
        contas.emplace_back(80.0f, 20.0f);   // total = 100
        contas.emplace_back(150.0f, 50.0f);  // total = 200
        contas.emplace_back(40.0f, 10.0f);   // total = 50

        // Pipeline: filter (total > 60) | transform (extrai total)
        // Antes (laco manual):
        //   for (const auto& c : contas)
        //     if (c.calcular_total() > 60)
        //       std::cout << c.calcular_total();
        // Depois (ranges):
        exibir_contas_caras(contas, 60.0);
    }
    std::cout << "\n";

    // ==========================================
    // Questão 2 (A)(D): Hierarquia de exceções
    // try/catch capturando pela BASE
    // ==========================================
    std::cout << "[Q2-A] Hierarquia de excecoes:\n";
    {
        // Lanca invalid_bill, captura pela base domain_error
        try {
            validar_valor_conta(-50.0f);
        } catch (const domain_error& e) {
            std::cout << "  Excecao capturada pela base: " << e.what() << "\n";
        }

        // Lanca person_not_found, captura pela base domain_error
        try {
            validar_nome_pessoa("");
        } catch (const domain_error& e) {
            std::cout << "  Excecao capturada pela base: " << e.what() << "\n";
        }
    }
    std::cout << "\n";

    // ==========================================
    // Questão 2 (B)(D): std::optional — busca
    // Dois cenários: achou e não achou
    // ==========================================
    std::cout << "[Q2-B] std::optional (busca):\n";
    {
        std::vector<Person> pessoas;
        pessoas.emplace_back(1, "Joao", 20);
        pessoas.emplace_back(2, "Maria", 25);
        pessoas.emplace_back(3, "Ana", 30);

        // Caso 1: encontrou
        auto encontrado = find_by_name(pessoas, "Maria");
        if (encontrado.has_value()) {
            std::cout << "  Encontrou: " << encontrado->get_name()
                      << " (idade " << encontrado->get_age() << ")\n";
        }

        // Caso 2: nao encontrou
        auto nao_encontrado = find_by_name(pessoas, "Pedro");
        if (!nao_encontrado.has_value()) {
            std::cout << "  Pedro nao encontrado (nullopt)\n";
        }
    }
    std::cout << "\n";

    // ==========================================
    // Questão 2 (C)(D): std::variant com visit
    // Resultado sucesso ou erro
    // ==========================================
    std::cout << "[Q2-C] std::variant (resultado):\n";
    {
        // Caso sucesso: valor positivo
        auto resultado_ok = try_calculate(100.0f, 20.0f);
        std::cout << "  Resultado com valor positivo:\n";
        std::visit(result_visitor{}, resultado_ok);

        // Caso erro: valor negativo
        auto resultado_erro = try_calculate(-50.0f, 10.0f);
        std::cout << "  Resultado com valor negativo:\n";
        std::visit(result_visitor{}, resultado_erro);

        // Verificacao com holds_alternative
        if (std::holds_alternative<double>(resultado_ok)) {
            std::cout << "  holds_alternative<double>: true (sucesso)\n";
        }
        if (std::holds_alternative<std::string>(resultado_erro)) {
            std::cout << "  holds_alternative<string>: true (erro)\n";
        }
    }
    std::cout << "\n";

    // ==========================================
    // Questão 3 (A): Contêineres STL
    // map (indice ordenado) + unordered_set (unicidade O(1))
    // ==========================================
    std::cout << "[Q3-A] Conteineres STL:\n";
    {
        // std::map: indice de nomes de moradores por ID
        // Escolha: acesso ordenado por chave, ideal para exibir em ordem
        std::map<int, std::string> moradores;
        moradores[1] = "Joao";
        moradores[2] = "Maria";
        moradores[3] = "Ana";
        moradores[4] = "Carlos";
        std::cout << "  Moradores (map ordenado por ID):\n";
        for (const auto& [id, nome] : moradores) {
            std::cout << "    ID " << id << ": " << nome << "\n";
        }

        // std::unordered_set: categorias unicas de contas
        // Escolha: garante unicidade e acesso O(1) para verificar existencia
        std::unordered_set<std::string> categorias;
        categorias.insert("agua");
        categorias.insert("energia");
        categorias.insert("agua");       // duplicata ignorada
        categorias.insert("internet");
        std::cout << "  Categorias unicas (unordered_set): ";
        for (const auto& cat : categorias) {
            std::cout << cat << " ";
        }
        std::cout << "(total: " << categorias.size() << ")\n";
    }
    std::cout << "\n";

    // ==========================================
    // Questão 3 (B): Algoritmos STL + lambda
    // sort, count_if, accumulate com captura
    // ==========================================
    std::cout << "[Q3-B] Algoritmos STL + lambda:\n";
    {
        std::vector<WaterBill> contas;
        contas.emplace_back(120.0f, 30.0f);  // total = 150
        contas.emplace_back(40.0f, 10.0f);   // total = 50
        contas.emplace_back(80.0f, 20.0f);   // total = 100
        contas.emplace_back(200.0f, 50.0f);  // total = 250

        // sort: ordena contas por total (menor para maior)
        std::sort(contas.begin(), contas.end(),
            [](const WaterBill& a, const WaterBill& b) {
                return a.calcular_total() < b.calcular_total();
            });
        std::cout << "  Contas ordenadas por total:\n";
        for (const auto& c : contas) {
            std::cout << "    R$" << c.calcular_total() << "\n";
        }

        // count_if: conta quantas contas excedem um limite (lambda com captura)
        double limite = 100.0;
        auto n_caras = std::count_if(contas.begin(), contas.end(),
            [limite](const WaterBill& c) { return c.calcular_total() > limite; });
        std::cout << "  Contas com total > R$" << limite << ": " << n_caras << "\n";

        // accumulate: soma todos os totais
        double soma = std::accumulate(contas.begin(), contas.end(), 0.0,
            [](double acc, const WaterBill& c) { return acc + c.calcular_total(); });
        std::cout << "  Soma total de todas as contas: R$" << soma << "\n";
    }
    std::cout << "\n";

    // ==========================================
    // Questão 3 (C)(D): Concorrência
    // async + mutex para calculo paralelo
    //
    // Paralelizavel porque cada conta e independente:
    // calcular_total() e uma funcao const pura que
    // nao modifica estado compartilhado.
    // ==========================================
    std::cout << "[Q3-C/D] Concorrencia (async + mutex):\n";
    {
        std::vector<WaterBill> contas;
        contas.emplace_back(50.0f, 10.0f);    // total = 60
        contas.emplace_back(80.0f, 20.0f);    // total = 100
        contas.emplace_back(120.0f, 30.0f);   // total = 150
        contas.emplace_back(200.0f, 50.0f);   // total = 250

        // Calculo serial (referencia)
        double soma_serial = 0.0;
        for (const auto& c : contas) {
            soma_serial += c.calcular_total();
        }

        // Calculo paralelo com async
        std::mutex mtx;
        double soma_paralela = 0.0;
        std::vector<std::future<double>> futures;

        for (const auto& conta : contas) {
            // Cada future calcula o total de uma conta independentemente
            futures.push_back(std::async(std::launch::async,
                [&conta]() { return static_cast<double>(conta.calcular_total()); }));
        }

        // Coleta resultados e protege a soma com mutex
        for (auto& f : futures) {
            double parcial = f.get();
            std::lock_guard<std::mutex> lock{mtx};
            soma_paralela += parcial;
        }

        std::cout << "  Soma serial:   R$" << soma_serial << "\n";
        std::cout << "  Soma paralela: R$" << soma_paralela << "\n";
        std::cout << "  Resultados iguais: "
                  << (std::abs(soma_serial - soma_paralela) < 0.01 ? "sim" : "nao") << "\n";
    }
    std::cout << "\n";

    // ==========================================
    // Questão 4 (A)(B): Serialização JSON
    // Round-trip: salvar e carregar estado
    // ==========================================
    std::cout << "[Q4-A/B] Serializacao JSON round-trip:\n";
    {
        // Cria estado original
        home_state original;
        original.people.emplace_back(1, "Joao", 20);
        original.people.emplace_back(2, "Maria", 25);
        original.water_bills.emplace_back(100.0f, 20.0f);
        original.energy_bills.emplace_back(80.0f, 15.0f);

        // Salva em arquivo JSON
        json_repository json_repo("estado.json");
        json_repo.save(original);

        // Carrega do arquivo JSON
        auto carregado = json_repo.load();

        // Verifica round-trip
        std::cout << "  Round-trip igual: "
                  << (original == carregado ? "sim" : "nao") << "\n";
        std::cout << "  Pessoas carregadas: " << carregado.people.size() << "\n";
        std::cout << "  Contas agua carregadas: " << carregado.water_bills.size() << "\n";
        std::cout << "  Contas energia carregadas: " << carregado.energy_bills.size() << "\n";
    }
    std::cout << "\n";

    // ==========================================
    // Questão 4 (C)(D): DIP — Injeção de dependência
    // home_manager com json_repository e memory_repository
    // ==========================================
    std::cout << "[Q4-C/D] DIP (json_repository):\n";
    {
        json_repository json_repo("estado_manager.json");
        home_manager manager(json_repo);  // injecao de dependencia

        manager.add_person(Person(1, "Joao", 20));
        manager.add_water_bill(WaterBill(100.0f, 20.0f));
        manager.add_energy_bill(EnergyBill(60.0f, 10.0f));

        manager.save();
        manager.process();
    }
    std::cout << "\n";

    std::cout << "[Q4-C/D] DIP (memory_repository — sem disco):\n";
    {
        memory_repository mem_repo;          // implementacao de teste
        home_manager manager(mem_repo);      // mesma interface

        manager.add_person(Person(1, "Joao", 20));
        manager.add_person(Person(2, "Maria", 25));
        manager.add_water_bill(WaterBill(80.0f, 15.0f));

        manager.save();    // salva em memoria, sem arquivo
        manager.process(); // exercita a logica normalmente

        // Carrega de volta da memoria
        home_manager manager2(mem_repo);
        manager2.load();
        manager2.process(); // resultado identico
    }
    std::cout << "\n";

    // ==========================================
    // Legado TP1/TP2: Polimorfismo e composicao
    // ==========================================
    std::cout << "[LEGADO] Polimorfismo e composicao:\n";
    {
        // Destrutor virtual em cadeia
        Bill* base_ptr = new WaterBill(50.0f, 15.0f);
        std::cout << "  Total via ponteiro base: R$" << base_ptr->calcular_total() << "\n";
        delete base_ptr;

        // Vetor polimorfico
        std::vector<std::unique_ptr<Bill>> contas;
        contas.push_back(std::make_unique<WaterBill>(40.0f, 10.0f));
        contas.push_back(std::make_unique<EnergyBill>(80.0f, 15.0f));
        const Bill* maior = get_max_bill(contas);
        if (maior) {
            std::cout << "  Maior conta: R$" << maior->calcular_total() << "\n";
        }

        // Interface pura
        WaterBill wb(100.0f, 30.0f);
        pay_bill(wb);

        // Composicao e agregacao
        auto pessoa = std::make_shared<Person>(1, "Joao", 20);
        pessoa->celebrate_birthday();
        auto conta = std::make_unique<WaterBill>(100.0f, 10.0f);
        Home my_home(std::move(conta), pessoa);
        my_home.process_home();
    }

    std::cout << "\n=== Fim do Programa ===\n";
    return 0;
}
