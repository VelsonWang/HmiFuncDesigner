#include "inflection.h"

#include <QtCore/QRegExp>
#include <QtCore/QStringList>

QString Utils::snakeCase(const QString& name)
{
    QString newName = name;
    if (newName.indexOf(QRegExp("^[A-Z]+$")) != -1)
        return newName.toLower();

    newName = newName.replace(QRegExp("([A-Z]+)([A-Z][a-z])"),"\\1_\\2");
    newName = newName.replace(QRegExp("([a-z])([A-Z])"), "\\1_\\2");
    return newName.toLower();
}

QString Utils::classify(const QString& name)
{
    InflectionExceptions e;
    QString cify = name;
    QStringList wordList;
    QStringList tmpList = cify.split(QChar('_'));
    Q_FOREACH(QString s, tmpList) {
        s = e.singular(s);
        s = capitalize(s);
        wordList << s;
    }

    cify = wordList.join("");
    
    return cify;
}

QString Utils::camelize(const QString& name)
{
    QString camel = name;
    QRegExp rx1("/.?");
    int i = camel.indexOf(rx1);
    while (i != -1) {
        i++;
        camel[i] = camel[i].toUpper();
        i = camel.indexOf(rx1, i);
    }

    camel[0] = camel[0].toUpper();
    i = camel.indexOf(QChar('_'));
    while (i != -1) {
        i++;
        camel[i] = camel[i].toUpper();
        i = camel.indexOf(QChar('_'), i);
    }
    camel.remove(QChar('_'));
    camel.replace("/", "::");

    return camel;
}

QString Utils::capitalize(const QString& s)
{
    QString str = s;
    if (str.isEmpty())
        return s;

    str = str.toLower();
    str[0] = str.at(0).toUpper();
    return str;
}

QString Utils::tableize(const QString& name)
{
    QString table = name;
    InflectionExceptions e;
    table = snakeCase(table);
    QStringList wordList;
    QStringList tmpList = table.split(QChar('_'));
    QString last = tmpList.last();
    last = e.plural(last);
    tmpList.removeLast();
    wordList = tmpList;
    wordList << last;
    table = wordList.join(QChar('_'));
    
    return table;
}



Utils::InflectionExceptions::InflectionExceptions()
{
    word("equipment");
    word("information");
    word("money");
    word("species");
    word("series");
    word("fish");
    word("sheep");
    word("moose");
    word("hovercraft");
    word("grass");
    word("rain");
    word("milk");
    word("rice");
    word("plurals");
    word("postgres");

    word("Swiss", "Swiss");
    word("life", "lives");
    word("wife", "wives");
    word("goose", "geese");
    word("criterion", "criteria");
    word("alias", "aliases");
    word("status", "statuses");
    word("axis", "axes");
    word("crisis", "crises");
    word("testis", "testes");
    word("potato", "potatoes");
    word("tomato", "tomatoes");
    word("buffalo", "buffaloes");
    word("torpedo", "torpedoes");
    word("quiz", "quizzes");
    word("matrix", "matrices");
    word("vertex", "vertices");
    word("index", "indices");
    word("ox", "oxen");
    word("mouse", "mice");
    word("louse", "lice");
    word("thesis", "theses");
    word("thief", "thieves");
    word("analysis", "analyses");
    word("erratum", "errata");
    word("phenomenon", "phenomena");
    word("octopus", "octopi");
    word("thesaurus", "thesauri");
    word("movie", "movies");
    word("cactus", "cacti");
    word("plus", "plusses");
    word("cross", "crosses");
    word("medium", "media");
    word("datum", "data");
    word("basis", "bases");
    word("diagnosis", "diagnoses");

    rule("person", "people", true);
    rule("shoe", "shoes", true);
    rule("hive", "hives", true);
    rule("man", "men", true);
    rule("child", "children", true);
    rule("news", "news", true);
    rule("rf", "rves");
    rule("af", "aves");
    rule("ero", "eroes");
    rule("man", "men");
    rule("ch", "ches");
    rule("sh", "shes");
    rule("ss", "sses");
    rule("ta", "tum");
    rule("ia", "ium");
    rule("ra", "rum");
    rule("ay", "ays");
    rule("ey", "eys");
    rule("oy", "oys");
    rule("uy", "uys");
    rule("y", "ies");
    rule("x", "xes");
    rule("lf", "lves");
    rule("ffe", "ffes");
    rule("afe", "aves");
    rule("ouse", "ouses");
    /*
     *more cases of words ending in -oses not being singularized properly
     *than cases of words ending in -osis
     */
    //rule("osis", "oses");
    rule("ox", "oxes");
    rule("us", "uses");
    rule("", "s");

    /*
     *One-way singular rules.
     */
    singularRule("of", "ofs"); //proof
    singularRule("o", "oes"); //hero, heroes
    singularRule("f", "ves");

    /*
     *# One-way plural rules.
     */
    pluralRule("s", "ses");
    pluralRule("ive", "ives"); // don't want to snag wife
    pluralRule("fe", "ves"); // don't want to snag perspectives

}

