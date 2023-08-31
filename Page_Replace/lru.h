#ifndef LRU_H
#define LRU_H

#include <QObject>
#include"global.h"

class LRU : public QObject
{
    Q_OBJECT
public:
    explicit LRU(QObject *parent = nullptr);
    void start();
    int  algo();
    void writeQuickTable(int pageIndex);
    void writePageTable(int pageIndex);
    void updateLruTab(int pageIndex);
    void threadPause();
    void threadContinue();
    void stopTimer();
private:
    int runNum = 0;
    map<int,bool> m;  //判断当前页号是否在内存中
    map<int,bool> m1;  //判断当前页号是否在块表中
    map<int,list<int>::iterator> indexPageTab;  //页表索引
    map<int,list<int>::iterator> indexQuickTab;  //快表索引

    list<int> lruList;  //页表置换序列
    map<int,list<int>::iterator> indexLruTab;  //页表置换索引
    QTimer *timer;
    int index;
    int replaceIndex;
    bool isPause=false;
signals:
    void stop();
    void interrupt(int replaceIndex,int block,int index,int flag);

};

#endif // LRU_H
