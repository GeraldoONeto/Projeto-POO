#include <iostream>
#include <string>
#include <memory>

// ==========================================
// Classe 1: Person (Agregada)
// ==========================================
class Person {
private:
    std::string name_;
    int age_;

public:
    // Construtor com lista de inicialização
    Person(std::string name, int age) : name_(name), age_(age) { 
        std::cout << "Person criada.\n"; 
    }
    
    // Destrutor com efeito observável
    ~Person() { 
        std::cout << "Person destruida.\n"; 
    }
    
    // Getter const
    std::string get_name() const { return name_; }
    
    // Lógica real: atualiza o estado (idade)
    void celebrate_birthday() { 
        age_++;
        std::cout << name_ << " agora tem " << age_ << " anos.\n";
    }
};

// ==========================================
// Classe 2: Bill (Composta)
// ==========================================
class Bill {
private:
    float value_;

public:
    Bill(float value) : value_(value) { 
        std::cout << "Bill criada.\n"; 
    }
    
    ~Bill() { 
        std::cout << "Bill destruida.\n"; 
    }
    
    float get_value() const { return value_; }
    
    // Lógica real: calcula e altera valor
    void apply_discount(float discount) { 
        if (discount > 0 && discount < value_) {
            value_ -= discount; 
            std::cout << "Desconto aplicado. Novo valor: R$" << value_ << "\n"; 
        }
    }
};

// ==========================================
// Classe 3: Task (Composta, Agrega Person)
// ==========================================
class Task {
private:
    std::shared_ptr<Person> person_; // Agregação: referência sem posse
    bool is_done_;

public:
    Task(std::shared_ptr<Person> person) : person_(person), is_done_(false) { 
        std::cout << "Task criada.\n"; 
    }
    
    ~Task() { 
        std::cout << "Task destruida.\n"; 
    }
    
    // Lógica real: valida e atualiza estado
    void complete_task() { 
        if (!is_done_) {
            is_done_ = true; 
            std::cout << "Task completada por " << person_->get_name() << "!\n"; 
        }
    }
};

// ==========================================
// Classe 4: Home (Dona)
// ==========================================
class Home {
private:
    std::unique_ptr<Bill> bill_; // Composição: posse exclusiva
    std::unique_ptr<Task> task_; // Composição: posse exclusiva

public:
    Home(float bill_val, std::shared_ptr<Person> person) 
        : bill_(std::make_unique<Bill>(bill_val)), task_(std::make_unique<Task>(person)) {
        std::cout << "Home criada.\n";
    }
    
    ~Home() { 
        std::cout << "Home destruida.\n"; 
    }
    
    // Lógica real: orquestra métodos das dependências
    void process_home() { 
        bill_->apply_discount(10.0f);
        task_->complete_task();
    }
};

// ==========================================
// MAIN: Testes de Ciclo de Vida
// ==========================================
int main() {
    std::cout << "--- Inicio ---\n";
    
    // 1. Cria a pessoa de forma independente (Agregação)
    auto p1 = std::make_shared<Person>("Joao", 20);
    p1->celebrate_birthday(); // Executa lógica
    
    std::cout << "\n[ENTRANDO NO BLOCO DA CASA]\n";
    {
        // 2. Casa criada (Composição de Bill e Task internamente)
        Home my_home(100.0f, p1);
        my_home.process_home(); // Executa lógica
        
        std::cout << "[SAINDO DO BLOCO DA CASA]\n";
    } 
    // 3. A casa foi destruída aqui, levando Bill e Task (unique_ptr) junto.
    
    std::cout << "\n[FORA DO BLOCO]\n";
    // 4. A pessoa sobrevive, provando a agregação
    std::cout << p1->get_name() << " ainda existe!\n";
    
    std::cout << "--- Fim ---\n";
    return 0;
}
