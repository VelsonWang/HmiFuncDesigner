#ifndef ScriptWin_H
#define ScriptWin_H

#include "../../Public/Public.h"
#include <QWidget>
#include <QListWidget>
#include <QList>
#include <QVBoxLayout>
#include <QStringList>
#include <QJsonObject>

class ScriptWin : public QWidget
{
    Q_OBJECT
public:
    explicit ScriptWin(QWidget *parent = Q_NULLPTR);
    ~ScriptWin();
    void init();

public:
    QString userFriendlyCurrentFile();
    QString currentFile() const;

private:
    void updateUI();

public slots:
    void ListWidgetClicked(QListWidgetItem *item);
    void NewScript();
    void ModifyScript();
    void DeleteScript();

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    bool eventFilter(QObject *obj, QEvent *ev);

private:
    QListWidget *m_pListWidgetObj;
    QVBoxLayout *m_pVLayoutObj;
};


#endif // ScriptWin_H
