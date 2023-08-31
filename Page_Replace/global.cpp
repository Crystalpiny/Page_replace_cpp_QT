#include "global.h"
#include "run.h"

int page_low = 1;
int page_high = 10;
int page_list_low = 1;
int page_list_high = 10;

int memoryBlockNum=3;   //驻留内存页面的个数
int pageNum=10; //页面个数
QVector<int> pageList;   //页面序列
int memAcesTime=8;    //内存存取时间
bool useQTabl=false;//是否使用快表
int quickNum=2; //快表大小
int quiAcesTime=1; //快表存取时间
int missPageTime=20; //缺页中断时间
int run_time=300;   //执行速度

//fifo变量设置
int  fifo_interrupt;  	//中断次数
int fifo_totalTime;  	//总时间
double fifo_avgTime;   	//平均时间
double fifo_miss_page;	//缺页率
list<int> fifo_quickTable; //快表
list<int> fifo_pageTable; //页表

//lru变量设置
int lru_interrupt;  	//中断次数
int lru_totalTime;  	//总时间
double lru_avgTime;   	//平均时间
double lru_miss_page;	//缺页率
list<int> lru_quickTable; //快表
list<int> lru_pageTable; //页表

//lfu变量设置
int lfu_interrupt;  	//中断次数
int lfu_totalTime;  	//总时间
double lfu_avgTime;   	//平均时间
double lfu_miss_page;	//缺页率
list<int> lfu_quickTable; //快表
list<int> lfu_pageTable; //页表

//opt变量设置
int opt_interrupt;  	//中断次数
int opt_totalTime;  	//总时间
double opt_avgTime;   	//平均时间
double opt_miss_page;	//缺页率
list<int> opt_quickTable; //快表
list<int> opt_pageTable; //页表

bool DB_OK=false;

//QMainWindow_Table *tableFIFO;
//QMainWindow_Table *tableLRU;
//QMainWindow_Table *tableLFU;
//QMainWindow_Table *tableOPT;

QTableWidget *fifoTable;
QTableWidget *lruTable;
QTableWidget *lfuTable;
QTableWidget *optTable;

QListWidget *fifoTLB;
QListWidget *lruTLB;
QListWidget *lfuTLB;
QListWidget *optTLB;
//runpart
QTableWidget *Table;

QListWidget *TLB;

int flags=0;
