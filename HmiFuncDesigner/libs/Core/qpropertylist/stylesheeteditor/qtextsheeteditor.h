#ifndef QTEXTSHEETEDITOR_H
#define QTEXTSHEETEDITOR_H

#include "qbaseeditorwidget.h"

namespace Ui
{
class QTextSheetEditor;
}

class QTextSheetEditor : public QBaseEditorWidget
{
    Q_OBJECT

public:
    explicit QTextSheetEditor(QWidget *parent = 0);
    ~QTextSheetEditor();

    void setItem(QAbstractStylesheetItem *item);
    void init(QAbstractStylesheetItem *item);
    void takeResource(QUndoCommand *cmd);
    void addResource(QUndoCommand *cmd);

private slots:
    void on_pushButton_clicked();

private:
    Ui::QTextSheetEditor *ui;
};

#endif // QTEXTSHEETEDITOR_H
