#ifndef DOMAIN_HPP
#define DOMAIN_HPP

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "registry.hpp"

// ==========================================
// Interface Pura: IPayable (Q3)
// ==========================================
class IPayable {
public:
    virtual void process_payment() const = 0;
    virtual ~IPayable() = default;
};

// ==========================================
// Classe 1: Person (Agregada)
// ==========================================
class Person : public counted<Person> {
private:
    int id_;
    std::string name_;
    int age_;

public:
    Person() : id_(0), name_(""), age_(0) {}
    Person(int id, std::string name, int age) : id_(id), name_(std::move(name)), age_(age) {}
    ~Person() = default;
    
    int id() const { return id_; }
    std::string get_name() const { return name_; }
    int get_age() const { return age_; }
    
    void celebrate_birthday() { 
        age_++;
        std::cout << name_ << " agora tem " << age_ << " anos.\n";
    }

    // Operador de igualdade para comparacao (necessario para testes)
    bool operator==(const Person& other) const {
        return id_ == other.id_ && name_ == other.name_ && age_ == other.age_;
    }
};

// ==========================================
// Base Abstrata: Bill (Q1)
// ==========================================
class Bill {
protected:
    float value_;

public:
    Bill() : value_(0.0f) {}
    Bill(float value) : value_(value) {}
    virtual ~Bill() = default;
    
    float get_value() const { return value_; }
    virtual float calcular_total() const = 0;
    
    virtual void exibir() const {
        std::cout << "Valor Base: R$" << value_ << "\n";
    }
    
    void apply_discount(float discount) { 
        if (discount > 0 && discount < value_) {
            value_ -= discount; 
            std::cout << "Desconto aplicado. Novo valor base: R$" << value_ << "\n"; 
        }
    }
};

// ==========================================
// Derivada Concreta 1: WaterBill (Q1, Q3)
// ==========================================
class WaterBill : public Bill, public IPayable, public counted<WaterBill> {
private:
    float taxa_esgoto_;
public:
    WaterBill() : Bill(0.0f), taxa_esgoto_(0.0f) {}
    WaterBill(float value, float taxa) : Bill(value), taxa_esgoto_(taxa) {}
    ~WaterBill() override = default;
    
    float calcular_total() const override { return get_value() + taxa_esgoto_; }
    
    void exibir() const override {
        Bill::exibir();
        std::cout << "+ Taxa de Esgoto: R$" << taxa_esgoto_ << " (Agua total: R$" << calcular_total() << ")\n";
    }
    
    void process_payment() const override {
        std::cout << "Pagando WaterBill de R$" << calcular_total() << "\n";
    }
};

// ==========================================
// Derivada Concreta 2: EnergyBill (Q1, Q3)
// ==========================================
class EnergyBill final : public Bill {
private:
    float taxa_iluminacao_;
public:
    EnergyBill() : Bill(0.0f), taxa_iluminacao_(0.0f) {}
    EnergyBill(float value, float taxa) : Bill(value), taxa_iluminacao_(taxa) {}
    ~EnergyBill() override = default;
    
    float calcular_total() const override { return get_value() + taxa_iluminacao_; }
    
    void exibir() const override {
        std::cout << "Conta de Energia total: R$" << calcular_total() << " (Base: R$" << get_value() << ")\n";
    }
};

// ==========================================
// Classe 3: Task (Composta, Agrega Person)
// ==========================================
class Task {
private:
    std::shared_ptr<Person> person_;
    bool is_done_;

public:
    Task(std::shared_ptr<Person> person) : person_(person), is_done_(false) {}
    ~Task() = default;
    
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
    std::unique_ptr<Bill> bill_; 
    std::unique_ptr<Task> task_; 

public:
    Home(std::unique_ptr<Bill> bill, std::shared_ptr<Person> person) 
        : bill_(std::move(bill)), task_(std::make_unique<Task>(person)) {}
    ~Home() = default;
    
    void process_home() { 
        bill_->apply_discount(10.0f);
        bill_->exibir();
        task_->complete_task();
    }
};

// ==========================================
// Funções Livres
// ==========================================
inline const Bill* get_max_bill(const std::vector<std::unique_ptr<Bill>>& contas) {
    if (contas.empty()) return nullptr;
    const Bill* max_bill = contas[0].get();
    for (const auto& conta : contas) {
        if (conta->calcular_total() > max_bill->calcular_total()) {
            max_bill = conta.get();
        }
    }
    return max_bill;
}

inline void pay_bill(const IPayable& payable) {
    payable.process_payment();
}

#endif // DOMAIN_HPP
