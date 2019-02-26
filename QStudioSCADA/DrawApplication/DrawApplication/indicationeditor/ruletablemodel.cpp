#include "ruletablemodel.h"
#include "coloreditor.h"
#include <QColor>

RuleTableModel::RuleTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    headers << trUtf8("Состояние") << trUtf8("Цвет фона") << trUtf8("Текст")
            << trUtf8("Цвет рамки") << trUtf8("Цвет знака");
}

QVariant RuleTableModel::data(const QModelIndex &index, int role) const {

    if (index.isValid() && role == Qt::DisplayRole) {
        return getData(index.row(),index.column());
    }
    else if (index.isValid() && role == Qt::DecorationRole) {

        if (index.column() == 1 || index.column() == 3 || index.column() == 4) {
            return ColorEditor::getColorPixmap(getData(index.row(),index.column()).value<QColor>());
        }
    }
    return QVariant();
}

QVariant RuleTableModel::getData(int num, int pos) const {

    switch (pos) {
        case H_STATE:
            return QVariant(ruleList.at(num)->state);
        case H_BGCOLOR:
            return QVariant(ruleList.at(num)->background);
        case H_TEXT:
            return QVariant(ruleList.at(num)->text);
        case H_BORDERCOLOR:
            return QVariant(ruleList.at(num)->bordercolor);
        case H_SIGNCOLOR:
            return QVariant(ruleList.at(num)->signbackground);
    }

    return QVariant();
}

bool RuleTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {

    if (index.isValid() && role == Qt::EditRole) {

        Features *f = ruleList.at(index.row());

        switch (index.column()) {
            case H_STATE:
                f->state = value.toString();
                break;
            case H_BGCOLOR:
                f->background = value.value<QColor>();
                break;
            case H_TEXT:
                f->text = value.toString();
                break;
            case H_BORDERCOLOR:
                f->bordercolor = value.value<QColor>();
                break;
            case H_SIGNCOLOR:
                f->signbackground = value.value<QColor>();
                break;
            default:
                return false;
        }

        emit dataChanged(index,index);
        emit changed();
        return true;
    }

    return false;
}

QVariant RuleTableModel::headerData(int section, Qt::Orientation orientation, int role) const {

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {

        switch (section) {

        case H_STATE:
            return headers[H_STATE];
        case H_BGCOLOR:
            return headers[H_BGCOLOR];
        case H_TEXT:
            return headers[H_TEXT];
        case H_BORDERCOLOR:
            return headers[H_BORDERCOLOR];
        case H_SIGNCOLOR:
            return headers[H_SIGNCOLOR];
        }
    }
    else
        return QVariant();

    return QVariant();
}

Qt::ItemFlags RuleTableModel::flags(const QModelIndex &index) const {

    if (index.isValid()) {
        return QAbstractTableModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsEnabled
                | Qt::ItemIsSelectable;
    }

    return Qt::NoItemFlags;
}

int RuleTableModel::rowCount(const QModelIndex &parent) const {

    Q_UNUSED(parent)
    return ruleList.size();
}

int RuleTableModel::columnCount(const QModelIndex &parent) const {

    Q_UNUSED(parent)
    return 5;
}

void RuleTableModel::addRule(Features *value) {

    beginInsertRows(QModelIndex(),ruleList.size(),ruleList.size());
    ruleList.insert(ruleList.end(),value);
    endInsertRows();
}

void RuleTableModel::deleteRule(const QModelIndex &index) {

    if (index.isValid()) {
        beginRemoveRows(QModelIndex(),index.row(),index.row());
        Features *f = ruleList.takeAt(index.row());
        delete f;
        endRemoveRows();
    }
}

QList <Features*> RuleTableModel::getRuleList() const {
    return ruleList;
}
