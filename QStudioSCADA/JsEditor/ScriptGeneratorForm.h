#ifndef SCRIPTGENERATORFORM_H
#define SCRIPTGENERATORFORM_H

#include <QWidget>

namespace Ui {
class ScriptGeneratorForm;
}

class ScriptGeneratorForm : public QWidget
{
    Q_OBJECT

public:
    explicit ScriptGeneratorForm(QWidget *parent = 0);
    ~ScriptGeneratorForm();

private:
    Ui::ScriptGeneratorForm *ui;
};

#endif // SCRIPTGENERATORFORM_H
