#include "lru.h"

LRU::LRU(QObject *parent) : QObject(parent)
{
    lru_interrupt=0;  	//中断次数
    lru_totalTime=0;  	//总时间
    lru_avgTime=0;   	//平均时间
    lru_miss_page=0;	//缺页率
    lru_quickTable.clear(); //快表
    lru_pageTable.clear(); //页表
   index=0;
   m.clear();
   m1.clear();
   indexPageTab.clear();
   indexQuickTab.clear();
   lruList.clear();
   indexLruTab.clear();
}
void LRU::start(){
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,[=](){
         if(!isPause){
            int curTime=algo();
            if(curTime==0) {
                timer->stop(); //停止
                emit stop();
            }
            //计算当前缺页率
            lru_miss_page=lru_interrupt*1.0/index;
            //计算总时间
            lru_totalTime+=curTime;
            //计算平均时间
            lru_avgTime=lru_totalTime*1.0/index;
         }
    });
    timer->start(run_time);
}

int LRU::algo(){
    //运行结束
    if(index>=pageNum){
       return 0;
    }
    bool is_interrupt=false;
    int curTime=0;  //当次运行时间
    replaceIndex=-1;
    int pageIndex=pageList[index++];  //获取页号
    //使用快表
    if(useQTabl){
        //不缺页
        if(m[pageIndex]){
            //在快表中
            if(m1[pageIndex]){
                //将该页表放到lru链表的最后
                updateLruTab(pageIndex);
                curTime=quiAcesTime+memAcesTime;   //访问快表时间+访问内存时间
            }
            //不在快表中
            else{
                writeQuickTable(pageIndex);
                updateLruTab(pageIndex);
                //访问快表的时间+两次内存时间+更新快表的时间 并把页面写入快表中
                curTime=quiAcesTime*2+memAcesTime*2;
            }
        }
        //缺页
        else{
            is_interrupt=true;
            lru_interrupt++;
            writePageTable(pageIndex);
            writeQuickTable(pageIndex);
            //写入lru表中
            lruList.push_back(pageIndex);
            indexLruTab[pageIndex]=--lruList.end();
            //访问快表时间+访问页表时间+中断+内存+快表
            curTime=missPageTime+memAcesTime*2+quiAcesTime*2;
        }
    }
    //不使用快表
    else{
        //不缺页
        if(m[pageIndex]){
            updateLruTab(pageIndex);
            //访问两次内存时间
            curTime=memAcesTime*2;
        }
        else{
            is_interrupt=true;
            lru_interrupt++;
            writePageTable(pageIndex);
            lruList.push_back(pageIndex);
            indexLruTab[pageIndex]=--lruList.end();
            //两次内存时间+中断时间
            curTime=missPageTime+memAcesTime*2;
        }
    }
    if(is_interrupt) emit interrupt(replaceIndex,pageIndex,index,2);
    else emit interrupt(replaceIndex,-1,index,2);
    return curTime;
}
//写入页表
void LRU::writePageTable(int pageIndex){
    //分配给进程的内存块已满
    if(lru_pageTable.size()>=memoryBlockNum){
        int temp=lruList.front();
        lruList.pop_front();
        //从页表中删除该页(使用替换去操作)
        m[temp]=false;
        list<int>::iterator it=indexPageTab[temp];
        *it=pageIndex;
        m[pageIndex]=true; indexPageTab[pageIndex]=it;
        //之前的页号在快表中
        if(m1[temp]){
            list<int>::iterator it=indexQuickTab[temp];
            lru_quickTable.erase(it);   //从快表中删除该页
            m1[temp]=false;
        }
        replaceIndex=temp;
     }
    else{
        m[pageIndex]=true;
        lru_pageTable.push_back(pageIndex);
        indexPageTab[pageIndex]=prev(lru_pageTable.end());
    }
}
//写入快表
void LRU::writeQuickTable(int pageIndex){
    if(lru_quickTable.size()>=quickNum){
        int temp=lru_quickTable.front();
        m1[temp]=false;
        lru_quickTable.pop_front();
    }
    lru_quickTable.push_back(pageIndex);
    m1[pageIndex]=true;
    indexQuickTab[pageIndex]=prev(lru_quickTable.end());
}
//更新lru表
void LRU::updateLruTab(int pageIndex){
     list<int> ::iterator it=indexLruTab[pageIndex];
     lruList.erase(it);
     lruList.push_back(pageIndex);
     indexLruTab[pageIndex]=prev(lruList.end());
}
void LRU::threadPause(){
    isPause=true;
}
void LRU::threadContinue(){
    isPause=false;
}
void LRU::stopTimer(){
    if(timer!=nullptr){
        timer->stop();
    }
  //  qDebug()<<3;
}
