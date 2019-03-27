#ifndef FILEPROPERTYEDITOR_H
#define FILEPROPERTYEDITOR_H

#include <QWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QLabel>
#include <QSpacerItem>

class FilePropertyEditor : public QWidget
{
    Q_OBJECT
public:
    explicit FilePropertyEditor(QWidget *parent = 0);
    QString getFile() const;
    bool eventFilter(QObject *obj, QEvent *ev);

signals:
    void dataChangedByUser(const QString &file, FilePropertyEditor* editor);

public slots:
    void setFile(const QString &file);

private slots:
    void onToolButtonClicked();

private:
    QString file_;
    QToolButton* toolButton_;
    QLabel* textLabel_;
    QSpacerItem* spacer_;
};

#endif // FILEPROPERTYEDITOR_H
