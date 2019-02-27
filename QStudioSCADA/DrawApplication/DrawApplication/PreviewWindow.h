#ifndef PREVIEWWINDOW_H
#define PREVIEWWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QtPrintSupport>
#include <QPainter>
#include "ui_previewwindow.h"

class PreviewWindow : public QMainWindow, public Ui::PreviewWindow
{
    Q_OBJECT
    
public:
    explicit PreviewWindow(QWidget *parent = 0);
    void updatePreview(const QPixmap &pixmap);

protected:
    void keyPressEvent(QKeyEvent *);

private:
    void createActions();

    QAction *actionPrint;
    QAction *actionClose;
    QPixmap previewPixmap;

private slots:
    void slotClose();
    void slotPrint();
};

#endif // PREVIEWWINDOW_H
