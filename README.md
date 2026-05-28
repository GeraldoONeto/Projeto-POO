# Sistema-Orientacao-a-Objetos

**Nome:** Geraldo Nunes de Oliveira Neto,
**Matrícula:** 20250018735


# Ideia do projeto:
Sistema de organização de uma residência, gerenciando membros da família, compromissos, tarefas domésticas e despesas financeiras. O objetivo é permitir a atribuição de tarefas específicas a individuos da casa e rastrear os gastos gerais gerados, mantendp um histórico centralizado de responsabilidades e finanças.

## Diagrama UML

```mermaid
classDiagram
    class Residencia {
        -string nome_
        -vector~unique_ptr~Despesa~~ despesas_
        -vector~unique_ptr~Tarefa~~ tarefas_
        +Residencia(string nome)
        +~Residencia()
        +adicionar_despesa(string descricao, float valor) void
        +adicionar_tarefa(string descricao, shared_ptr~MembroFamilia~ responsavel) void
        +calcular_total_despesas() const float
    }
    
    class MembroFamilia {
        -string nome_
        -int idade_
        +MembroFamilia(string nome, int idade)
        +~MembroFamilia()
        +obter_nome() const string
    }
    
    class Despesa {
        -string descricao_
        -float valor_
        +Despesa(string descricao, float valor)
        +~Despesa()
        +obter_valor() const float
    }
    
    class Tarefa {
        -string descricao_
        -shared_ptr~MembroFamilia~ responsavel_
        -bool esta_concluida_
        +Tarefa(string descricao, shared_ptr~MembroFamilia~ responsavel)
        +~Tarefa()
        +concluir_tarefa() void
        +obter_status() const string
    }

    Residencia *-- Despesa : contém (Composição)
    Residencia *-- Tarefa : contém (Composição)
    Tarefa o-- MembroFamilia : atribuída a (Agregação)
