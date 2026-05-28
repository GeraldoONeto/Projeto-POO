# Sistema-Orientacao-a-Objetos

**Nome:** Geraldo Nunes de Oliveira Neto,
**Matrícula:** 20250018735


# Ideia do projeto:
Sistema de organização de uma residência, gerenciando membros da família, compromissos, tarefas domésticas e despesas financeiras. O objetivo é permitir a atribuição de tarefas específicas a individuos da casa e rastrear os gastos gerais gerados, mantendp um histórico centralizado de responsabilidades e finanças.

## Diagrama UML

```mermaid
classDiagram
    class Household {
        -string name_
        -vector~unique_ptr~Expense~~ expenses_
        -vector~shared_ptr~Chore~~ chores_
        +Household(string name)
        +~Household()
        +add_expense(string description, float amount) void
        +add_chore(string description, shared_ptr~FamilyMember~ assignee) void
        +calculate_total_expenses() const float
    }
    
    class FamilyMember {
        -string name_
        -int age_
        +FamilyMember(string name, int age)
        +~FamilyMember()
        +get_name() const string
    }
    
    class Expense {
        -string description_
        -float amount_
        +Expense(string description, float amount)
        +~Expense()
        +get_amount() const float
    }
    
    class Chore {
        -string description_
        -shared_ptr~FamilyMember~ assignee_
        -bool is_completed_
        +Chore(string description, shared_ptr~FamilyMember~ assignee)
        +~Chore()
        +complete_chore() void
        +get_status() const string
    }

    Household *-- Expense : contém
    Household *-- Chore : contém
    Chore o-- FamilyMember : atribuída a