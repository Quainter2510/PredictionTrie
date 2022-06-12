#include "mainwindow.h"
#include <QFileDialog>
#include <fstream>
#include <sstream>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->line, SIGNAL(textChanged()), SLOT(changeText()));
    connect(ui->btn, SIGNAL(clicked()), SLOT(loadText()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadText() {
    QString filePath = QFileDialog::getExistingDirectory(this, tr("Choose Directory"), "");
    std::string str, word;
    if (!filePath.toStdString().empty()) {
        std::ifstream file(filePath.toStdString());
        if (!file.is_open()) {
            throw std::runtime_error("Could not open the files ");
        }
        file >> str;
        while (!str.empty()) {

            for (char c : str) {
                if ('a' <= c && 'z' >= c) {
                    word += c;
                }
                if ('A' <= c && 'Z' >= c) {
                    word += c + 32;
                }
            }
            if (!word.empty()){
                trie.insert(word);
            }
            word.clear();
            file >> str;
        }
    }
}

void MainWindow::changeText(const QString& text) {
    std::vector<MatchedPair> q = trie.allWordsStartedWith(text.toStdString());
    if (q.empty()) {
        ui->table->setEnabled(false);
        ui->table->clear();
    }
    std::sort(q.begin(), q.end(), comp);
    ui->table->clear();
    ui->table->setRowCount(q.size());
    ui->table->setColumnCount(2);
    for (int i = 0; i < q.size(); i++) {
        ui->table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(std::get<0>(q[i]))));
        ui->table->setItem(i, 1, new QTableWidgetItem(std::get<1>(q[i])));
    }
}

