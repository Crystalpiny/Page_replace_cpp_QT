#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMessageBox>
#include <QDateTime>
#include "global.h"

QT_BEGIN_NAMESPACE
namespace Ui {class settings;}
QT_END_NAMESPACE

class settings : public QWidget
{
    Q_OBJECT

public:
    explicit settings(QWidget *parent = nullptr);
    ~settings();
    bool allIsEdited();

signals:
    void showrunsig();
//    void showanalyzesig();

//    void showQfifosig();
//    void showQlrusig();
//    void showQlfusig();
//    void showQoptsig();

//    void showfifosig();
//    void showlrusig();
//    void showlfusig();
//    void showoptsig();

    void showallsig();
    void showallQsig();


private slots:
//    void on_pushButton_14_clicked();
//    void on_pushButton_15_clicked();

    void on_randPagenum_clicked();

    void on_randPagelist_clicked();

    void on_HandInput_clicked();

    void on_useQTable_stateChanged(int arg1);

    void on_Saveparams_clicked();

    void on_OPT_clicked();

    void on_FIFO_clicked();

    void on_LRU_clicked();

    void on_LFU_clicked();

    void on_RunALL_clicked();

    void on_resDefault_clicked();

private:
    Ui::settings *ui;
};

#endif // SETTINGS_H
