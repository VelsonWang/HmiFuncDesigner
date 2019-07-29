#include "QFramelessDockWidget.h"
#include <QHBoxLayout>
#include <QDebug>

QFramelessDockWidget::QFramelessDockWidget(QWidget *parent)
    : QDockWidget(parent)
{
    m_titleWidget = new QWidget(this);
    m_titleWidget->setObjectName(QStringLiteral("titleWidget"));
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(m_titleWidget->sizePolicy().hasHeightForWidth());
    m_titleWidget->setSizePolicy(sizePolicy);

    m_titleWidgetHLayout = new QHBoxLayout(m_titleWidget);
    m_titleWidgetHLayout->setSpacing(0);
    m_titleWidgetHLayout->setObjectName(QStringLiteral("titleWidgetHLayout"));
    m_titleWidgetHLayout->setContentsMargins(0, 0, 0, 0);
    m_titleLabel = new QLabel(m_titleWidget);
    m_titleLabel->setObjectName(QStringLiteral("titleLabel"));
    m_titleLabel->setText(tr("仪器控制"));
    m_titleWidgetHLayout->addWidget(m_titleLabel);

    m_hSpacer = new QSpacerItem(106, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_titleWidgetHLayout->addItem(m_hSpacer);

    m_btnHideOrShow = new QPushButton(m_titleWidget);
    m_btnHideOrShow->setObjectName(QStringLiteral("btnHideOrShow"));
    m_btnHideOrShow->setText(tr(">>"));
    m_btnHideOrShow->setMaximumWidth(20);
    connect(m_btnHideOrShow, SIGNAL(pressed()), this, SLOT(onBtnHideOrShowClick()));
    m_titleWidgetHLayout->addWidget(m_btnHideOrShow);

    this->setTitleBarWidget(m_titleWidget);
}

QFramelessDockWidget::~QFramelessDockWidget()
{
    //delete m_titleWidgetHLayout;
    //delete m_titleLabel;
    //delete m_hSpacer;
    //delete m_btnHideOrShow;
    delete m_titleWidget;
}


void QFramelessDockWidget::onBtnHideOrShowClick()
{
    if(m_btnHideOrShow->text() == tr(">>"))
    {
        m_btnHideOrShow->setText(tr("<<"));
        m_titleLabel->hide();
        this->setMinimumWidth(0);
        this->resize(20, this->size().height());
    }
    else
    {
        m_btnHideOrShow->setText(tr(">>"));
        m_titleLabel->show();
        this->setMinimumWidth(240);
        this->resize(240, this->size().height());
    }
}
