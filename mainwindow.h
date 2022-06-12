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
    PredictionTrie trie;

private slots:
    void loadText();
    void changeText(const QString& text);
};
#endif // MAINWINDOW_H
