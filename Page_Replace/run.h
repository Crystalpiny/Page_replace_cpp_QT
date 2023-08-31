#ifndef RUN_H
#define RUN_H

#include <QWidget>
#include "global.h"
#include "settings.h"
#include <QGridLayout>
#include <QTableWidget>
#include <QMessageBox>
#include <QThread>
#include <fifo.h>
#include <lru.h>
#include <lfu.h>
#include <opt.h>

QT_BEGIN_NAMESPACE
namespace Ui {class run;}
QT_END_NAMESPACE

class run : public QWidget
{
    Q_OBJECT

public:
    explicit run(QWidget *parent = nullptr);
    ~run();
    void setTableWidget(QTableWidget *tableW);
    void setThread();
    void setTable(int replaceIndex,int interrupt,int index,int flag);
    void fifoupdateParam(QString labelITR,QString labelTT,QString labelAT,QString labelMP);
    void lruupdateParam(QString labelITR,QString labelTT,QString labelAT,QString labelMP);
    void lfuupdateParam(QString labelITR,QString labelTT,QString labelAT,QString labelMP);
    void optupdateParam(QString labelITR,QString labelTT,QString labelAT,QString labelMP);
    void addEnd();

protected:
    void closeEvent(QCloseEvent*event);

private:
    Ui::run *ui;

    //线程操作
    QThread *thread1;
    QThread *thread2;
    QThread *thread3;
    QThread *thread4;
    FIFO *threadFifo;
    LRU *threadLru;
    LFU *threadLfu;
    OPT *threadOpt;

    int is_end;

signals:
    void showsettingsig();
    void showanalyzesig();

    void startThread();
    void stopThreadTimer();
    void startThreadOpt();


public slots:
    void on_pushButton_13_clicked();
    void on_pushButton_15_clicked();

    void showall();
    void showfifo();
    void showlru();
    void showlfu();
    void showopt();

    void on_stopall_clicked();
    void on_optstop_clicked();
    void on_optkeep_clicked();
    void on_fifostop_clicked();
    void on_fifokeep_clicked();
    void on_lfustop_clicked();
    void on_lfukeep_clicked();
    void on_lrustop_clicked();
    void on_lrukeep_clicked();
};

#endif // RUN_H
