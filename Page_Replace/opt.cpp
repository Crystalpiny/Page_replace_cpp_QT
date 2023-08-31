#include "opt.h"

OPT::OPT(QObject *parent) : QObject(parent)
{
    opt_interrupt=0;  	//中断次数
    opt_totalTime=0;  	//总时间
    opt_avgTime=0;   	//平均时间
    opt_miss_page=0;	//缺页率
    opt_quickTable.clear(); //快表
    opt_pageTable.clear(); //页表

    index=0;
    blockQueue.clear();
    m.clear();
    m1.clear();
    indexPageTab.clear();
    indexQuickTab.clear();
    //遍历一次页面序列
    for(int i=0;i<pageList.size();i++){
        int j=pageList[i];
        blockQueue[j].push_back(i);
    }
}
void OPT::start()
{
    timer=new QTimer(this);
    connect(timer,&QTimer::timeout,[=](){
         if(!isPause){
            int curTime=algo();
            if(curTime==0) {
                timer->stop(); //停止
                emit stop();
            }
            //计算当前缺页率
            opt_miss_page=opt_interrupt*1.0/index;;
            //计算总时间
            opt_totalTime+=curTime;
            //计算平均时间
            opt_avgTime=opt_totalTime*1.0/index;
         }
    });
    timer->start(run_time);
}
int OPT::algo(){
    //运行结束
    if(index>=pageNum){
       return 0;
    }
    int curTime=0;  //当次运行时间
    int pageIndex=pageList[index++];  //获取页号
    bool is_interrupt=false;
    replaceIndex=-1;
    //使用快表
    if(useQTabl){
        //不缺页
        if(m[pageIndex]){
            //在快表中
            if(m1[pageIndex]){
                curTime=quiAcesTime+memAcesTime;    //访问快表时间+访问内存时间
            }
            //不在快表中
            else{
                writeQuickTable(pageIndex);
                //访问快表的时间+两次内存时间+更新快表的时间 并把页面写入快表中
                curTime=quiAcesTime*2+memAcesTime*2;
            }
        }
        //缺页
        else{
            is_interrupt=true;
            opt_interrupt++;
            //写入页表
            writePageTable(pageIndex);
            //写入块表
            writeQuickTable(pageIndex);
            //访问快表时间+访问页表时间+中断+内存+快表
            curTime=missPageTime+memAcesTime*2+quiAcesTime*2;
        }
    }
    //不使用快表
    else{
        //不缺页
        if(m[pageIndex]){
            curTime=memAcesTime*2;
        }
        else{
            is_interrupt=true;
            opt_interrupt++;
            //写入页表
            writePageTable(pageIndex);
            //两次内存时间+中断时间
            curTime=missPageTime+memAcesTime*2;
        }
    }
    blockQueue[pageIndex].pop_front();  //更新最近页号
    if(is_interrupt) emit interrupt(replaceIndex,pageIndex,index,4);
    else emit interrupt(replaceIndex,-1,index,4);
    return curTime;
}
void OPT::writeQuickTable(int pageIndex){
    if(opt_quickTable.size()>=quickNum){
        int temp=opt_quickTable.front();
        m1[temp]=false;
        opt_quickTable.pop_front();
    }
    opt_quickTable.push_back(pageIndex);
    m1[pageIndex]=true;
    indexQuickTab[pageIndex]=prev(opt_quickTable.end());
}
void OPT::writePageTable(int pageIndex){
    //分配给进程的内存块已满
    if(opt_pageTable.size()>=memoryBlockNum){
        //置换算法
        int max=-1; int v=-1;
        for(list<int>::iterator it=opt_pageTable.begin();it!=opt_pageTable.end();it++){
            //cout<<*it<<" "<<blockQueue[*it].front()<<" "<<blockQueue[*it].empty()<<endl;
            if(blockQueue[*it].empty()){
                v=*it;
                break;
            }
            else{
                if(blockQueue[*it].front()>max){
                    max=blockQueue[*it].front();
                    v=*it;
                }
            }
        }
        //v即为被置换的内存块号
        list<int>::iterator it=indexPageTab[v];
        int temp=*it;
        *it=pageIndex;
        m[v]=false; m[pageIndex]=true;
        indexPageTab[pageIndex]=it;
        //cout<<v<<" "<<temp<<" "<<m1[temp]<<endl;
        //之前的页号在快表中
        if(m1[temp]){
            list<int>::iterator it=indexQuickTab[temp];
            opt_quickTable.erase(it);   //从快表中删除该页
            m1[temp]=false;
        }
        replaceIndex=v;
     }
    else{
        m[pageIndex]=true;
        opt_pageTable.push_back(pageIndex);
        indexPageTab[pageIndex]=prev(opt_pageTable.end());
    }
}
void OPT::threadPause(){
    isPause=true;
}
void OPT::threadContinue(){
    isPause=false;
}
void OPT::stopTimer(){
    if(timer!=nullptr){
        timer->stop();
    }
   // qDebug()<<4;
}