void Utils::InflectionExceptions::word(const QString& singular,
                                       const QString& plural)
{
    QString realPlural = plural.isEmpty() ? singular : plural;
    singularWord(singular, realPlural);
    pluralWord(realPlural, singular);
}

void Utils::InflectionExceptions::singularWord(const QString& singular,
                                               const QString& plural)
{
    m_singularOf[plural] = singular;
    m_singularOf[capitalize(plural)] = capitalize(singular);
}

void Utils::InflectionExceptions::pluralWord(const QString& plural,
                                             const QString& singular)
{
    m_pluralOf[singular] = plural;
    m_pluralOf[capitalize(singular)] = capitalize(plural);
}

QString Utils::InflectionExceptions::singular(const QString& word)
{
    QString result = word;
    if (!m_singularOf.value(result).isEmpty()) {
        return m_singularOf.value(result);
    } else {
        QRegExp rx = singularizationRules();
        int i = result.indexOf(rx);
        while (i != -1) {
            result.replace(i, rx.cap(0).length(), 
                           m_singularizationRules.value(rx.cap(0)));
            i = result.indexOf(rx);
        }
        m_singularOf[word] = result;
        return result;
    }
}

QString Utils::InflectionExceptions::plural(const QString& word)
{
    QString result = word;
    if (!m_pluralOf.value(result).isEmpty()) {
        return m_pluralOf.value(result);
    } else {
        QRegExp rx = pluralizationRules();
        int i = result.indexOf(rx);
        while (i != -1) {
            QString cap = rx.cap(0);
            if (cap.length() == 0) {
                result += QChar('s');
                break;
            } else {
                result.replace(i, cap.length(),
                               m_pluralizationRules.key(cap));
                break;
            }
        }
        m_pluralOf[word] = result;
        return result;
    }
}

void Utils::InflectionExceptions::rule(const QString& singular,
                                       const QString& plural,
                                       bool wholeWord)
{
    singularRule(singular, plural);
    pluralRule(singular, plural);
    if (wholeWord) {
        word(singular,plural);
    }
}

void Utils::InflectionExceptions::singularRule(const QString& singular,
                                                const QString& plural)
{
    m_singularizationRules[singular] = plural;
}

void Utils::InflectionExceptions::pluralRule(const QString& singular,
                                              const QString& plural)
{
    m_pluralizationRules[plural] = singular;
}

QRegExp Utils::InflectionExceptions::singularizationRules() const
{
    static QRegExp rx;
    if (rx.isEmpty()) {
        QString regExp = "(";
        QList<QString> regexList = m_singularizationRules.values();
        QStringList strList = static_cast<QStringList>(regexList);
        regExp += strList.join("|");
        regExp += ")$";
        rx.setPattern(regExp);
        rx.setCaseSensitivity(Qt::CaseInsensitive);
    }
    return rx;
}

QRegExp Utils::InflectionExceptions::pluralizationRules() const
{
    static QRegExp rx;
    if (rx.isEmpty()) {
        QString regExp = "(";
        QList<QString> regexList = m_pluralizationRules.values();
        QStringList strList = static_cast<QStringList>(regexList);
        regExp += strList.join("|");
        regExp += ")$";
        rx.setPattern(regExp);
        rx.setCaseSensitivity(Qt::CaseInsensitive);
    }
    return rx;
}


