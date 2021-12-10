#ifndef QBACKGROUNSHEETEDITOR_H
#define QBACKGROUNSHEETEDITOR_H

#include "qbaseeditorwidget.h"

namespace Ui
{
class QBackgrounSheetEditor;
}

class QBackgrounSheetEditor : public QBaseEditorWidget
{
    Q_OBJECT

public:
    explicit QBackgrounSheetEditor(QWidget *parent = 0);
    ~QBackgrounSheetEditor();

    void setItem(QAbstractStylesheetItem *item);
    void init(QAbstractStylesheetItem *item);
    void takeResource(QUndoCommand *cmd);
    void addResource(QUndoCommand *cmd);

private slots:
    void on_colorBtn_clicked();
    void on_gradientBtn_clicked();
    void on_imageBtn_clicked();
    void repeat_changed();
    void position_changed();

private:
    Ui::QBackgrounSheetEditor *ui;

protected:
    QString m_tempResource;
    QString m_startResource;
};

#endif // QBACKGROUNSHEETEDITOR_H
