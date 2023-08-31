#ifndef GLOBAL_H
#define GLOBAL_H
#include<QMap>
#include<QList>
#include<QVector>
#include<QQueue>
#include<QStack>
#include<QDebug>
#include<list>
#include<iostream>
#include<QTimer>
#include <QTableWidget>
#include<QListWidget>
//#include<qmainwindow_table.h>
using namespace std;

extern int page_low;
extern int page_high;
extern int page_list_low;
extern int page_list_high;

extern int memoryBlockNum;   //驻留内存页面的个数
extern int pageNum; //页面个数
extern QVector<int> pageList;   //页面序列数组形式
extern int memAcesTime;    //内存存取时间
extern bool useQTabl;//是否使用快表
extern int quickNum; //快表大小
extern int quiAcesTime; //快表存取时间
extern int missPageTime; //缺页中断时间
extern int run_time;   //执行速度

//fifo变量设置
extern int  fifo_interrupt;  	//中断次数
extern int fifo_totalTime;  	//总时间
extern double fifo_avgTime;   	//平均时间
extern double fifo_miss_page;	//缺页率
extern list<int> fifo_quickTable; //快表
extern list<int> fifo_pageTable; //页表

//lru变量设置
extern int lru_interrupt;  	//中断次数
extern int lru_totalTime;  	//总时间
extern double lru_avgTime;   	//平均时间
extern double lru_miss_page;	//缺页率
extern list<int> lru_quickTable; //快表
extern list<int> lru_pageTable; //页表

//lfu变量设置
extern int lfu_interrupt;  	//中断次数
extern int lfu_totalTime;  	//总时间
extern double lfu_avgTime;   	//平均时间
extern double lfu_miss_page;	//缺页率
extern list<int> lfu_quickTable; //快表
extern list<int> lfu_pageTable; //页表

//opt变量设置
extern int opt_interrupt;  	//中断次数
extern int opt_totalTime;  	//总时间
extern double opt_avgTime;   	//平均时间
extern double opt_miss_page;	//缺页率
extern list<int> opt_quickTable; //快表
extern list<int> opt_pageTable; //页表

//数据库操作是否完成
//extern bool DB_OK;
//extern QMainWindow_Table *tableFIFO;
//extern QMainWindow_Table *tableLRU;
//extern QMainWindow_Table *tableLFU;
//extern QMainWindow_Table *tableOPT;

extern QTableWidget *fifoTable;
extern QTableWidget *lruTable;
extern QTableWidget *lfuTable;
extern QTableWidget *optTable;

extern QListWidget *fifoTLB;
extern QListWidget *lruTLB;
extern QListWidget *lfuTLB;
extern QListWidget *optTLB;

//runpart
extern QTableWidget *Table;
extern QListWidget *TLB;

//运行标志，判断运行哪一个算法
extern int flags;

#endif // GLOBAL_H
