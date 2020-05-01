#ifndef FAKEBOOK_H
#define FAKEBOOK_H

#include "qpersistantobject.h"

class FakeBook : public QPersistantObject
{
Q_OBJECT
Q_PROPERTY(QString author READ author WRITE setAuthor)
Q_PROPERTY(int yearPublished READ yearPublished WRITE setYearPublished)
public:
    Q_INVOKABLE FakeBook(QObject* parent = 0) : QPersistantObject(parent) {}
    virtual ~FakeBook() {}

    void setAuthor(const QString& a) { m_author = a; }
    QString author() const { return m_author; }

    void setYearPublished(int year) { m_yearPublished = year; }
    int yearPublished() const { return m_yearPublished; }

private:
    QString m_author;
    int m_yearPublished;
};

#endif
/* vim: set et sts=4 sw=4 ts=16 tw=78 : */

