#include "mainwindow.h"
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[])
{
    QApplication prog(argc, argv);

    MainWindow w;

    w.show();
    
    return prog.exec();
}
