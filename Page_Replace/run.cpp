#include "run.h"
#include "ui_run.h"

#include <QWidget>

run::run(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::run)
{
    ui->setupUi(this);
//    connect(w1,&settings::showfifosig,this,&run::showfifo);
//    connect(w1,&settings::showlrusig,this,&run::showlru);
//    connect(w1,&settings::showlfusig,this,&run::showlfu);
//    connect(w1,&settings::showoptsig,this,&run::showopt);

//    connect(w1,&settings::showallsig,this,&run::showall);
//    connect(w1,&settings::showallQsig,this,&run::showallQ);
//    connect(w1,&settings::showallsig,this,&run::showlfu);
//    connect(w1,&settings::showallsig,this,&run::showopt);

}

void run::closeEvent(QCloseEvent*event){
    delete fifoTable;
    delete lruTable ;
    delete lfuTable ;
    delete optTable ;
    delete fifoTLB;
    delete lruTLB ;
    delete lfuTLB ;
    delete optTLB ;
}

run::~run()
{
    delete ui;
}

void run::on_pushButton_13_clicked()
{
    this->hide();
    emit showsettingsig();
}

void run::on_pushButton_15_clicked()
{
    this->hide();
    emit showanalyzesig();
}

void run::showopt(){
    fifoTable=ui->fifo->findChild<QTableWidget*>("fifoTable");
    lruTable =ui->lru->findChild<QTableWidget*>("lruTable");
    lfuTable =ui->lfu->findChild<QTableWidget*>("lfuTable");
    optTable =ui->opt->findChild<QTableWidget*>("optTable");

    fifoTLB=ui->fifo->findChild<QListWidget*>("fifoTLB");
    lruTLB =ui->lru->findChild<QListWidget*>("lruTLB");
    lfuTLB =ui->lfu->findChild<QListWidget*>("lfuTLB");
    optTLB =ui->opt->findChild<QListWidget*>("optTLB");

    //清空所有单元格内容
    fifoTable->clear();
    lruTable->clear();
    lfuTable->clear();
    optTable->clear();

    fifoTLB->clear();
    lruTLB ->clear();
    lfuTLB ->clear();
    optTLB ->clear();

    //铺设表格
    setTableWidget(optTable);

    is_end=0;

    //开始运行数据
    thread4=new QThread(this);
    threadOpt =new OPT();
    threadOpt->moveToThread(thread4);
    connect(this,&run::startThreadOpt,threadFifo,&FIFO::start);
    connect(threadOpt,&OPT::interrupt,this,&run::setTable,Qt::QueuedConnection);
    connect(ui->optstop,&QPushButton::clicked,threadOpt,&OPT::threadPause);
    connect(ui->optkeep,&QPushButton::clicked,threadOpt,&OPT::threadContinue);
    //启动线程

    thread4->start();
    emit startThreadOpt();

    ui->optstop->setEnabled(true);

    ui->optkeep->setEnabled(false);

}
void run::showfifo(){

}
void run::showlru(){

}
void run::showlfu(){


}

/*
 *    设置表格
 */
