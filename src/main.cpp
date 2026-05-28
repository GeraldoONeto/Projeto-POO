#include <iostream>
#include <string>
#include <vector>
#include <memory>

// ==========================================
// Classe 1: Person (Agregada)
// ==========================================
class Person {
private:
    std::string name_;
    int age_;

public:
    Person(std::string name, int age) : name_(name), age_(age) {
        std::cout << "Person(" << name_ << ") criada.\n";
    }

    ~Person() {
        std::cout << "~Person(" << name_ << ") destruida.\n";
    }

    std::string get_name() const { return name_; }

    // Logica real: faz aniversario
    void have_birthday() {
        age_++;
        std::cout << name_ << " fez aniversario! Idade: " << age_ << "\n";
    }
};

// ==========================================
// Classe 2: Expense (Composta)
// ==========================================
class Expense {
private:
    float value_;

public:
    Expense(float value) : value_(value) {
        std::cout << "Expense(" << value_ << ") criada.\n";
    }

    ~Expense() {
        std::cout << "~Expense(" << value_ << ") destruida.\n";
    }

    float get_value() const { return value_; }

    // Logica real: aplica juros
    void apply_tax(float tax) {
        value_ += tax;
        std::cout << "Juros aplicados. Novo valor: " << value_ << "\n";
    }
};

// ==========================================
// Classe 3: Task (Composta, Agrega Person)
// ==========================================
class Task {
private:
    std::string name_;
    std::shared_ptr<Person> person_; // Agregacao (observa o ponteiro)
    bool done_;

public:
    Task(std::string name, std::shared_ptr<Person> person) 
        : name_(name), person_(person), done_(false) {
        std::cout << "Task(" << name_ << ") criada para " << person_->get_name() << ".\n";
    }

    ~Task() {
        std::cout << "~Task(" << name_ << ") destruida.\n";
    }

    // Logica real: conclui a tarefa
    void complete() {
        done_ = true;
        std::cout << "Task " << name_ << " concluida!\n";
    }
};

// ==========================================
// Classe 4: Home (Dona)
// ==========================================
class Home {
private:
    std::vector<std::unique_ptr<Expense>> expenses_; // Composicao (dona)
    std::vector<std::unique_ptr<Task>> tasks_;       // Composicao (dona)

public:
    Home() {
        std::cout << "Home criada.\n";
    }

    ~Home() {
        std::cout << "~Home destruida.\n";
    }

    void add_expense(float value) {
        expenses_.push_back(std::make_unique<Expense>(value));
    }

    void add_task(std::string name, std::shared_ptr<Person> person) {
        tasks_.push_back(std::make_unique<Task>(name, person));
    }

    // Logica real: soma tudo
    float get_total() const {
        float total = 0.0f;
        for (const auto& e : expenses_) {
            total += e->get_value();
        }
        return total;
    }
};

// ==========================================
// Main: Testes do Roteiro
// ==========================================
int main() {
    // 1. Cria objeto independente para agregação
    std::shared_ptr<Person> p1 = std::make_shared<Person>("Ana", 20);
    p1->have_birthday(); 

    // 2. Abre bloco para testar a vida util da Composição
    std::cout << "\n[ENTRANDO NA CASA]\n";
    {
        Home my_home;
        
        my_home.add_expense(100.0f);
        my_home.add_task("Lavar louca", p1);
        
        std::cout << "Soma de gastos: " << my_home.get_total() << "\n";
        
        std::cout << "\n[SAINDO DA CASA]\n";
    } 
    // my_home é destruída e puxa as Tasks e Expenses com ela

    std::cout << "\n[FORA DO BLOCO]\n";
    std::cout << p1->get_name() << " ainda existe perfeitamente!\n";
    
    return 0;
}
