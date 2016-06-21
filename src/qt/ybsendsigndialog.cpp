#include "ybsendsigndialog.h"
#include "ybmessagedialogtitle.h"
#include "ybpushbutton.h"

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>

YbSendSignDialog::YbSendSignDialog(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedHeight(400);
    this->setFixedWidth(500);
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(255, 255, 255));
    this->setPalette(pa);
    setStyleSheet("QLineEdit{border: 2px groove rgb(211, 211, 211)} QPlainTextEdit{border: 2px groove rgb(211, 211, 211)}");

    QFont boldFont;
    boldFont.setBold(true);

    QString titleInfoStr(tr("您可以用您的地址对消息进行签名，以证明您是该地址的所有人，注意不要对模棱两可\n的消息签名，以免遭受钓鱼式的攻击。请确保消息真实明确的表达您的意愿。"));
    QPixmap sendPix(":icons/sendindialog");
    QString titleStr(tr("发送签名消息"));
    title = new YbMessageDialogTitle(sendPix, titleStr, titleInfoStr);

    QPixmap pastePix(":icons/paste");
    pasteButton = new YbPushButton(pastePix);
    connect(pasteButton, SIGNAL(clicked()), this, SLOT(paste()));

    QPixmap addrPix(":icons/addr");
    addrButton = new YbPushButton(addrPix);
    connect(addrButton, SIGNAL(clicked()), this, SLOT(showAddr()));

    QPixmap signPix(":icons/sign");
    addrSignButton = new YbPushButton(signPix);
    connect(addrSignButton, SIGNAL(clicked()), this, SLOT(showSign()));

    addrEdit = new QLineEdit;
    addrSignEdit = new QLineEdit;
    label = new QPlainTextEdit;

    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->setSpacing(0);
    QLabel *addrText = new QLabel(tr("地址："));
    addrText->setFont(boldFont);
    hlayout1->addSpacing(25);
    hlayout1->addWidget(addrText);
    hlayout1->addSpacing(25);
    hlayout1->addWidget(addrEdit);
    hlayout1->addWidget(pasteButton);
    hlayout1->addWidget(addrButton);
    hlayout1->addSpacing(25);

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->setSpacing(0);
    QLabel *labelText = new QLabel(tr("标签："));
    labelText->setFont(boldFont);
    hlayout2->addSpacing(25);
    hlayout2->addWidget(labelText);
    hlayout2->addSpacing(25);
    hlayout2->addWidget(label);
    hlayout2->addSpacing(25);

    QHBoxLayout *hlayout3 = new QHBoxLayout;
    hlayout3->setSpacing(0);
    QLabel *addrSignText = new QLabel(tr("地址签名："));
    addrSignText->setFont(boldFont);
    hlayout3->addSpacing(25);
    hlayout3->addWidget(addrSignText);
    hlayout3->addWidget(addrSignEdit);
    hlayout3->addWidget(addrSignButton);
    hlayout3->addSpacing(25);

    buttonBar = new SendSignButtonBar(this);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addWidget(title);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(hlayout1);
    mainLayout->addLayout(hlayout2);
    mainLayout->addLayout(hlayout3);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(buttonBar);

    setLayout(mainLayout);
}

void YbSendSignDialog::paste()
{

}

void YbSendSignDialog::showAddr()
{

}

void YbSendSignDialog::showSign()
{

}


SendSignButtonBar::SendSignButtonBar(QWidget *parent)
{
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(224, 238, 238));
    this->setPalette(pa);

    setFixedHeight(60);
    setFixedWidth(500);

    backButton = new YbPushButton(tr("返回"), 60, 30, false, this);
    sendButton = new YbPushButton(tr("发送"), 60, 30, true, this);
    connect(backButton, SIGNAL(clicked()), this, SIGNAL(back()));
    connect(sendButton, SIGNAL(clicked()), this, SIGNAL(send()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(backButton);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    mainLayout->addWidget(sendButton);

    setLayout(mainLayout);
}
