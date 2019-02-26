#ifndef RULETABLEMODEL_H
#define RULETABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include "features.h"

class RuleTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    RuleTableModel(QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;

    void addRule(Features *value);
    void deleteRule(const QModelIndex &index);
    QList <Features*> getRuleList() const;

private:
    enum {
        H_STATE,
        H_BGCOLOR,
        H_TEXT,
        H_BORDERCOLOR,
        H_SIGNCOLOR
    };

    QStringList headers;
    QList <Features*> ruleList;

    QVariant getData(int num, int pos) const;

signals:
    void changed();
};

#endif // RULETABLEMODEL_H
