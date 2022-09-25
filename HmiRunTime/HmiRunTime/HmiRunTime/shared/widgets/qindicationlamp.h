#ifndef QINDICATIONLAMP_H
#define QINDICATIONLAMP_H

#include <QLabel>
#include <QPaintEvent>
#include "realtimedb.h"
#include "iloader.h"

class QIndicationLamp : public QLabel, public ILoader
{
    Q_OBJECT
    Q_PROPERTY(QString tag READ getTag WRITE setTag)
    Q_PROPERTY(bool StateOnInitial READ getStateOnInitial WRITE setStateOnInitial)
    Q_PROPERTY(QString ResetImageFile READ getResetImageFile WRITE setResetImageFile)
    Q_PROPERTY(QString SetImageFile READ getSetImageFile WRITE setSetImageFile)
    Q_PROPERTY(bool NoScale READ isNoScale WRITE setNoScale)
    Q_PROPERTY(int BoardWidth READ getBoardWidth WRITE setBoardWidth)
    Q_PROPERTY(QColor BoardColor READ getBoardColor WRITE setBoardColor)

public:
    Q_INVOKABLE QIndicationLamp(QWidget *parent = nullptr);

    QString getTag();
    void setTag(const QString szName);

    bool getStateOnInitial();
    void setStateOnInitial(bool bState);

    QString getResetImageFile();
    void setResetImageFile(const QString szName);

    QString getSetImageFile();
    void setSetImageFile(const QString szName);

    bool isNoScale();
    void setNoScale(bool bNoScale);

    int getBoardWidth();
    void setBoardWidth(int iWidth);

    QColor getBoardColor();
    void setBoardColor(QColor color);

public:
    void fromObject(XMLObject* xml) override;

private:
    void setPropertyInner();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    QString m_szTag;
    bool m_bStateOnInitial;
    QString m_szResetImageFile;
    QString m_szSetImageFile;
    QImage m_resetImageObj;
    QImage m_setImageObj;
    bool m_bNoScale;
    int m_iBoardWidth;
    QColor m_boardColorObj;
    QString m_tagId;
    RunTimeTag *m_tag;
};

#endif // QINDICATIONLAMP_H
