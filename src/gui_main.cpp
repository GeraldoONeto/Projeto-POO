// ==========================================
// Questão 6 — Ponto de entrada da GUI Qt
//
// A janela é camada fina sobre home_manager.
// A mesma lógica testável sem GUI (Q4/Q5)
// funciona aqui, com repository injetado.
// ==========================================

#include <QApplication>
#include "main_window.hpp"

int main(int argc, char** argv) {
    QApplication qt{argc, argv};

    // Cria repositorio de producao (mesmo DIP da Q4)
    json_repository repo("estado_gui.json");

    // Injeta no manager (sem regra de negocio aqui)
    home_manager manager(repo);

    // Cria a janela — camada fina sobre a logica
    main_window window(manager);
    window.show();

    return qt.exec();
}
