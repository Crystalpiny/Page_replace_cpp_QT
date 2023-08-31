#include "mainwindow.h"
#include<QDebug>
#include <QApplication>
#include <QTest>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("多线程模拟页面置换算法");

//    w.setWindowIcon(QIcon(":/new/prefix1/logo.png",QSize(64,64)));
//    QIcon类的构造函数接受一个文件路径作为参数，表示要使用的图标文件。
//    如果您需要调整图标的大小，可以使用 QIcon::addFile() 方法来添加具有不同尺寸的图标文件。
//    QIcon icon;
//    icon.addFile(":/new/prefix1/logo.png",QSize(128,128));
//    w.setWindowIcon(icon);


//    创建了一个QIcon对象 icon 并设置其为图标文件。然后我们使用 pixmap() 方法将图标转换为QPixmap对象，并设置其大小为64x64像素。
//    接下来，我们使用 scaled() 方法将图像缩放到32x32像素大小，并保持纵横比不变。pixmap 像素图
//    最后，我们将缩放后的QPixmap对象设置为窗口的图标。
//    QIcon icon(":/new/prefix1/logo.png");
//    QPixmap pixmap = icon.pixmap(QSize(64, 64));
//    QPixmap scaledPixmap = pixmap.scaled(QSize(256, 256), Qt::KeepAspectRatio);
//    w.setWindowIcon(QIcon(scaledPixmap));


    w.show();
    return a.exec();
}
