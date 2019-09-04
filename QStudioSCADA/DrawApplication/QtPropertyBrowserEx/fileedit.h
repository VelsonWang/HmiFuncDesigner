
#ifndef FILEEDIT_H
#define FILEEDIT_H

#include <QLabel>

class FileEdit : public QWidget
{
    Q_OBJECT
public:
    FileEdit(QWidget *parent = Q_NULLPTR);

    void setFilePath(const QString &filePath);
    QString filePath() const;
    void setFilter(const QString &filter);
    QString filter() const;

signals:
    void filePathChanged(const QString &szVal);

private slots:
    void buttonClicked();

private:
    QLabel *theLabel_;
    QStringList funcs_;
    QString theFilter;

};

#endif
