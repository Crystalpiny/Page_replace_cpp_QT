#include "analyze.h"
#include "ui_analyze.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtWidgets/QGridLayout>
#include <QBarSet>
#include <QBarSeries>
#include <QBarCategoryAxis>
#include <QDateTime>
#include <QMessageBox>

#define GB 1073741824     //定义GB容量大小

analyze::analyze(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::analyze)
{
    ui->setupUi(this);
    baseLayout=new QGridLayout();//布局管理器
    baseLayout->setContentsMargins(30,30,30,30); //调整边距
}

analyze::~analyze()
{
    delete ui;
}

void analyze::init()
{
    /********************Creating Barchart1*****************/
    chartView1=new QChartView(creatBarChart1());
    chartView1->setStyleSheet("background: rgba(0,0,0,0.5)");
    chartView1->setRenderHint(QPainter::Antialiasing);//渲染抗锯齿
        //将barchart1放置在widget中，并且这种方法可以使chart随窗口的拉动而动态变化
    baseLayout->addWidget(chartView1,0,0);

    //*******************Creating BarChart2*********************//

    chartView2=new QChartView(creatBarChart2());
    chartView2->setRenderHint(QPainter::Antialiasing);//渲染抗锯齿
    chartView2->setStyleSheet("background: rgba(0,0,0,0.5)");
        //将barchart2放置在widget中，并且这种方法可以使chart随窗口的拉动而动态变化
    baseLayout->addWidget(chartView2,0,1);
    ui->table->setLayout(baseLayout);
}

void analyze::del()
{

    delete chartView1;
    chartView1=nullptr;
    delete chartView2;
    chartView2=nullptr;
    //baseLayout->deleteLater();
}


QChart *analyze::creatBarChart1() const
{
    QBarSet *set0=new QBarSet("缺页率");
        *set0<<opt_miss_page<<fifo_miss_page<<lru_miss_page<<lfu_miss_page;
        //创建一个序列对象，并将上面的数据添加到序列中
        QBarSeries *bseries=new QBarSeries();
        bseries->append(set0);
        bseries->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd); //在柱子顶部显示数值
        bseries->setLabelsVisible(true);
        //创建一个QChart类的对象chart，并将series对象加入到chart中
        QChart *chart = new QChart();
        chart->addSeries(bseries);
        chart->setBackgroundVisible(false);
        //设置标题和动画
        chart->setTitle("四种页面置换算法缺页率");
        chart->legend()->setLabelColor(Qt::white);
        chart->setTitleBrush(Qt::white);
        chart->setAnimationOptions(QChart::SeriesAnimations);//设置动画
        set0->setColor(QColor(135,206,255));

        //创建坐标
        //首先创建一个字符串列表，作为横坐标
        QStringList *categories = new QStringList();
        *categories<<"OPT"<<"FIFO"<<"LRU"<<"LFU";
        //创建一个类别轴对象
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(*categories);//添加分类
        axisX->setLabelsColor(QColor(255,255,255));
        chart->createDefaultAxes();//设置默认坐标轴，虽然后面的设置会覆盖默认坐标轴，但是也要在后面设置之前进行初始设置
        chart->axes(Qt::Vertical).back()->setLabelsColor(QColor(255,255,255)); //y轴字体颜色
        chart->axes(Qt::Vertical).back()->setRange(0,1);
        chart->setAxisX(axisX,bseries);

        //设置图例
        chart->legend()->setVisible(true);//图例可见
        chart->legend()->setAlignment(Qt::AlignBottom);//将图例放在表底
        return chart;
}

QChart *analyze::creatBarChart2() const
{
    QBarSet *set1 = new QBarSet("时间（ms）");
        *set1<<opt_totalTime<<fifo_totalTime<<lru_totalTime<<lfu_totalTime;
        //创建一个序列对象，并将上面的数据添加到序列中
        QBarSeries *bseries = new QBarSeries();
        bseries->append(set1);
        bseries->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
        bseries->setLabelsVisible(true);
        //创建一个QChart类的对象chart，并将series对象加入到chart中
        QChart *chart = new QChart();
        //QChart *chart2=ui_bar->chartViewBar2->chart();
        chart->addSeries(bseries);
        chart->setBackgroundVisible(false);
        //设置标题和动画
        chart->setTitle("四种页面置换算法时间");
        chart->legend()->setLabelColor(Qt::white);
        chart->setTitleBrush(Qt::white);
        chart->setAnimationOptions(QChart::SeriesAnimations);//设置动画
        set1->setColor(QColor(102,205,170));

        //创建坐标
        //首先创建一个字符串列表，作为横坐标
        QStringList *categories = new QStringList();
        *categories<<"OPT"<<"FIFO"<<"LRU"<<"LFU";
        //创建一个类别轴对象
        QBarCategoryAxis *axis = new QBarCategoryAxis();
        axis->append(*categories);//添加分类
        axis->setLabelsColor(QColor(255,255,255));
        chart->createDefaultAxes();//设置默认坐标轴，虽然后面的设置会覆盖默认坐标轴，但是也要在后面设置之前进行初始设置
        chart->axes(Qt::Vertical).back()->setLabelsColor(QColor(255,255,255));
        chart->axes(Qt::Vertical).back()->setRange(0,400);
        chart->setAxisX(axis,bseries);//将类别轴对象和序列对象作为参数，设置X轴。

        //设置图例
        chart->legend()->setVisible(true);//图例可见
        chart->legend()->setAlignment(Qt::AlignBottom);//将图例放在表底
        return chart;

}

void analyze::on_pushButton_13_clicked()
{
    this->hide();
    emit showsettingsig();
}

void analyze::on_pushButton_14_clicked()
{
    this->hide();
    emit showrunsig();
}

