#include "settings.h"
#include "ui_settings.h"

settings::settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings)
{
    ui->setupUi(this);
    ui->TextQTable->setEnabled(false);
    ui->QAcessTime->setEnabled(false);
}

settings::~settings()
{
    delete ui;
}


//void settings::on_pushButton_14_clicked()
//{
//    this->hide();
//    emit showrunsig();
//}

//void settings::on_pushButton_15_clicked()
//{
//    this->hide();
//    emit showanalyzesig();
//}


/*
 * 随机生成页面个数，个数范围必须得在TextPageLow-TextPageHigh;手动输入页面个数则无需受限于这范围
 * 随机生成页号序列，页号范围必须得在TextListLow-TextListHigh,且个数必得是TextPagenum;手动输入页号序列，无需范围，但个数必得是TextPagenum，否则报错
 */


/*
 *        随机生成页面个数
 */

void settings::on_randPagenum_clicked()
{
    if(ui->TextPageLow->toPlainText().toInt()<0){
        QMessageBox::critical(this, "错误", "所给个数范围最小值不能必得大于0，已重置为默认值");
        page_low=1;
        ui->TextPageLow->setPlainText(QString::number(page_low));
    }
    //low大于high,重置为默认值，
    if(ui->TextPageLow->toPlainText().toInt()>ui->TextPageHigh->toPlainText().toInt()){
        int select=QMessageBox::information(this,"提示","所给个数范围不合理,是否重置为默认范围!","确定","取消",0,1);
        if(select==0)
        {
            page_low=1;
            page_high=10;
            ui->TextPageLow->setPlainText(QString::number(page_low));
            ui->TextPageHigh->setPlainText(QString::number(page_high));
        }
    }
    else{
        page_low=ui->TextPageLow->toPlainText().toInt();
        page_high=ui->TextPageHigh->toPlainText().toInt();
        qsrand(QDateTime::currentMSecsSinceEpoch());
        pageNum = qrand()%(page_high-page_low) + page_low;
        ui->TextPagenum->setPlainText(QString::number(pageNum));
        // ui->pBtn_pgList->setEnabled(true);
    }
//    qDebug()<<page_low;
}

/*
 *        随机生成页面序列
 */
void settings::on_randPagelist_clicked()
{
        page_list_low=ui->TextListLow->toPlainText().toInt();
        page_list_high=ui->TextListHigh->toPlainText().toInt();

        //防止手输页面个数时，全局变量pageNum并未读取文本框TextPagenum的值
        pageNum=ui->TextPagenum->toPlainText().toInt();

        //在随机生成页面序列之前，先清空pageList
        pageList.clear();

        for(int i=0;i<pageNum;i++)
        {
            pageList.append(qrand()%(page_list_high-page_list_low) + page_list_low);
        }

        //把QList类型的pageList转化为QString类型
        QString pageListString = "";
        for (int i = 0; i < pageList.size(); i++) {
            if(i==pageList.size()-1){
                pageListString += QString::number(pageList[i]) + "";
            }
            else{
                pageListString += QString::number(pageList[i]) + ",";
            }
        }
        ui->TextRandList->setPlainText(pageListString);
}

/*
 *        手动输入页面序列
 */
void settings::on_HandInput_clicked()
{
        pageNum=ui->TextPagenum->toPlainText().toInt();
        if(ui->TextRandList->toPlainText().isEmpty()){
            QMessageBox::critical(this, "错误", "页面序列不能为空，请输入");
        }
        else if(ui->TextRandList->toPlainText().length()/2+1!=pageNum){
            QMessageBox::critical(this, "错误", "页面序列数与页面个数不匹配，请重新输入");
//            qDebug()<<pageNum;
            qDebug()<<ui->TextRandList->toPlainText();
            qDebug()<<ui->TextRandList->toPlainText().length();
//            qDebug()<<ui->TextRandList->toPlainText().length()/2+1;
        }
        else{
            QString inputStr = ui->TextRandList->toPlainText(); // 读取手动输入的字符串
            QStringList inputList = inputStr.split(","); // 将字符串按逗号分割成多个子串
            pageList.clear(); // 清空页面序列

            // 将每个子串转换为整数，并添加到页面序列中
            foreach (QString str, inputList) {
                int page = str.toInt();
                pageList.append(page);
            }
        }

        //  qDebug()<<pageList;
}

/*
 *       是否使用快表
 */
// 用stateChanged()函数读取QCheckBox的状态,
// 当QCheckBox被选中时，参数的值将被设置为Qt::Checked；当QCheckBox未被选中时，参数的值将被设置为Qt::Unchecked。
// 在Qt中，QWidget类及其子类（例如QPushButton）具有一个名为setEnabled()的函数。此函数用于启用或禁用控件。当控件被禁用时，用户无法与其进行交互，并且控件将被显示为灰色。
void settings::on_useQTable_stateChanged(int arg1)
{
    if(arg1==Qt::Checked){
        useQTabl=true;
        ui->TextQTable->setEnabled(true);
        ui->QAcessTime->setEnabled(true);
    }
    else{
        useQTabl=false;
        ui->TextQTable->setEnabled(false);
        ui->QAcessTime->setEnabled(false);
    }
    // qDebug()<<useQTabl;
}

