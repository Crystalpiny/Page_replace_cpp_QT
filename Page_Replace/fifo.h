#ifndef FIFO_H
#define FIFO_H

#include <QObject>
#include<map>
#include "global.h"
#include<QTimer>

namespace Ui {
class FIFO;
}

class FIFO : public QObject
{
    Q_OBJECT
public:
    explicit FIFO(QObject *parent = nullptr);
    void start();
    int algo();
    void writeQuickTable(int pageIndex);
    void writePageTable(int pageIndex);
    void threadPause();
    void threadContinue();
    void stopTimer();
private:
    map<int,bool> m;  //判断当前页号是否在内存中
    map<int,bool> m1;  //判断当前页号是否在快表中
    list<int> q;    //记录入块顺序
    map<int,list<int>::iterator> indexPageTab;  //页表索引
    map<int,list<int>::iterator> indexQuickTab;  //快表索引
    int index;   //运行索引
    QTimer *timer;
    int replaceIndex;
    bool isPause=false;
signals:
    void stop();
    void interrupt(int replaceIndex,int block,int index,int flag);
};

#endif // FIFO_H
