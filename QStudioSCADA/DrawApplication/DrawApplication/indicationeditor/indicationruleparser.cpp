#include "indicationruleparser.h"
#include <QColor>
#include <QtDebug>

IndicationRuleParser::IndicationRuleParser(QObject *parent) :
    QObject(parent)
{
    errorState = trUtf8("No suitable rule for this state");
}

bool IndicationRuleParser::setRule(const QString &mRule) {

    ruleStr = mRule;

    if (ruleStr.isEmpty()) {
        return false;
    }

    parseRule();
    return true;
}

QString IndicationRuleParser::getErrorStateMessage() const {
    return errorState;
}

void IndicationRuleParser::parseRule() {

    QStringList expressions = getExpressionsList();

    qDebug() << expressions.size();
    foreach (QString s,expressions) {
        qDebug() << "NEXT EXPRESSION: "<<s;
        parseExpression(s);
    }
}

void IndicationRuleParser::parseExpression(const QString &expression) {

    QStringList ruleTargetList = getRuleTargetList(expression);
    QString stateName = getStateName(ruleTargetList.first()).trimmed();
    qDebug() << "PARSING" << stateName;
    createFeature(stateName,ruleTargetList[1]);
}

void IndicationRuleParser::createFeature(const QString &state, const QString &ruleAttributes) {

    QStringList attributesList = ruleAttributes.split(trUtf8(","),QString::SkipEmptyParts,Qt::CaseInsensitive);
    Features *feature = new Features;

    foreach (QString s,attributesList) {

        QStringList tokenList = s.split(trUtf8("="),QString::SkipEmptyParts,Qt::CaseInsensitive);

        if (tokenList.first().trimmed() == trUtf8("background")) {
            feature->background = QColor(tokenList[1].trimmed());
            qDebug() << feature->background;
        }

        if (tokenList.first().trimmed() == trUtf8("text")) {
            feature->text = tokenList[1].trimmed();
            qDebug() << feature->text;
        }

        if (tokenList.first().trimmed() == trUtf8("bordercolor")) {
            feature->bordercolor = QColor(tokenList[1].trimmed());
            qDebug() << feature->bordercolor;
        }

        if (tokenList.first().trimmed() == trUtf8("signbackground")) {
            feature->signbackground = QColor(tokenList[1].trimmed());
            qDebug() << feature->signbackground;
        }
    }

    feature->state = state;

    ruleList.insert(ruleList.end(),feature);
}

QString IndicationRuleParser::getStateName(const QString &target) const {
    return target.split(trUtf8("="),QString::SkipEmptyParts,Qt::CaseInsensitive)[1];
}

QStringList IndicationRuleParser::getRuleTargetList(const QString &expression) const {
    return expression.split(trUtf8(":"),QString::SkipEmptyParts,Qt::CaseInsensitive);
}

QStringList IndicationRuleParser::getExpressionsList() const {
    return ruleStr.split(trUtf8(";"),QString::SkipEmptyParts,Qt::CaseInsensitive);
}

QList <Features*> IndicationRuleParser::getRuleList() const {
    return ruleList;
}
