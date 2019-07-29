#include "ScriptGeneratorForm.h"
#include "ui_ScriptGeneratorForm.h"

ScriptGeneratorForm::ScriptGeneratorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScriptGeneratorForm)
{
    ui->setupUi(this);
}

ScriptGeneratorForm::~ScriptGeneratorForm()
{
    delete ui;
}
