#include <iostream>
#include <string>
#include <vector>
#include <memory>

// ==========================================
// Classe 1: Person (Membro da Familia)
// ==========================================
class Person {
private:
    std::string name_; [span_1](start_span)// Identificador em inglês, snake_case e sufixo _[span_1](end_span)
    int age_;

public:
    [span_2](start_span)// Construtor com lista de inicialização[span_2](end_span)
    Person(std::string name, int age) : name_(name), age_(age) {
        std::cout << "Person(" << name_ << ") criada.\n";
    }

    [span_3](start_span)// Destrutor explícito observável[span_3](end_span)
    ~Person() {
        std::cout << "~Person(" << name_ << ") destruida.\n";
    }

    [span_4](start_span)// Getters const[span_4](end_span)
    std::string get_name() const { return name_; }

    [span_5](start_span)// Método com lógica real[span_5](end_span)
    bool is_adult() const { 
        return age_ >= 18; 
    }
};

// ==========================================
// Classe 2: Bill (Conta / Despesa)
// ==========================================
class Bill {
private:
    std::string name_;
    float value_;

public:
    Bill(std::string name, float value) : name_(name), value_(value) {
        std::cout << "Bill(" << name_ << ", R$" << value_ << ") criada.\n";
    }

    ~Bill() {
        std::cout << "~Bill(" << name_ << ") destruida.\n";
    }

    [span_6](start_span)// Método com lógica real: aplica um desconto na conta[span_6](end_span)
    void apply_discount(float percent) {
        if (percent > 0.0f && percent <= 100.0f) {
            value_ -= value_ * (percent / 100.0f);
            std::cout << "Desconto aplicado! Novo valor de " << name_ << ": R$" << value_ << "\n";
        }
    }

    float get_value() const { return value_; }
};

// ==========================================
// Classe 3: Task (Tarefa)
[span_7](start_span)// Demonstra Agregação com Person[span_7](end_span)
// ==========================================
class Task {
private:
    std::string description_;
    std::shared_ptr<Person> worker_; [span_8](start_span)[span_9](start_span)// shared_ptr para agregação: observa sem posse[span_8](end_span)[span_9](end_span)
    bool done_;

public:
    Task(std::string description, std::shared_ptr<Person> worker) 
        : description_(description), worker_(worker), done_(false) {
        std::cout << "Task(" << description_ << ") criada e atribuida a " << worker_->get_name() << ".\n";
    }

    ~Task() {
        std::cout << "~Task(" << description_ << ") destruida.\n";
        // O trabalhador (worker_) NÃO é destruído aqui. [span_10](start_span)Ele existe independentemente[span_10](end_span).
    }

    [span_11](start_span)// Método com lógica real[span_11](end_span)
    void mark_done() {
        if (!done_) {
            done_ = true;
            std::cout << "Tarefa '" << description_ << "' marcada como concluida por " << worker_->get_name() << ".\n";
        }
    }
};

// ==========================================
// Classe 4: House (Residencia)
[span_12](start_span)// Demonstra Composição com Bill e Task[span_12](end_span)
// ==========================================
class House {
private:
    std::string address_;
    std::vector<std::unique_ptr<Bill>> bills_; [span_13](start_span)[span_14](start_span)// unique_ptr para composição: posse exclusiva[span_13](end_span)[span_14](end_span)
    std::vector<std::unique_ptr<Task>> tasks_; [span_15](start_span)[span_16](start_span)// unique_ptr para composição: posse exclusiva[span_15](end_span)[span_16](end_span)

public:
    House(std::string address) : address_(address) {
        std::cout << "House(" << address_ << ") iniciada.\n";
    }

    ~House() {
        std::cout << "~House(" << address_ << ") destruida.\n";
        [span_17](start_span)// As contas e tarefas dependentes são destruídas automaticamente aqui[span_17](end_span).
    }

    void add_bill(std::string name, float value) {
        bills_.push_back(std::make_unique<Bill>(name, value));
    }

    void add_task(std::string description, std::shared_ptr<Person> worker) {
        tasks_.push_back(std::make_unique<Task>(description, worker));
    }

    [span_18](start_span)// Método com lógica real[span_18](end_span)
    float get_total_bills() const {
        float total = 0.0f;
        for (const auto& bill : bills_) {
            total += bill->get_value();
        }
        return total;
    }

    // Executa lógicas internas para demonstração
    void process_routines() {
        if (!bills_.empty()) bills_[0]->apply_discount(10.0f); // 10% de desconto na primeira conta
        if (!tasks_.empty()) tasks_[0]->mark_done();           // Conclui a primeira tarefa
    }
};

// ==========================================
[span_19](start_span)[span_20](start_span)[span_21](start_span)// MAIN: Testes e Saídas[span_19](end_span)[span_20](end_span)[span_21](end_span)
// ==========================================
int main() {
    std::cout << "--- CRIANDO OBJETOS INDEPENDENTES ---\n";
    [span_22](start_span)// Criados com shared_ptr para uso na agregação (Teste 6)[span_22](end_span)
    std::shared_ptr<Person> p1 = std::make_shared<Person>("Joao", 40);
    std::shared_ptr<Person> p2 = std::make_shared<Person>("Maria", 15);

    std::cout << "\nO " << p1->get_name() << " eh adulto? " << (p1->is_adult() ? "Sim" : "Nao") << "\n";

    std::cout << "\n--- ENTRANDO NO ESCOPO DA COMPOSICAO ---\n";
    {
        House my_house("Rua das Flores, 123"); [span_23](start_span)// Objeto dono criado (Teste 5)[span_23](end_span)

        my_house.add_bill("Agua", 100.0f);
        my_house.add_bill("Internet", 150.0f);
        
        my_house.add_task("Varrer a sala", p2); // Agrega a Maria à tarefa da casa

        std::cout << "\nTotal de contas original: R$" << my_house.get_total_bills() << "\n";
        
        my_house.process_routines(); [span_24](start_span)// Demonstra a lógica real sendo executada[span_24](end_span)

        std::cout << "Total de contas apos desconto: R$" << my_house.get_total_bills() << "\n";

        std::cout << "\n--- FIM DO ESCOPO DA COMPOSICAO ---\n";
    } 
    [span_25](start_span)[span_26](start_span)// my_house morre aqui e puxa as contas e tarefas junto[span_25](end_span)[span_26](end_span)

    std::cout << "\n--- FORA DO ESCOPO ---\n";
    std::cout << p1->get_name() << " e " << p2->get_name() << " continuam existindo (Agregacao!).\n"; [span_27](start_span)// Teste 6[span_27](end_span)

    std::cout << "\n--- FIM DO PROGRAMA ---\n";
    return 0;
}
