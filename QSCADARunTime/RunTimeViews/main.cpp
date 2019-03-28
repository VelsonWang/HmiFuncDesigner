
#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    int ret = 0;

    QApplication  app(argc, argv);


    ret = app.exec();


    return ret;
}
