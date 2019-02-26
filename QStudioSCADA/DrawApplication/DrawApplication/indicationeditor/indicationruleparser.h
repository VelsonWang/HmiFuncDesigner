#ifndef INDICATIONRULEPARSER_H
#define INDICATIONRULEPARSER_H

#include <QObject>
#include <QStringList>
#include "features.h"

class IndicationRuleParser : public QObject
{
    Q_OBJECT
public:
    explicit IndicationRuleParser(QObject *parent = 0);
    bool setRule(const QString &mRule);
    QString getErrorStateMessage() const;
    QList <Features*> getRuleList() const;

private:
    QString ruleStr;
    QString errorState;
    QList <Features*> ruleList;

    void parseRule();
    void parseExpression(const QString &expression);
    void createFeature(const QString &state, const QString &ruleAttributes);
    QString getStateName(const QString &target) const;
    QStringList getRuleTargetList(const QString &expression) const;
    QStringList getExpressionsList() const;
};

#endif // INDICATIONRULEPARSER_H
