#ifndef ANALYZE_H
#define ANALYZE_H

#include <QWidget>
#include <QtCharts/QChartGlobal>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include "global.h"
#include <QMessageBox>

//显示计算机信息引入库
#include "windows.h"
//#include <QHostAddress>
//#include <QSysInfo>
#include "QSettings"
//#include "QDebug"
#include <QDesktopWidget>
#include <QFileInfoList>
#include <QDir>
#include <QLibrary>
#include <QTimer>
#include <QHostInfo>


namespace Ui {
class analyze;
}

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE
typedef QPair<QPointF, QString> Data;
typedef QList<Data> DataList;
typedef QList<DataList> DataTable;

QT_CHARTS_USE_NAMESPACE//此句必备

class analyze : public QWidget
{
    Q_OBJECT

public:
    explicit analyze(QWidget *parent = nullptr);
    ~analyze();
    void init();
    void del();
private:
    Ui::analyze *ui;
    QChart *creatBarChart1() const;
    QChart *creatBarChart2() const;
    QGridLayout *baseLayout;
    QChartView *chartView1,*chartView2;
signals:
    void showsettingsig();
    void showrunsig();
private slots:
    void on_pushButton_13_clicked();
    void on_pushButton_14_clicked();
    void on_saveButton_clicked();
};

#endif // ANALYZE_H
