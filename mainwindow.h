#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "predictiontrie.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    PredictionTrie _trie;
    void error(const std::string& error);

private slots:
    void loadText();
    void changeText();
};
#endif // MAINWINDOW_H
