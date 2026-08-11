#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

// ==========================================
// Questão 6 — Interface Gráfica com Qt
//
// Camada fina: a janela NÃO contém regra de
// negócio. Ela apenas invoca home_manager
// (que recebe repository& por DIP, Q4).
// ==========================================

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QMessageBox>
#include <QStatusBar>
#include <QHeaderView>
#include <QFileDialog>
#include <QFont>

#include "repository.hpp"

// ==========================================
// main_window: janela principal (camada fina)
//
// Toda lógica de negócio fica em home_manager.
// A janela apenas:
//   - exibe dados do estado
//   - coleta inputs do usuario
//   - delega ao home_manager
// ==========================================
class main_window : public QMainWindow {
    Q_OBJECT

public:
    // Recebe home_manager por referencia (DIP preservado)
    explicit main_window(home_manager& manager, QWidget* parent = nullptr)
        : QMainWindow(parent), manager_(manager)
    {
        setWindowTitle("Sistema de Gestão Residencial — TP3");
        setMinimumSize(800, 600);

        auto* central = new QWidget(this);
        auto* layout = new QVBoxLayout(central);

        // --- Secao: Moradores ---
        layout->addWidget(create_people_group());

        // --- Secao: Contas ---
        layout->addWidget(create_bills_group());

        // --- Secao: Acoes ---
        layout->addWidget(create_actions_group());

        setCentralWidget(central);
        statusBar()->showMessage("Pronto");

        refresh_tables();
    }

private slots:
    // Adiciona um morador ao estado via home_manager
    void on_add_person() {
        int id = person_id_spin_->value();
        QString name = person_name_edit_->text().trimmed();
        int age = person_age_spin_->value();

        if (name.isEmpty()) {
            QMessageBox::warning(this, "Erro", "Nome nao pode ser vazio.");
            return;
        }

        manager_.add_person(Person(id, name.toStdString(), age));
        person_name_edit_->clear();
        refresh_tables();
        statusBar()->showMessage("Morador adicionado: " + name);
    }

    // Adiciona uma conta ao estado via home_manager
    void on_add_bill() {
        float value = static_cast<float>(bill_value_spin_->value());
        float taxa = static_cast<float>(bill_taxa_spin_->value());
        QString type = bill_type_combo_->currentText();

        if (type == "Agua") {
            manager_.add_water_bill(WaterBill(value, taxa));
        } else {
            manager_.add_energy_bill(EnergyBill(value, taxa));
        }

        refresh_tables();
        statusBar()->showMessage("Conta adicionada: " + type + " R$" + QString::number(value + taxa, 'f', 2));
    }

    // Salva estado via home_manager (serialização Q4)
    void on_save() {
        try {
            manager_.save();
            statusBar()->showMessage("Estado salvo com sucesso!");
            QMessageBox::information(this, "Salvo", "Estado salvo em JSON com sucesso.");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Erro ao Salvar", e.what());
        }
    }

    // Carrega estado via home_manager (serialização Q4)
    void on_load() {
        try {
            manager_.load();
            refresh_tables();
            statusBar()->showMessage("Estado carregado com sucesso!");
            QMessageBox::information(this, "Carregado", "Estado restaurado do JSON.");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Erro ao Carregar", e.what());
        }
    }

    // Processa (calcula totais) via home_manager
    void on_process() {
        const auto& state = manager_.state();
        double total = 0.0;
        for (const auto& b : state.water_bills)
            total += b.calcular_total();
        for (const auto& b : state.energy_bills)
            total += b.calcular_total();

        QString msg = QString("Moradores: %1\n"
                              "Contas de Agua: %2\n"
                              "Contas de Energia: %3\n"
                              "Total Geral: R$%4")
                          .arg(state.people.size())
                          .arg(state.water_bills.size())
                          .arg(state.energy_bills.size())
                          .arg(total, 0, 'f', 2);

        QMessageBox::information(this, "Resumo do Sistema", msg);
        statusBar()->showMessage("Processamento concluido — Total: R$" + QString::number(total, 'f', 2));
    }

private:
    home_manager& manager_;

    // Widgets de entrada — moradores
    QSpinBox* person_id_spin_;
    QLineEdit* person_name_edit_;
    QSpinBox* person_age_spin_;

    // Widgets de entrada — contas
    QDoubleSpinBox* bill_value_spin_;
    QDoubleSpinBox* bill_taxa_spin_;
    QComboBox* bill_type_combo_;

    // Tabelas de exibicao
    QTableWidget* people_table_;
    QTableWidget* bills_table_;

    // Cria o grupo de moradores (formulario + tabela)
    QGroupBox* create_people_group() {
        auto* group = new QGroupBox("Moradores");
        auto* layout = new QVBoxLayout(group);

        // Formulario de entrada
        auto* form = new QHBoxLayout();

        form->addWidget(new QLabel("ID:"));
        person_id_spin_ = new QSpinBox();
        person_id_spin_->setRange(1, 9999);
        form->addWidget(person_id_spin_);

        form->addWidget(new QLabel("Nome:"));
        person_name_edit_ = new QLineEdit();
        person_name_edit_->setPlaceholderText("Nome do morador");
        form->addWidget(person_name_edit_);

        form->addWidget(new QLabel("Idade:"));
        person_age_spin_ = new QSpinBox();
        person_age_spin_->setRange(0, 150);
        form->addWidget(person_age_spin_);

        auto* add_btn = new QPushButton("Adicionar");
        connect(add_btn, &QPushButton::clicked, this, &main_window::on_add_person);
        form->addWidget(add_btn);

        layout->addLayout(form);

        // Tabela
        people_table_ = new QTableWidget(0, 3);
        people_table_->setHorizontalHeaderLabels({"ID", "Nome", "Idade"});
        people_table_->horizontalHeader()->setStretchLastSection(true);
        people_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
        people_table_->setSelectionBehavior(QAbstractItemView::SelectRows);
        layout->addWidget(people_table_);

        return group;
    }