void analyze::on_saveButton_clicked()
{
    QDateTime time = QDateTime::currentDateTime();
    QString dateTime = time.toString("yyyy-MM-dd-HH-mm-ss");
    QString fileName = "D:/" + dateTime + ".txt";
    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream in(&file);

    QString str = QString::fromUtf8("输入......");
    in << str << endl ;
    str = QString::fromUtf8("页表大小：");
    in << str << memoryBlockNum << endl;
    str = QString::fromUtf8("内存存取时间：");
    in << str << memAcesTime << endl;
    if(useQTabl)
    {
        str = QString::fromUtf8("启用快表，快表大小：");
        in << str << quickNum << endl;
    }
    else
    {
        str = QString::fromUtf8("不启用快表，快表大小：");
    }
    str = QString::fromUtf8("快表存取时间：");
    in << str << quiAcesTime << endl;
    str = QString::fromUtf8("缺页中断时间：");
    in << str << missPageTime << endl;
    str = QString::fromUtf8("输入的页面序列：");
    QString pageListString = "";
    for (int i = 0; i < pageList.size(); i++) {
        if(i==pageList.size()-1){
            pageListString += QString::number(pageList[i]) + "";
        }
        else{
            pageListString += QString::number(pageList[i]) + ",";
        }
    }
    in << str << pageListString << endl;

    str = QString::fromUtf8("输出......");
    in << endl << str << endl;
    QString count = QString::fromUtf8("中断次数：");
    QString tTime = QString::fromUtf8("总时间：");
    QString aveTime = QString::fromUtf8("平均时间：");
    QString rate = QString::fromUtf8("缺页率：");

    str = QString::fromUtf8("FIFO： ");
    in << str << count << fifo_interrupt << " ";
    in << tTime << fifo_totalTime << " ";
    in << aveTime << fifo_avgTime << " ";
    in << rate << fifo_miss_page << endl;

    str = QString::fromUtf8("LRU：  ");
    in << str <<  count << lru_interrupt << " ";
    in << tTime << lru_totalTime << " ";
    in << aveTime << lru_avgTime << " ";
    in << rate << lru_miss_page << endl;

    str = QString::fromUtf8("LFU：  ");
    in << str <<  count << lfu_interrupt << " ";
    in << tTime << lfu_totalTime << " ";
    in << aveTime << lfu_avgTime << " ";
    in << rate << lfu_miss_page << endl;

    str = QString::fromUtf8("OPT：  ");
    in << str <<  count << opt_interrupt << " ";
    in << tTime << opt_totalTime << " ";
    in << aveTime << opt_avgTime << " ";
    in << rate << opt_miss_page << endl;

    //计算机名称
    in << endl;
    str=QHostInfo::localHostName();
    QString machineName=QString::fromUtf8("计算机名称：");
    in << machineName << str << endl;

    //cpu信息
    QSettings *CPU = new QSettings("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",QSettings::NativeFormat);
    str = CPU->value("ProcessorNameString").toString();
    QString cpuInfo=QString::fromUtf8("CPU信息：");
    in << cpuInfo << str << endl;

    //内存信息
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof (statex);
    GlobalMemoryStatusEx(&statex);
    double m_totalMem = statex.ullTotalPhys  * 1.0/ GB;
    double m_freeMem = statex.ullAvailPhys * 1.0 / GB;
    str = QString("可用 %1 GB / 共 %2 GB" ).arg(QString::asprintf("%.2f", m_freeMem)).arg(QString::asprintf("%.2f", m_totalMem));
    QString memoryInfo=QString::fromUtf8("内存信息：");
    in << memoryInfo << str << endl;

    //操作系统信息
//    QString sysBit = "unknown";
//    if(QSysInfo::currentCpuArchitecture()==QLatin1String("x86_64")){
//        sysBit = "64位";
//    }else{
//        sysBit = "32位";
//    }

//    str = QSysInfo::prettyProductName() + " " + sysBit;
//    QString osInfo=QString::fromUtf8("操作系统信息：");
//    in << osInfo << str << endl;

    //硬盘信息
    QString m_diskDescribe = "";
    QFileInfoList list = QDir::drives();
    foreach (QFileInfo dir, list)
    {
        QString dirName = dir.absolutePath();
        dirName.remove("/");
        LPCWSTR lpcwstrDriver = (LPCWSTR)dirName.utf16();
        ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;
        if(GetDiskFreeSpaceEx(lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes) )
        {
            QString free = QString::number((double) liTotalFreeBytes.QuadPart / GB, 'f', 1);
            free += "G";
            QString all = QString::number((double) liTotalBytes.QuadPart / GB, 'f', 1);
            all += "G";

            QString str = QString("%1 %2/%3       ").arg(dirName, free, all);
            m_diskDescribe += str;

            double freeMem = (double) liTotalFreeBytes.QuadPart / GB;
            double m_maxFreeDisk=0.0;
            if(freeMem > m_maxFreeDisk)
                m_maxFreeDisk = freeMem;
        }
    }
    QString diskInfo=QString::fromUtf8("硬盘信息：");
    in << diskInfo << m_diskDescribe << endl;


    bool flag = file.exists();
       if(!flag)
       {
           QMessageBox mess1(QMessageBox::Critical,tr("错误"),tr("保存失败！"));
           QPushButton *okbutton = (mess1.addButton(tr("确定"),QMessageBox::AcceptRole));
           mess1.exec();
       }
       else
       {
           QMessageBox mess2(QMessageBox::Information,tr("提示"),tr("保存成功！"));
           QPushButton *okbutton = (mess2.addButton(tr("确定"),QMessageBox::AcceptRole));
           mess2.exec();
       }

    file.flush();
    file.close();
}