void run::setTableWidget(QTableWidget *tableW)
{
     tableW->clear();
     tableW->clearSpans();
    // qDebug()<<mem;

     tableW->setColumnCount(memoryBlockNum+2);
     tableW->setRowCount(pageNum+2);

     for(int i=2;i<tableW->rowCount();i++)
     {
         for(int j=0;j<tableW->columnCount();j++)
         {
             tableW->setItem(i,j, new QTableWidgetItem(""));
         }
     }

     //从0行一列开始，跨越1行memoryBlockNum列
     tableW->setSpan(0,1,1,memoryBlockNum);//从第0行第1列开始的单元格与随后的memoryBlockNum个单元格一起合并为一个单元格
     tableW->setSpan(0,memoryBlockNum+1,2,1);//从第0行第memoryBlockNum+1列开始的单元格与其下面的单元格合并为一个单元格，并跨越两行。
     tableW->setSpan(0,0,2,1);

     tableW->setItem(0,1, new QTableWidgetItem("页表内容"));
     tableW->setItem(0,memoryBlockNum+1,new QTableWidgetItem("缺页置换"));
     tableW->setItem(0,0, new QTableWidgetItem("访问页面序列"));

     tableW->item(0,1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
     tableW->item(0,memoryBlockNum+1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
     tableW->item(0,0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

     for(int i=1;i<=memoryBlockNum;i++)
     {
        //qDebug()<<i;
         tableW->setItem(1,i, new QTableWidgetItem(QString::number(i)));
         tableW->item(1,i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        // qDebug()<<tableW->item(1,i)->text();
     }

     //qDebug()<<pageNum;
     for(int i=2;i<=pageNum+1;i++)
     {

         if(i!=pageNum+2)
         {
             tableW->setItem(i,0, new QTableWidgetItem(QString::number(pageList.at(i-2))));
             tableW->item(i,0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
         }
         else
         {
             tableW->setItem(i,0, new QTableWidgetItem(QString::number(pageList.at(i-3))));
             tableW->item(i,0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
         }
     }
     //qDebug()<<tableW->itemAt(1,3)->text();
}

void run::showall(){
    fifoTable=ui->fifo->findChild<QTableWidget*>("fifoTable");
    lruTable =ui->lru->findChild<QTableWidget*>("lruTable");
    lfuTable =ui->lfu->findChild<QTableWidget*>("lfuTable");
    optTable =ui->opt->findChild<QTableWidget*>("optTable");

    fifoTLB=ui->fifo->findChild<QListWidget*>("fifoTLB");
    lruTLB =ui->lru->findChild<QListWidget*>("lruTLB");
    lfuTLB =ui->lfu->findChild<QListWidget*>("lfuTLB");
    optTLB =ui->opt->findChild<QListWidget*>("optTLB");

    //清空所有单元格内容
    fifoTable->clear();
    lruTable->clear();
    lfuTable->clear();
    optTable->clear();

    fifoTLB->clear();
    lruTLB ->clear();
    lfuTLB ->clear();
    optTLB ->clear();

    //铺设表格
    setTableWidget(fifoTable);
    setTableWidget(lruTable);
    setTableWidget(lfuTable);
    setTableWidget(optTable);

    is_end=0;

    //开始运行数据
    setThread();
    //启动线程
    thread1->start();
    thread2->start();
    thread3->start();
    thread4->start();
    emit startThread();

    ui->fifostop->setEnabled(true);
    ui->lrustop->setEnabled(true);
    ui->lfustop->setEnabled(true);
    ui->optstop->setEnabled(true);


    ui->fifokeep->setEnabled(false);
    ui->lrukeep->setEnabled(false);
    ui->lfukeep->setEnabled(false);
    ui->optkeep->setEnabled(false);

    ui->stopall->setEnabled(true);

}

void run::setThread(){
    thread1=new QThread(this);
    thread2=new QThread(this);
    thread3=new QThread(this);
    thread4=new QThread(this);
    threadFifo=new FIFO();
    threadLfu =new LFU();
    threadLru =new LRU();
    threadOpt =new OPT();

    //将`threadFifo`对象移动到`thread1`线程中执行
    threadFifo->moveToThread(thread1);
    threadLfu->moveToThread(thread2);
    threadLru->moveToThread(thread3);
    threadOpt->moveToThread(thread4);

    /*
     * 建立信号连接
     */
    connect(this,&run::startThread,threadFifo,&FIFO::start);
    connect(this,&run::startThread,threadLru,&LRU::start);
    connect(this,&run::startThread,threadLfu,&LFU::start);
    connect(this,&run::startThread,threadOpt,&OPT::start);


    connect(threadFifo,&FIFO::interrupt,this,&run::setTable,Qt::QueuedConnection);
    connect(threadLru,&LRU::interrupt,this,&run::setTable,Qt::QueuedConnection);
    connect(threadLfu,&LFU::interrupt,this,&run::setTable,Qt::QueuedConnection);
    connect(threadOpt,&OPT::interrupt,this,&run::setTable,Qt::QueuedConnection);
    //暂停线程
    connect(ui->fifostop,&QPushButton::clicked,threadFifo,&FIFO::threadPause);
    connect(ui->lrustop,&QPushButton::clicked,threadLru,&LRU::threadPause);
    connect(ui->lfustop,&QPushButton::clicked,threadLfu,&LFU::threadPause);
    connect(ui->optstop,&QPushButton::clicked,threadOpt,&OPT::threadPause);
    //继续线程
    connect(ui->fifokeep,&QPushButton::clicked,threadFifo,&FIFO::threadContinue);
    connect(ui->lrukeep,&QPushButton::clicked,threadLru,&LRU::threadContinue);
    connect(ui->lfukeep,&QPushButton::clicked,threadLfu,&LFU::threadContinue);
    connect(ui->optkeep,&QPushButton::clicked,threadOpt,&OPT::threadContinue);
    //结束线程
    connect(this,&run::stopThreadTimer,threadFifo,&FIFO::stopTimer);
    connect(this,&run::stopThreadTimer,threadLru,&LRU::stopTimer);
    connect(this,&run::stopThreadTimer,threadLfu,&LFU::stopTimer);
    connect(this,&run::stopThreadTimer,threadOpt,&OPT::stopTimer);
    //正常结束
    connect(threadFifo,&FIFO::stop,this,&run::addEnd);
    connect(threadLru,&LRU::stop,this,&run::addEnd);
    connect(threadLfu,&LFU::stop,this,&run::addEnd);
    connect(threadOpt,&OPT::stop,this,&run::addEnd);

}


//设置表格

void run::setTable(int replaceIndex,int interrupt,int index,int flag){
    index+=1;
    int i=1;
    QTableWidget *tw;
    QListWidget *lw;
    list<int> pageTable,quickTable;
    switch(flag){
        case 1: tw=fifoTable;lw=fifoTLB;
                pageTable=fifo_pageTable; quickTable=fifo_quickTable;
                fifoupdateParam(QString::number(fifo_interrupt),QString::number(fifo_totalTime),QString::number(fifo_avgTime,'f',2),QString::number(fifo_miss_page*100,'f',2));
        break;
        case 2: tw=lruTable;lw=lruTLB;
                pageTable=lru_pageTable; quickTable=lru_quickTable;//两位小数，浮点数字符串
                lruupdateParam(QString::number(lru_interrupt),QString::number(lru_totalTime),QString::number(lru_avgTime,'f',2),QString::number(lru_miss_page*100,'f',2));
        break;
        case 3:tw=lfuTable;lw=lfuTLB;
               pageTable=lfu_pageTable; quickTable=lfu_quickTable;
               lfuupdateParam(QString::number(lfu_interrupt),QString::number(lfu_totalTime),QString::number(lfu_avgTime,'f',2),QString::number(lfu_miss_page*100,'f',2));
        break;
        case 4:tw=optTable;lw=optTLB;
               pageTable=opt_pageTable; quickTable=opt_quickTable;
               optupdateParam(QString::number(opt_interrupt),QString::number(opt_totalTime),QString::number(opt_avgTime,'f',2),QString::number(opt_miss_page*100,'f',2));
        break;
    }

    for(auto it:pageTable){
        tw->setItem(index,i,new QTableWidgetItem(QString::number(it)));
        tw->item(index,i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        i+=1;
    }

    //设置快表
    i=1;
    lw->clear();
    QListWidgetItem *item=new QListWidgetItem("快表");
    item->setTextAlignment(Qt::AlignHCenter);
    lw->insertItem(0,item);
    for(auto it:quickTable){
        QListWidgetItem *item=new QListWidgetItem(QString::number(it));
        item->setTextAlignment(Qt::AlignHCenter);
        lw->insertItem(i++,item);
    }

    //边色和置换信息
    if(interrupt!=-1)
    {
        tw->item(index,0)->setBackground(Qt::gray);
        if(replaceIndex!=-1)
        {
            QString replaceMessage;
            replaceMessage=QString::number(interrupt)+"置换了"+QString::number(replaceIndex);
            tw->setItem(index,memoryBlockNum+1,new QTableWidgetItem(replaceMessage));
            tw->item(index,memoryBlockNum+1)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}


/*
 * 更新参数
 */
void run::fifoupdateParam(QString labelITR,QString labelTT,QString labelAT,QString labelMP)
{
    ui->labelITR_4->clear();
    ui->labelAT_4->clear();
    ui->labelMP_4->clear();
    ui->labelTT_4->clear();
    ui->labelITR_4->setText("中断次数: "+labelITR);  //中断次数
    ui->labelTT_4->setText("总时间: "+labelTT+"ms");    //总时间
    ui->labelAT_4->setText("平均时间: "+labelAT+"ms");    //平均时间
    ui->labelMP_4->setText("缺页率: "+labelMP);    //缺页率
}
void run::lruupdateParam(QString labelITR,QString labelTT,QString labelAT,QString labelMP)
{
    ui->labelITR_6->clear();
    ui->labelAT_6->clear();
    ui->labelMP_6->clear();
    ui->labelTT_6->clear();
    ui->labelITR_6->setText("中断次数: "+labelITR);  //中断次数
    ui->labelTT_6->setText("总时间: "+labelTT+"ms");    //总时间
    ui->labelAT_6->setText("平均时间: "+labelAT+"ms");    //平均时间
    ui->labelMP_6->setText("缺页率: "+labelMP);    //缺页率
}
void run::lfuupdateParam(QString labelITR,QString labelTT,QString labelAT,QString labelMP)
{
    ui->labelITR_5->clear();
    ui->labelAT_5->clear();
    ui->labelMP_5->clear();
    ui->labelTT_5->clear();
    ui->labelITR_5->setText("中断次数: "+labelITR);  //中断次数
    ui->labelTT_5->setText("总时间: "+labelTT+"ms");    //总时间
    ui->labelAT_5->setText("平均时间: "+labelAT+"ms");    //平均时间
    ui->labelMP_5->setText("缺页率: "+labelMP);    //缺页率
}
void run::optupdateParam(QString labelITR,QString labelTT,QString labelAT,QString labelMP)
{
    ui->labelITR_3->clear();
    ui->labelAT_3->clear();
    ui->labelMP_3->clear();
    ui->labelTT_3->clear();
    ui->labelITR_3->setText("中断次数: "+labelITR);  //中断次数
    ui->labelTT_3->setText("总时间: "+labelTT+"ms");    //总时间
    ui->labelAT_3->setText("平均时间: "+labelAT+"ms");    //平均时间
    ui->labelMP_3->setText("缺页率: "+labelMP);    //缺页率
}

void run::addEnd(){
    is_end++;
    if(is_end==4)
    {
        on_stopall_clicked();
    }
}


void run::on_stopall_clicked()
{
    emit stopThreadTimer();
    _sleep(100);//暂停执行 100 毫秒后再继续执行下面的代码。
    thread1->quit();
    thread1->wait();
    delete thread1;
    delete threadFifo;
    thread2->quit();
    thread2->wait();
    delete thread2;
    delete threadLru;
    thread3->quit();
    thread3->wait();
    delete thread3;
    delete threadLfu;
    thread4->quit();
    thread4->wait();
    delete thread4;
    delete threadOpt;
    ui->optkeep->setEnabled(false);
    ui->optstop->setEnabled(false);
    ui->fifokeep->setEnabled(false);
    ui->fifostop->setEnabled(false);
    ui->lfukeep->setEnabled(false);
    ui->lfustop->setEnabled(false);
    ui->lrukeep->setEnabled(false);
    ui->lrustop->setEnabled(false);
    ui->stopall->setEnabled(false);
}

void run::on_optstop_clicked()
{
    ui->optkeep->setEnabled(true);
    ui->optstop->setEnabled(false);
}


void run::on_optkeep_clicked()
{
    ui->optkeep->setEnabled(false);
    ui->optstop->setEnabled(true);
}

void run::on_fifostop_clicked()
{
    ui->fifokeep->setEnabled(true);
    ui->fifostop->setEnabled(false);
}

void run::on_fifokeep_clicked()
{
    ui->fifokeep->setEnabled(false);
    ui->fifostop->setEnabled(true);
}

void run::on_lfustop_clicked()
{
    ui->lfukeep->setEnabled(true);
    ui->lfustop->setEnabled(false);
}

void run::on_lfukeep_clicked()
{
    ui->lfukeep->setEnabled(false);
    ui->lfustop->setEnabled(true);
}

void run::on_lrustop_clicked()
{
    ui->lrukeep->setEnabled(true);
    ui->lrustop->setEnabled(false);
}

void run::on_lrukeep_clicked()
{
    ui->lrukeep->setEnabled(false);
    ui->lrustop->setEnabled(true);
}
