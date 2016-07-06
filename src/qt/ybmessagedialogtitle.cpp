#include "ybmessagedialogtitle.h"

#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>

YbMessageDialogTitle::YbMessageDialogTitle(QPixmap pixmap, QString title, QString titleInfo, QWidget *parent) :
    QWidget(parent)
{
    this->title = new QLabel;
    this->titleInfo = new QLabel;
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(224, 238, 238));
    this->setPalette(pa);

    setMinimumHeight(85);
    setMinimumWidth(500);

    boldFont.setBold(true);

    this->title->setStyleSheet("font-size:20px;");
    this->title->setFont(boldFont);
    this->title->setText(title);
    this->titleInfo->setText(titleInfo);

    QLabel *pixLabel = new QLabel;
    pixLabel->setPixmap(pixmap);
    pixLabel->setMinimumSize(pixmap.size());
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->addSpacing(10);
    titleLayout->addWidget(pixLabel);
    titleLayout->addWidget(this->title);
    titleLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QHBoxLayout *infoLayout = new QHBoxLayout;
    infoLayout->addSpacing(10);
    infoLayout->addWidget(this->titleInfo);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(titleLayout);
    mainLayout->addLayout(infoLayout);

    setLayout(mainLayout);
}

YbMessageDialogTitle::YbMessageDialogTitle(QPixmap pixmap, QString title, QWidget *parent)
{
    this->title = new QLabel;
    this->titleInfo = NULL;
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(224, 238, 238));
    this->setPalette(pa);

    setMinimumHeight(60);
    setMinimumWidth(500);

    boldFont.setBold(true);

    this->title->setStyleSheet("font-size:20px;");
    this->title->setFont(boldFont);
    this->title->setText(title);

    QLabel *pixLabel = new QLabel;
    pixLabel->setPixmap(pixmap);
    pixLabel->setMinimumSize(pixmap.size());
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->addSpacing(10);
    titleLayout->addWidget(pixLabel);
    titleLayout->addWidget(this->title);
    titleLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(titleLayout);

    setLayout(mainLayout);
}

YbMessageDialogTitle::YbMessageDialogTitle(QString title, QWidget *parent) :
    QWidget(parent)
{
    this->title = new QLabel;
    this->titleInfo = NULL;
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(224, 238, 238));
    this->setPalette(pa);

    setMinimumHeight(60);
    setMinimumWidth(500);

    boldFont.setBold(true);

    this->title->setStyleSheet("font-size:20px;");
    this->title->setFont(boldFont);
    this->title->setText(title);

    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->addWidget(this->title);
    titleLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(titleLayout);

    setLayout(mainLayout);
}

YbMessageDialogTitle::~YbMessageDialogTitle()
{
    if(title != NULL){
        delete title;
        title = NULL;
    }
    if(titleInfo != NULL){
        delete titleInfo;
        titleInfo = NULL;
    }
}

void YbMessageDialogTitle::setTitle(const QString &title)
{
    this->title->setText(title);
}

void YbMessageDialogTitle::setTitleInfo(const QString &titleInfo)
{
    this->titleInfo->setText(titleInfo);
}
