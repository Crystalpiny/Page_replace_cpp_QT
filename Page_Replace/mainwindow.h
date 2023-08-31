#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDateTime>
#include "global.h"
#include "settings.h"
#include "run.h"
#include "analyze.h"
#include<QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    settings *w1;
    run *w2;
    analyze *w3;


private slots:
    void on_pushButton_14_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_15_clicked();

public slots:
    void showrun();
    void showsettings();
    void showanalyze();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
