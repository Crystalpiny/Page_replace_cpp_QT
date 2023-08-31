#include "fifo.h"
#include<iostream>
using namespace std;

FIFO::FIFO(QObject *parent) : QObject(parent)
{
    fifo_interrupt=0;  	//中断次数
    fifo_totalTime=0;  	//总时间
    fifo_avgTime=0;   	//平均时间
    fifo_miss_page=0;	//缺页率
    fifo_quickTable.clear(); //快表
    fifo_pageTable.clear(); //页表

    index=0;
    m.clear();
    m1.clear();
    indexPageTab.clear();
    indexQuickTab.clear();
    q.clear();
}
void FIFO::start()
{
    timer=new QTimer(this);  //创建一个定时器
    connect(timer,&QTimer::timeout,[=](){
        if(!isPause){
            int curTime=algo();
            if(curTime==0) {//算法已经完成
                timer->stop(); //停止
                emit stop();
            }
            //计算当前缺页率
            fifo_miss_page=fifo_interrupt*1.0/index;
            //计算总时间
            fifo_totalTime+=curTime;
            //计算平均时间
            fifo_avgTime=fifo_totalTime*1.0/index;
        }
    });
    timer->start(run_time);
}
//返回当次运行时间
int FIFO::algo(){
    bool is_interrupt=false;
    replaceIndex=-1;
    //运行结束
    if(index>=pageNum){
       return 0;
    }
    int curTime=0;  //当次运行时间
    int pageIndex=pageList[index++];  //获取页号
    //使用快表
    if(useQTabl){
        //未缺页
        if(m[pageIndex]){
            //在快表中
            if(m1[pageIndex]){
                curTime=quiAcesTime+memAcesTime;   //访问快表时间+访问内存时间
            }
            else{
                //访问快表的时间+两次内存时间+更新快表的时间 并把页面写入快表中
                writeQuickTable(pageIndex);
                curTime=quiAcesTime*2+memAcesTime*2;
            }
        }
        //缺页
        else{
            is_interrupt=true;
            fifo_interrupt++;
            //产生缺页中断将其写入页表和快表
            writePageTable(pageIndex);
            writeQuickTable(pageIndex);
            curTime=missPageTime+memAcesTime*2+quiAcesTime*2;
        }
    }
    //未使用快表
    else{
        //未缺页
        if(m[pageIndex]){
            //访问两次内存时间
            curTime=memAcesTime*2;
        }
        //缺页
        else{
            is_interrupt=true;
            fifo_interrupt++;
            writePageTable(pageIndex);
            curTime=missPageTime+memAcesTime*2;
        }
    }
    //cout<<pageIndex<<" "<<&indexPageTab[pageIndex]<<" "<<*indexPageTab[pageIndex]<<" "<<endl;
    if(is_interrupt) emit interrupt(replaceIndex,pageIndex,index,1);
    else emit interrupt(replaceIndex,-1,index,1);
    return curTime;
}
//将页号写入页表中
void FIFO::writePageTable(int pageIndex){
    //分配给进程的内存块已满
    if(fifo_pageTable.size()>=memoryBlockNum){
        int temp=q.front();
        q.pop_front();
        q.push_back(pageIndex);
        //cout<<temp<<" ";
        auto it=indexPageTab[temp];
        m[temp]=false; m[pageIndex]=true;
        *it=pageIndex;
        //cout<<it<<endl;
        indexPageTab[pageIndex]=it;
        if(m1[temp]){
            auto it=indexQuickTab[temp];
            fifo_quickTable.erase(it);   //从快表中删除该页
            m1[temp]=false;
        }
        replaceIndex=temp;
     }
    else{
        q.push_back(pageIndex);
        fifo_pageTable.push_back(pageIndex);
        indexPageTab[pageIndex]=prev(fifo_pageTable.end());
        m[pageIndex]=true;
    }
}
//将页号写入快表中
void FIFO::writeQuickTable(int pageIndex){
    if(fifo_quickTable.size()>=quickNum){
        int temp=fifo_quickTable.front();
        m1[temp]=false;
        fifo_quickTable.pop_front();
    }
    fifo_quickTable.push_back(pageIndex);
    m1[pageIndex]=true;
    indexQuickTab[pageIndex]=prev(fifo_quickTable.end());
}
void FIFO::threadPause(){
    qDebug()<<1231232;
    isPause=true;
}
void FIFO::threadContinue(){
    isPause=false;
}
void FIFO::stopTimer(){
    if(timer!=nullptr){
        timer->stop();
    }
  //  qDebug()<<1;
}
