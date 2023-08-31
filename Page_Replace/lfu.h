#ifndef LFU_H
#define LFU_H

#include <QObject>
#include"global.h"

class LFU : public QObject
{
    Q_OBJECT
public:
    explicit LFU(QObject *parent = nullptr);
    void start();
    int  algo();
    void writeQuickTable(int pageIndex);
    void writePageTable(int pageIndex);
    void updateFre(int pageIndex);
    void threadPause();
    void threadContinue();
    void stopTimer();
private:
    struct Node{
      int pageIndex;
      int fre;
    };
    int minfreq = 1;
    int index = 0;
    int runNum = 0;
    map<int,bool> m;  //判断当前页号是否在内存中
    map<int,bool> m1;  //判断当前页号是否在快表中
    map<int,list<Node>> freqIndex;  //频率索引
    map<int,list<Node>::iterator> keyIndex;  //页号索引

    map<int,list<int>::iterator> indexPageTab;  //页表索引
    map<int,list<int>::iterator> indexQuickTab;  //快表索引
    QTimer *timer;
    int replaceIndex;
    bool isPause=false;
signals:
    void stop();
    void interrupt(int replaceIndex,int block,int index,int flag);
};

#endif // LFU_H
