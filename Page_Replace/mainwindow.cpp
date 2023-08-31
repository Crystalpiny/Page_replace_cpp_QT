#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    w1=new settings();
    w2=new run();
    w3=new analyze();
    w3->init();
    connect(w1,&settings::showrunsig,this,&MainWindow::showrun);
//    connect(w1,&settings::showanalyzesig,this,&MainWindow::showanalyze);
    connect(w2,&run::showsettingsig,this,&MainWindow::showsettings);
    connect(w2,&run::showanalyzesig,this,&MainWindow::showanalyze);
    connect(w3,&analyze::showsettingsig,this,&MainWindow::showsettings);
    connect(w3,&analyze::showrunsig,this,&MainWindow::showrun);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_13_clicked()
{
    this->hide();
    w1->show();
}

void MainWindow::showrun()
{
    this->hide();
    switch(flags){
    case 1:w2->showopt();
    case 2:w2->showfifo();
    case 3:w2->showlfu();
    case 4:w2->showlru();
    case 5:w2->showall();

    }


    w2->show();
}

void MainWindow::showsettings()
{
    this->hide();
    w1->show();
}

void MainWindow::showanalyze()
{
    w3->del();
    this->hide();
    w3->init();
    w3->show();
}

void MainWindow::on_pushButton_14_clicked()
{
    this->hide();
    w2->show();
}


void MainWindow::on_pushButton_15_clicked()
{
    this->hide();
    w3->show();
}
