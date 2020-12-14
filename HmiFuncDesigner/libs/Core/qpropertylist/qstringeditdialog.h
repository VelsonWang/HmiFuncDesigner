#ifndef QSTRINGEDITDIALOG_H
#define QSTRINGEDITDIALOG_H

#include <QDialog>
#include <QItemDelegate>
#include <QTreeWidget>
#include <QMap>
#include <QUndoStack>

namespace Ui {
class QStringEditDialog;
}

class QAbstractProperty;
class QLanguage;

class QLanguageDelegate: public QItemDelegate
{
    Q_OBJECT
public :
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


class QStringEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit QStringEditDialog(QAbstractProperty *property,QUndoStack* stack,QWidget* parent=0);
    ~QStringEditDialog();

private slots:
    void on_enabled_clicked();

    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

protected:
    QAbstractProperty    *m_property;
    QMap<QTreeWidgetItem*,QLanguage*>     m_items;
    QUndoStack          *m_undo_stack;
private:
    Ui::QStringEditDialog *ui;
};

#endif // QSTRINGEDITDIALOG_H
