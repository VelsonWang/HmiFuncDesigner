#ifndef ELEMENTLIBRARYLISTWIDGET_H
#define ELEMENTLIBRARYLISTWIDGET_H

#include <QListWidget>
#include <QMouseEvent>
#include <QListWidgetItem>

class ElementLibraryListWidgetItem : public QListWidgetItem
{
public:
    ElementLibraryListWidgetItem();

    void setFilePath(const QString &);
    QString getFilePath() const;

private:
    QString m_szFilePath;
};

//------------------------------------------------------------------------------

class ElementLibraryListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit ElementLibraryListWidget(QListWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void startDrag();

    QPoint startPos;

public slots:
    void loadLibrary();
};

#endif // ELEMENTLIBRARYLISTWIDGET_H
