#include "qresourceview.h"
#include "qresourceshowwidget.h"
#include "../../qfilelistview.h"
#include "../../minisplitter.h"
#include "../../qfilemanager.h"
#include <QVBoxLayout>

QResourceView::QResourceView(QWidget *parent) :
    QDialog(parent),
    m_fileView(new QFileListView(false, true, false)),
    m_showWidget(new QResourceShowWidget),
    m_fileInfo(NULL),
    m_ret(0)
{
    MiniSplitter *s = new MiniSplitter;
    s->addWidget(m_fileView);
    s->addWidget(m_showWidget);

    QVBoxLayout *v = new QVBoxLayout;
    v->setMargin(0);
    v->setSpacing(0);
    v->addWidget(s);

    this->setLayout(v);
    this->resize(200, 300);

    connect(m_fileView, SIGNAL(sigSelectFile(tagFileInfo*)),
            this, SLOT(select(tagFileInfo*)));

    connect(m_showWidget, SIGNAL(cancel()), this, SLOT(close()));
    connect(m_showWidget, SIGNAL(ok()), this, SLOT(ok()));
}

QResourceView::~QResourceView()
{
}

void QResourceView::select(tagFileInfo *file)
{
    m_fileInfo = file;
    m_showWidget->setImage(file->m_file_data);
}

void QResourceView::setFile(const QByteArray &data)
{
    m_fileInfo = NULL;
    m_showWidget->setImage(data);
}

int QResourceView::getRet()
{
    return m_ret;
}

tagFileInfo *QResourceView::getFile()
{
    return m_fileInfo;
}

void QResourceView::ok()
{
    m_ret = 1;
    close();
}
