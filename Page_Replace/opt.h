#ifndef OPT_H
#define OPT_H

#include <QObject>
#include"global.h"
class OPT : public QObject
{
    Q_OBJECT
public:
    explicit OPT(QObject *parent = nullptr);
    void start();
    int  algo();
    void writeQuickTable(int pageIndex);
    void writePageTable(int pageIndex);
    void updateLruTab(int pageIndex);
    void threadPause();
    void threadContinue();
    void stopTimer();
private:
    int index=0;
    map<int,list<int>> blockQueue; //内存块队列
    map<int,bool> m;  //判断当前页号是否在内存中
    map<int,bool> m1;  //判断当前页号是否在快表中

    map<int,list<int>::iterator> indexPageTab;  //页表索引
    map<int,list<int>::iterator> indexQuickTab;  //快表索引

    QTimer *timer;
    int replaceIndex;
    bool isPause=false;
signals:
    void stop();
    void interrupt(int replaceIndex,int block,int index,int flag);
};

#endif // OPT_H
