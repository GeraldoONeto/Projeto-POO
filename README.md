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
        +adicionar_despesa(string descricao, string categoria, float valor) void
        +adicionar_tarefa(string descricao, int pontos, shared_ptr~MembroFamilia~ responsavel) void
        +calcular_despesas_pendentes() const float
        +exibir_relatorio() const void
    }
    
    class MembroFamilia {
        -string nome_
        -int idade_
        -int pontuacao_tarefas_
        +MembroFamilia(string nome, int idade)
        +~MembroFamilia()
        +adicionar_pontos(int pontos) void
        +obter_nome() const string
        +obter_pontuacao() const int
    }
    
    class Despesa {
        -string descricao_
        -string categoria_
        -float valor_
        -bool esta_paga_
        +Despesa(string descricao, string categoria, float valor)
        +~Despesa()
        +pagar_despesa() void
        +obter_valor() const float
        +esta_pendente() const bool
    }
    
    class Tarefa {
        -string descricao_
        -int recompensa_pontos_
        -shared_ptr~MembroFamilia~ responsavel_
        -bool esta_concluida_
        +Tarefa(string descricao, int pontos, shared_ptr~MembroFamilia~ responsavel)
        +~Tarefa()
        +concluir_tarefa() void
    }

    Residencia *-- Despesa : compõe (dono)
    Residencia *-- Tarefa : compõe (dono)
    Tarefa o-- MembroFamilia : agrega (referência)

