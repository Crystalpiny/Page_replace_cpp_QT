#ifndef RUNPART_H
#define RUNPART_H

#include <QWidget>
#include <opt.h>
#include "settings.h"
#include <QGridLayout>
#include <QTableWidget>
#include <QMessageBox>
#include <QThread>
#include <fifo.h>
#include <lru.h>
#include <lfu.h>
namespace Ui {
class runpart;
}

class runpart : public QWidget
{
    Q_OBJECT

public:
    explicit runpart(QWidget *parent = nullptr);
    ~runpart();
    void setTableWidget(QTableWidget *tableW);
    void setThread(int sig);
    void setTable(int replaceIndex,int interrupt,int index);
    void updateParam(QString labelITR,QString labelTT,QString labelAT,QString labelMP);

    void addEnd();

protected:
    void closeEvent(QCloseEvent*event);

private:

    //线程操作
    QThread *thread;

    FIFO *threadFifo;
    LRU *threadLru;
    LFU *threadLfu;
    OPT *threadOpt;

    int is_end;

signals:
//    void showsettingsig();
//    void showanalyzesig();

    void startThread();
    void stopThreadTimer();


public slots:

    void show(int sig);
    void on_stop_clicked();
    void on_keep_clicked();
private:
    Ui::runpart *ui;
};

#endif // RUNPART_H
