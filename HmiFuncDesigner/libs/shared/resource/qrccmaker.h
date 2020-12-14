#ifndef QMAKERCC_H
#define QMAKERCC_H

#include <QString>
#include <QByteArray>
#include <QHash>

class ResourceFileInfo;
class QResourceFile;
class QRCCMaker
{
public:
    QRCCMaker();
    ~QRCCMaker();

    void    setResourceFile(QResourceFile* resourceFile);

    void    clear();

    void writeNumber2(quint16 number);
    void writeNumber4(quint32 number);
    void writeChar(char c) { m_out.append(c); }
    void writeByteArray(const QByteArray &);
    void write(const char *, int len);

    QByteArray rccData();
protected:
    void    addData(ResourceFileInfo *r, QString path, const QByteArray &a);
    bool    writeHeader();
    bool    writeDataBlobs();
    bool    writeDataNames();
    bool    writeDataStructure();
    bool    writeInitializer();
protected:
    ResourceFileInfo    *m_root;
    QResourceFile       *m_resourceFile;
    QByteArray m_out;
    int m_treeOffset;
    int m_namesOffset;
    int m_dataOffset;
};

#endif // QMAKERCC_H
