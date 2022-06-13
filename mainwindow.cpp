#include "mainwindow.h"
#include <QFileDialog>
#include <fstream>
#include <sstream>
#include <iostream>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->line->setEnabled(false);
    connect(ui->line, SIGNAL(editingFinished()), SLOT(changeText()));
    connect(ui->btn, SIGNAL(clicked()), SLOT(loadText()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::loadText() {
    QString filePath = QFileDialog::getOpenFileName(this, "", "");
    std::string str, word;
    if (!filePath.toStdString().empty()) {
        std::ifstream file(filePath.toStdString());
        if (!file.is_open()) {
            error("Could not open the files");
            return;
        }
        while (file >> str) {
            for (char c : str) {
                if ('a' <= c && 'z' >= c) {
                    word += c;
                }
                if ('A' <= c && 'Z' >= c) {
                    word += c + 32;
                }
            }
            if (!word.empty()){
                _trie.insert(word);
            }

            word.clear();
        }
    }
    ui->line->setEnabled(true);
    changeText();
}

void MainWindow::error(const std::string& error) {
    ui->table->clear();
    ui->table->setColumnCount(1);
    ui->table->setRowCount(1);
    ui->table->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("ERROR")));
    QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(error));
    item->setTextAlignment(4);
    ui->table->setItem(0, 0, item);
    ui->table->item(0,0)->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
}

void MainWindow::changeText() {
    QString text = ui->line->text();
    std::vector<MatchedPair> q = _trie.allWordsStartedWith(text.toStdString());
    if (q.empty()) {
        error("words not found");
        return;
    }
    std::sort(q.begin(), q.end(), comp);
    ui->table->clear();
    ui->table->setRowCount(q.size());
    ui->table->setColumnCount(2);
    ui->table->setHorizontalHeaderItem(0, new QTableWidgetItem(QString("Word")));
    ui->table->setHorizontalHeaderItem(1, new QTableWidgetItem(QString("Count")));
    for (size_t i = 0; i < q.size(); i++) {
        ui->table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(std::get<0>(q[i]))));
        ui->table->setItem(i, 1, new QTableWidgetItem(QString::number(std::get<1>(q[i]))));

        ui->table->item(i,0)->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
        ui->table->item(i,1)->setFlags(Qt::ItemIsDragEnabled|Qt::ItemIsUserCheckable|Qt::ItemIsSelectable);
    }
    QHeaderView* header = ui->table->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
}

