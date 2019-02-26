#include "previewwindow.h"

PreviewWindow::PreviewWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    setWindowTitle(trUtf8("预览"));
    createActions();
}

void PreviewWindow::createActions() {

    actionClose = new QAction(QIcon(":/images/fullscreen.png"),trUtf8("退出预览 (ESC)"),
                              this);
    actionPrint = new QAction(QIcon(":/images/fileprint.png"),trUtf8("打印"),this);

    toolBar->addAction(actionClose);
    toolBar->addAction(actionPrint);
    toolBar->setMovable(false);

    connect(actionClose,SIGNAL(triggered()),SLOT(slotClose()));
    connect(actionPrint,SIGNAL(triggered()),SLOT(slotPrint()));
}

void PreviewWindow::updatePreview(const QPixmap &pixmap) {

    previewPixmap = pixmap;
    previewLabel->setPixmap(pixmap);
    setWindowState(Qt::WindowMaximized);
    show();
}

void PreviewWindow::keyPressEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Escape) {
        close();
    }
}

void PreviewWindow::slotClose() {
    close();
}

void PreviewWindow::slotPrint() {

    QPrinter printer;
    printer.setPaperSize(QSizeF(previewLabel->width(),previewLabel->height()),
                         QPrinter::Point);

    QPrintDialog dialog(&printer);

    if (dialog.exec()) {
        QPainter painter(&printer);
        painter.drawPixmap(0,0,previewPixmap);
    }
}

