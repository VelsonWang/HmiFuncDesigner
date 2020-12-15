#ifndef INFLECTION_P_H
#define INFLECTION_P_H

#include <QtCore/QString>

class InflectionExceptions
{
    public:
        InflectionExceptions();
        void clear();

        QString singular(const QString& word);
        QString plural(const QString& word);

    private:
        void word(const QString& singular,
                  const QString& plural = QString());
        void singularWord(const QString&, const QString&);
        void pluralWord(const QString&, const QString&);

        void rule(const QString&, const QString&, bool = false);
        void singularRule(const QString&, const QString&);
        void pluralRule(const QString&, const QString&);

        QRegExp singularizationRules() const;
        QRegExp pluralizationRules() const;

    private:
        typedef QHash<QString, QString> StringHash;
        StringHash m_singularOf;
        StringHash m_pluralOf;

        StringHash m_singularizationRules;
        StringHash m_pluralizationRules;
};

#endif



