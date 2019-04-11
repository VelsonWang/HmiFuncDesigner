#ifndef FONTPROPERTYEDITOR_H
#define FONTPROPERTYEDITOR_H

#include <QWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QLabel>
#include <QFont>
#include <QSpacerItem>

class FontPropertyEditor : public QWidget
{
    Q_OBJECT
public:
    explicit FontPropertyEditor(QWidget *parent = 0);

    QFont getFont() const;
    static QString getFontString(const QFont& font);

    bool eventFilter(QObject *obj, QEvent *ev);

signals:
    void dataChangedByUser(const QFont &getFont, FontPropertyEditor* editor);

public slots:
    void setFont(const QFont &font);

private slots:
    void onToolButtonClicked();

private:
    QFont font_;
    QToolButton* toolButton_;
    QLineEdit* lineEdit_;
    QLabel* textLabel_;
    QSpacerItem* spacer_;
};

#endif // FONTPROPERTYEDITOR_H