    // Cria o grupo de contas (formulario + tabela)
    QGroupBox* create_bills_group() {
        auto* group = new QGroupBox("Contas");
        auto* layout = new QVBoxLayout(group);

        // Formulario de entrada
        auto* form = new QHBoxLayout();

        form->addWidget(new QLabel("Tipo:"));
        bill_type_combo_ = new QComboBox();
        bill_type_combo_->addItems({"Agua", "Energia"});
        form->addWidget(bill_type_combo_);

        form->addWidget(new QLabel("Valor Base (R$):"));
        bill_value_spin_ = new QDoubleSpinBox();
        bill_value_spin_->setRange(0.0, 99999.99);
        bill_value_spin_->setDecimals(2);
        form->addWidget(bill_value_spin_);

        form->addWidget(new QLabel("Taxa (R$):"));
        bill_taxa_spin_ = new QDoubleSpinBox();
        bill_taxa_spin_->setRange(0.0, 99999.99);
        bill_taxa_spin_->setDecimals(2);
        form->addWidget(bill_taxa_spin_);

        auto* add_btn = new QPushButton("Adicionar");
        connect(add_btn, &QPushButton::clicked, this, &main_window::on_add_bill);
        form->addWidget(add_btn);

        layout->addLayout(form);

        // Tabela
        bills_table_ = new QTableWidget(0, 4);
        bills_table_->setHorizontalHeaderLabels({"Tipo", "Valor Base", "Taxa", "Total"});
        bills_table_->horizontalHeader()->setStretchLastSection(true);
        bills_table_->setEditTriggers(QAbstractItemView::NoEditTriggers);
        bills_table_->setSelectionBehavior(QAbstractItemView::SelectRows);
        layout->addWidget(bills_table_);

        return group;
    }

    // Cria o grupo de acoes (botoes Salvar/Carregar/Processar)
    QGroupBox* create_actions_group() {
        auto* group = new QGroupBox("Acoes");
        auto* layout = new QHBoxLayout(group);

        auto* save_btn = new QPushButton("Salvar (JSON)");
        connect(save_btn, &QPushButton::clicked, this, &main_window::on_save);
        layout->addWidget(save_btn);

        auto* load_btn = new QPushButton("Carregar (JSON)");
        connect(load_btn, &QPushButton::clicked, this, &main_window::on_load);
        layout->addWidget(load_btn);

        auto* process_btn = new QPushButton("Processar");
        connect(process_btn, &QPushButton::clicked, this, &main_window::on_process);
        layout->addWidget(process_btn);

        return group;
    }

    // Atualiza as tabelas com o estado atual do home_manager
    void refresh_tables() {
        const auto& state = manager_.state();

        // Tabela de moradores
        people_table_->setRowCount(static_cast<int>(state.people.size()));
        for (int i = 0; i < static_cast<int>(state.people.size()); ++i) {
            const auto& p = state.people[static_cast<std::size_t>(i)];
            people_table_->setItem(i, 0, new QTableWidgetItem(QString::number(p.id())));
            people_table_->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(p.get_name())));
            people_table_->setItem(i, 2, new QTableWidgetItem(QString::number(p.get_age())));
        }

        // Tabela de contas (agua + energia juntas)
        int total_bills = static_cast<int>(state.water_bills.size() + state.energy_bills.size());
        bills_table_->setRowCount(total_bills);
        int row = 0;

        for (const auto& b : state.water_bills) {
            float taxa = b.calcular_total() - b.get_value();
            bills_table_->setItem(row, 0, new QTableWidgetItem("Agua"));
            bills_table_->setItem(row, 1, new QTableWidgetItem(QString("R$%1").arg(b.get_value(), 0, 'f', 2)));
            bills_table_->setItem(row, 2, new QTableWidgetItem(QString("R$%1").arg(taxa, 0, 'f', 2)));
            bills_table_->setItem(row, 3, new QTableWidgetItem(QString("R$%1").arg(b.calcular_total(), 0, 'f', 2)));
            row++;
        }
        for (const auto& b : state.energy_bills) {
            float taxa = b.calcular_total() - b.get_value();
            bills_table_->setItem(row, 0, new QTableWidgetItem("Energia"));
            bills_table_->setItem(row, 1, new QTableWidgetItem(QString("R$%1").arg(b.get_value(), 0, 'f', 2)));
            bills_table_->setItem(row, 2, new QTableWidgetItem(QString("R$%1").arg(taxa, 0, 'f', 2)));
            bills_table_->setItem(row, 3, new QTableWidgetItem(QString("R$%1").arg(b.calcular_total(), 0, 'f', 2)));
            row++;
        }
    }
};

#endif // MAIN_WINDOW_HPP
