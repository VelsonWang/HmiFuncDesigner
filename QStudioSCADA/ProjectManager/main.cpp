#include "MainWindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("JasonWangCtd");

    QFont font;
    font.setFamily("宋体");
    font.setPointSize(10);
    app.setFont(font);

    MainWindow win;
    win.show();

    return app.exec();
}
