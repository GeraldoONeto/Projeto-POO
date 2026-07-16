# Sistema-Orientacao-a-Objetos

**Nome:** Geraldo Nunes de Oliveira Neto,
**Matrícula:** 20250018735


# Ideia do projeto:
Sistema de organização de uma residência, gerenciando membros da família, compromissos, tarefas domésticas e despesas financeiras. O objetivo é permitir a atribuição de tarefas específicas a individuos da casa e rastrear os gastos gerais gerados, mantendp um histórico centralizado de responsabilidades e finanças.

## Diagrama UML

```mermaid
classDiagram
    class Home {
        -unique_ptr~Bill~ bill_
        -unique_ptr~Task~ task_
        +Home(float bill_val, shared_ptr~Person~ person)
        +process_home() void
    }
    
    class Person {
        -string name_
        -int age_
        +celebrate_birthday() void
    }
    
    class Bill {
        <<abstract>>
        #float value_
        +calcular_total() float*
        +exibir() void
        +apply_discount(float discount) void
    }
    
    class IPayable {
        <<interface>>
        +process_payment() void*
    }
    
    class WaterBill {
        -float taxa_esgoto_
        +calcular_total() float
        +exibir() void
        +process_payment() void
    }
    
    class EnergyBill {
        -float taxa_iluminacao_
        +calcular_total() float
        +exibir() void
    }

    class Task {
        -shared_ptr~Person~ person_
        -bool is_done_
        +complete_task() void
    }

    Home *-- Bill : compõe
    Home *-- Task : compõe
    Task o-- Person : agrega
    Bill <|-- WaterBill : herda
    Bill <|-- EnergyBill : herda
    IPayable <|.. WaterBill : implementa
```

## Herança Avançada

A classe `EnergyBill` foi marcada como `final` (no nível de classe) porque ela representa uma entidade de negócio completa e folha na nossa árvore de domínio. Essa garantia de design assegura que ninguém no futuro crie uma subclasse de `EnergyBill` para injetar comportamentos indesejados (como substituir a lógica do `calcular_total()` sem controle) ou adicionar taxas extras que fujam da estrutura rígida dessa conta. Isso previne bugs arquiteturais onde a lógica dessa folha poderia ser acidentalmente corrompida por polimorfismo excessivo.