/*
 *     判断所需的参数是否都填写
 */
bool settings::allIsEdited()
{
    bool editFull=true;  //判断设置的参数是否都填写
    if(ui->TextPTable->toPlainText().isEmpty())
    {
        editFull=false;
        QMessageBox::information(this,"提示","页表大小不能为空，请重新设置","确定",0);
    }
    else if(ui->TextPagenum->toPlainText().isEmpty())
    {
       editFull=false;
       QMessageBox::information(this,"提示","页面个数不能为空，请重新设置","确定",0);
    }
    else if(ui->TextRandList->toPlainText().isEmpty())
    {
        editFull=false;
        QMessageBox::information(this,"提示","页面序列不能为空，请重新设置","确定",0);
    }
    else if(ui->MAcessTime->toPlainText().isEmpty())
    {
        editFull=false;
        QMessageBox::information(this,"提示","内存存取时间不能为空，请重新设置","确定",0);
    }
    else if(ui->LossPageTime->toPlainText().isEmpty())
    {
        editFull=false;
        QMessageBox::information(this,"提示","缺页中断时间不能为空，请重新设置","确定",0);
    }
    if(ui->useQTable->isChecked())
    {
        if(ui->TextQTable->toPlainText().isEmpty())
        {
            editFull=false;
            QMessageBox::information(this,"提示","快表大小不能为空，请重新设置","确定",0);
        }
        else if(ui->QAcessTime->toPlainText().isEmpty())
        {
            editFull=false;
            QMessageBox::information(this,"提示","快表存取时间不能为空，请重新设置","确定",0);
        }
    }
    return editFull;
}

/*
 *    保存参数，即设置相应的全局变量的值
 */
void settings::on_Saveparams_clicked()
{
    if(allIsEdited()){
        memoryBlockNum=ui->TextPTable->toPlainText().toInt();
        pageNum=ui->TextPagenum->toPlainText().toInt();
        memAcesTime=ui->MAcessTime->toPlainText().toInt();
        missPageTime=ui->LossPageTime->toPlainText().toInt();
        QString inputStr = ui->TextRandList->toPlainText(); // 读取输入的字符串
        QStringList inputList = inputStr.split(","); // 将字符串按逗号分割成多个子串
        pageList.clear(); // 清空页面序列

        // 将每个子串转换为整数，并添加到页面序列中
        foreach (QString str, inputList) {
            int page = str.toInt();
            pageList.append(page);
        }
        if(ui->useQTable->isChecked())
        {
            quickNum=ui->TextQTable->toPlainText().toUInt();
            quiAcesTime=ui->QAcessTime->toPlainText().toUInt();
        }
        QMessageBox::information(this,"提示","参数保存成功，可以开始执行页面置换模拟","确定",0);
    }

}



//目前这个只能实现点击OPT按钮就跳到运行界面，咱想实现的是点击点击OPT按钮跳到运行界面，然后OPT那个表格开始运行
void settings::on_OPT_clicked()
{
    this->hide();
    flags=1;
    emit showrunsig();
}


void settings::on_FIFO_clicked()
{
    this->hide();
    flags=2;
    emit showrunsig();
}


void settings::on_LRU_clicked()
{
    this->hide();
    flags=4;
    emit showrunsig();
}


void settings::on_LFU_clicked()
{
    this->hide();
    flags=3;
    emit showrunsig();
}

void settings::on_RunALL_clicked()
{
    this->hide();
    flags=5;
    emit showrunsig();
//    emit showallsig();
//    if(useQTabl){
//        emit showQfifosig();
//        emit showQlrusig();
//        emit showQlfusig();
//        emit showQoptsig();
//        emit showallQsig();

//        emit showfifosig();
//        emit showlrusig();
//        emit showlfusig();
//        emit showoptsig();
//    }
//    else{
//        emit showfifosig();
//        emit showlrusig();
//        emit showlfusig();
//        emit showoptsig();

//        emit showallsig();
//    }
}



/*
 *    恢复默认值
 */

void settings::on_resDefault_clicked()
{
    page_low = 1;
    page_high = 10;
    pageNum = 10; // 逻辑页面个数

    page_list_low=1;
    page_list_high=10;
    pageList.clear();
    ui->TextRandList->setPlainText("");

    memoryBlockNum = 3; // 驻留内存页面个数
    quickNum = 2;            // 快表的大小
    useQTabl = false;

    memAcesTime = 8;
    missPageTime = 20;
    quiAcesTime = 1;


    ui->TextPageLow->setPlainText(QString::number(page_low));
    ui->TextPageHigh->setPlainText(QString::number(page_high));
    ui->TextPagenum->setPlainText(QString::number(pageNum));
    ui->TextListLow->setPlainText(QString::number(page_list_low));
    ui->TextListHigh->setPlainText(QString::number(page_list_high));
    ui->TextPTable->setPlainText(QString::number(memoryBlockNum));
    ui->TextQTable->setPlainText(QString::number(quickNum));
    ui->useQTable->setChecked(false);
    ui->QAcessTime->setPlainText(QString::number(quiAcesTime));
    ui->MAcessTime->setPlainText(QString::number(memAcesTime));
    ui->LossPageTime->setPlainText(QString::number(missPageTime));
}
