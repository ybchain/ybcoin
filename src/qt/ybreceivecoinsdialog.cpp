#include "ybreceivecoinsdialog.h"
#include "ybmainwindow.h"
#include "ybmessagedialogtitle.h"
#include "ybpushbutton.h"

#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QFont>
#include <QApplication>
#include <QClipboard>

YbReceiveCoinsDialog::YbReceiveCoinsDialog(QWidget *parent) :
    QWidget(parent)
{
    createWidget();
}

void YbReceiveCoinsDialog::setLineEditFocus()
{
    signFrom->setFocus();
}

void YbReceiveCoinsDialog::paste()
{
    signFrom->setText(QApplication::clipboard()->text());
    signFrom->setFocus();
}

void YbReceiveCoinsDialog::clear()
{
    this->signFrom->clear();
    setLineEditFocus();
}

void YbReceiveCoinsDialog::sendSign()
{
    QString addr = signFrom->text();

    emit showSendSign(addr);
}

void YbReceiveCoinsDialog::cancel()
{
    close();
}

void YbReceiveCoinsDialog::createWidget()
{
    this->setMinimumHeight(300);
    this->setMinimumWidth(500);
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(255, 255, 255));
    this->setPalette(pa);
    QFont boldFont;
    boldFont.setBold(true);

    QPixmap sendPix(":icons/sendindialog");
    QString titleStr(tr("接收"));
    QString titleInfoStr(tr("已为您创建可以与他人共享的地址，您可以用其接收元宝币。"));
    title = new YbMessageDialogTitle(sendPix, titleStr, titleInfoStr, this);

    setStyleSheet("QLineEdit{border: 2px groove rgb(211, 211, 211)}");
    signFrom = new QLineEdit;

    QPixmap pastePix(":icons/paste");
    pasteButton = new YbPushButton(pastePix);
    connect(pasteButton, SIGNAL(clicked()), this, SLOT(paste()));

    QLabel *label = new QLabel(tr("复制分享地址"));
    label->setFont(boldFont);
    QHBoxLayout *hlayoutLabel = new QHBoxLayout;
    hlayoutLabel->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hlayoutLabel->addWidget(label);
    hlayoutLabel->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QHBoxLayout *hlayoutEdit = new QHBoxLayout;
    hlayoutEdit->setSpacing(0);
    hlayoutEdit->addSpacing(20);
    hlayoutEdit->addWidget(pasteButton);
    hlayoutEdit->addWidget(signFrom);
    hlayoutEdit->addSpacing(20);

    buttonBar = new ReceiveCoinsButtonBar;
    connect(buttonBar, SIGNAL(sendSign()), this, SLOT(sendSign()));
    connect(buttonBar, SIGNAL(cancel()), this, SLOT(cancel()));
    connect(buttonBar, SIGNAL(clear()), this, SLOT(clear()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addWidget(title);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(hlayoutLabel);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(hlayoutEdit);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(buttonBar);

    setLayout(mainLayout);
}


ReceiveCoinsButtonBar::ReceiveCoinsButtonBar(QWidget *parent)
{
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(224, 238, 238));
    this->setPalette(pa);

    setMinimumHeight(60);
    setMinimumWidth(500);

    sendSignButton = new YbPushButton(tr("发送签名消息"), 80, 30, true, this);
    cancelButton = new YbPushButton(tr("取消"), 60, 30, false, this);
    clearButton = new YbPushButton(tr("清空消息"), 60, 30, false, this);
    connect(sendSignButton, SIGNAL(clicked()), this, SIGNAL(sendSign()));
    connect(cancelButton, SIGNAL(clicked()), this, SIGNAL(cancel()));
    connect(clearButton, SIGNAL(clicked()), this, SIGNAL(clear()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    mainLayout->addWidget(cancelButton);
    mainLayout->addWidget(clearButton);
    mainLayout->addWidget(sendSignButton);

    setLayout(mainLayout);
}
