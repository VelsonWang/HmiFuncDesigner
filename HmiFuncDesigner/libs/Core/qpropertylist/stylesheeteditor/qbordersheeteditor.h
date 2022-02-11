#ifndef QBORDERSHEETEDITOR_H
#define QBORDERSHEETEDITOR_H

#include "qbaseeditorwidget.h"

namespace Ui
{
class QBorderSheetEditor;
}

class QBorderSheetEditor : public QBaseEditorWidget
{
    Q_OBJECT

public:
    explicit QBorderSheetEditor(QWidget *parent = 0);
    ~QBorderSheetEditor();

    void setItem(QAbstractStylesheetItem *item);
    void init(QAbstractStylesheetItem *item);

    void takeResource(QUndoCommand *cmd);
    void addResource(QUndoCommand *cmd);

protected:
    void same_color();

protected slots:
    void left_gradient();
    void top_gradient();
    void right_gradient();
    void bottom_gradient();
    void left_color();
    void top_color();
    void right_color();
    void bottom_color();

private:
    Ui::QBorderSheetEditor *ui;

protected:
    QString m_tempResource;
    QString m_start_resource;

private slots:
    void on_same_color_clicked();
    void on_image_clicked();
    void left_width_valueChanged(int arg1);
    void top_width_valueChanged(int arg1);
    void right_width_valueChanged(int arg1);
    void bottom_width_valueChanged(int arg1);
    void on_same_width_clicked();
    void left_style_currentIndexChanged(int index);
    void top_style_currentIndexChanged(int index);
    void right_style_currentIndexChanged(int index);
    void bottom_style_currentIndexChanged(int index);
    void on_same_style_clicked();
};

#endif // QBORDERSHEETEDITOR_H
