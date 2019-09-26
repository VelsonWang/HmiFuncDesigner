#include "scriptedit.h"
#include "scriptpropertyeditordlg.h"
#include <QHBoxLayout>
#include <QToolButton>
#include <QFocusEvent>

ScriptEdit::ScriptEdit(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(0);
    theLabel_ = new QLabel(this);
    theLabel_->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    QToolButton *button = new QToolButton(this);
    button->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
    button->setText(QLatin1String("..."));
    layout->addWidget(theLabel_);
    layout->addWidget(button);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_InputMethodEnabled);
    connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
}

void ScriptEdit::buttonClicked()
{
    ScriptPropertyEditorDlg dlg(Q_NULLPTR, supportEvents_);
    dlg.setScript(script_);
    if(dlg.exec() == QDialog::Accepted) {
        QString script = dlg.getScript();
        setScript(script);
        emit scriptChanged(script);
    }
}

QString ScriptEdit::getScript() const
{
    return script_;
}


void ScriptEdit::setScript(const QString &script)
{
    QString szScriptTmp = script;
    script_ = script;
    theLabel_->setText(szScriptTmp.replace("\n", ""));
}

/**
 * @brief setSupportEvents
 * @details 设置支持的事件列表
 * @param events 事件列表
 */
void ScriptEdit::setSupportEvents(QStringList events)
{
    supportEvents_ = events;
}
