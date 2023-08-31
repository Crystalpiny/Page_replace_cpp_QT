#include "lfu.h"
#include<QDebug>

LFU::LFU(QObject *parent) : QObject(parent)
{
    lfu_interrupt=0;  	//中断次数
    lfu_totalTime=0;  	//总时间
    lfu_avgTime=0;   	//平均时间
    lfu_miss_page=0;	//缺页率
    lfu_quickTable.clear(); //快表
    lfu_pageTable.clear(); //页表

    index=0;
    m.clear();
    m1.clear();
    freqIndex.clear();
    keyIndex.clear();
    indexPageTab.clear();
    indexQuickTab.clear();
}
void LFU::start(){
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,[=](){
         if(!isPause){
            int curTime=algo();
            if(curTime==0){
                timer->stop(); //停止
                emit stop();
            }
            //计算当前缺页率
            lfu_miss_page=lfu_interrupt*1.0/index;
            //计算总时间
            lfu_totalTime+=curTime;
            //计算平均时间
            lfu_avgTime=lfu_totalTime*1.0/index;
         }
    });
    timer->start(run_time);
}
int LFU::algo(){
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
                updateFre(pageIndex);
                curTime=quiAcesTime+memAcesTime;   //访问快表时间+访问内存时间
            }
            //不在快表中
            else{
                //if(pageIndex==0) cout<<pageIndex<<" "<<m[pageIndex]<<" "<<m1[pageIndex]<<endl;
                updateFre(pageIndex);
                writeQuickTable(pageIndex);
                //访问快表的时间+两次内存时间+更新快表的时间 并把页面写入快表中
                curTime=quiAcesTime*2+memAcesTime*2;
            }
        }
        //缺页
        else{
            is_interrupt=true;
            //cout<<pageIndex<<endl;
            lfu_interrupt++;
            writePageTable(pageIndex);
            writeQuickTable(pageIndex);
            //更新最小索引
            Node v={pageIndex,1};
            freqIndex[1].push_back(v);
            minfreq=1;
            keyIndex[pageIndex]=prev(freqIndex[1].end());
            //访问快表时间+访问页表时间+中断+内存+快表
            curTime=missPageTime+memAcesTime*2+quiAcesTime*2;
        }
    }
    //不使用快表
    else{
        //不缺页
        if(m[pageIndex]){
            updateFre(pageIndex);
            //访问两次内存时间
            curTime=memAcesTime*2;
        }
        else{
            is_interrupt=true;
            lfu_interrupt++;
            writePageTable(pageIndex);
            //更新最小索引
            Node v={pageIndex,1};
            freqIndex[1].push_back(v);
            minfreq=1;
            keyIndex[pageIndex]=prev(freqIndex[1].end());
            //两次内存时间+中断时间
            curTime=missPageTime+memAcesTime*2;
        }
    }
    if(is_interrupt) emit interrupt(replaceIndex,pageIndex,index,3);
    else emit interrupt(replaceIndex,-1,index,3);
    return curTime;
}
//更新频率
void LFU::updateFre(int pageIndex){
    list<Node>::iterator it=keyIndex[pageIndex];
    int fre=it->fre;
    Node newNode={pageIndex,fre+1};
    freqIndex[fre].erase(it);  //在当前频率中删除此结点
    if(freqIndex[fre].size()==0 && fre==minfreq) minfreq++;
    freqIndex[fre+1].push_back(newNode);
    keyIndex[pageIndex]=prev(freqIndex[fre+1].end());
}
//写入页表
void LFU::writePageTable(int pageIndex){
    //分配给进程的内存块已满
    if(lfu_pageTable.size()>=memoryBlockNum){
        Node temp=freqIndex[minfreq].front();
        freqIndex[minfreq].pop_front();
        m[temp.pageIndex]=false;
        auto it=indexPageTab[temp.pageIndex];
        *it=pageIndex;
        m[pageIndex]=true; indexPageTab[pageIndex]=it;
        if(m1[temp.pageIndex]){
            list<int>::iterator it=indexQuickTab[temp.pageIndex];
            lfu_quickTable.erase(it);   //从快表中删除该页
            m1[temp.pageIndex]=false;
        }
        replaceIndex=temp.pageIndex;
     }
    else{
        m[pageIndex]=true;
        lfu_pageTable.push_back(pageIndex);
        indexPageTab[pageIndex]=prev(lfu_pageTable.end());
    }
}
//写入快表
void LFU::writeQuickTable(int pageIndex){
    if(lfu_quickTable.size()>=quickNum){
        int temp=lfu_quickTable.front();
        m1[temp]=false;
        lfu_quickTable.pop_front();
    }
    lfu_quickTable.push_back(pageIndex);
    m1[pageIndex]=true;
    indexQuickTab[pageIndex]=prev(lfu_quickTable.end());
}
void LFU::threadPause(){
    isPause=true;
}
void LFU::threadContinue(){
    isPause=false;
}
void LFU::stopTimer(){
    if(timer!=nullptr){
        timer->stop();
    }
   // qDebug()<<2;
}
