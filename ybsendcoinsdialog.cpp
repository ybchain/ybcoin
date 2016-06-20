#include "ybsendcoinsdialog.h"
#include "ybpushbutton.h"

#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QPixmap>
#include <QScrollArea>

YbSendCoinsDialog::YbSendCoinsDialog(QWidget *parent) :
    addAsLabel(new QLineEdit), title(new SendCoinsTitle),
    buttonBar(new SendCoinsButtonBar), QWidget(parent)
{
    this->setFixedHeight(570);
    this->setFixedWidth(500);
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(255, 255, 255));
    this->setPalette(pa);

    connect(buttonBar, SIGNAL(cancel()), this, SLOT(cancel()));
    connect(buttonBar, SIGNAL(clear()), this, SLOT(clear()));
    connect(buttonBar, SIGNAL(send()), this, SLOT(send()));

    receiverLayout = new QVBoxLayout;
    boldFont.setBold(true);

    setStyleSheet("QLineEdit{border: 2px groove rgb(211, 211, 211)}");
    addAsLabel->setFixedHeight(25);
    addAsLabel->setFixedWidth(382);

    addAsLabel->setToolTip(tr("输入标签"));

    Receiver *receiver = new Receiver(this);
    connect(receiver, SIGNAL(addReceiver()), this, SLOT(addReceiver()));
    connect(receiver, SIGNAL(closeReceiver()), this, SLOT(closeReceiver()));
    receiver->setCloseButtonVisible(false);
    receiverLayout->addWidget(receiver);
    receiverList.append(receiver);

    QHBoxLayout *labelLayout = new QHBoxLayout;
    QLabel *labelText = new QLabel(tr("标签："));
    labelText->setFont(boldFont);
    labelLayout->addSpacing(20);
    labelLayout->addWidget(labelText);
    labelLayout->addWidget(addAsLabel);
    labelLayout->addSpacing(28);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addWidget(title);
    mainLayout->addLayout(receiverLayout);
    mainLayout->addLayout(labelLayout);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(buttonBar);

    setLayout(mainLayout);
}

YbSendCoinsDialog::~YbSendCoinsDialog()
{
    if(title != NULL){
        delete title;
        title = NULL;
    }
    if(buttonBar != NULL){
        delete buttonBar;
        buttonBar = NULL;
    }
    if(addAsLabel != NULL){
        delete addAsLabel;
        addAsLabel = NULL;
    }
    if(receiverLayout != NULL){
        delete receiverLayout;
        receiverLayout = NULL;
    }
    foreach(Receiver *receiver, receiverList){
        receiverList.removeOne(receiver);
        delete receiver;
        receiver = NULL;
    }
}

void YbSendCoinsDialog::addReceiver()
{
    Receiver *receiver = new Receiver(this);
    connect(receiver, SIGNAL(addReceiver()), this, SLOT(addReceiver()));
    connect(receiver, SIGNAL(closeReceiver()), this, SLOT(closeReceiver()));
    receiver->setCloseButtonVisible(true);
    if(receiverList.count() == 2){
        receiver->setAddButtonVisible(false);
    }
    receiverLayout->addWidget(receiver);
    receiverList.append(receiver);

    for(int i = receiverList.count() - 2; i >= 0; --i){
        receiverList.at(i)->setAddButtonVisible(false);
    }
    if(receiverList.count() > 1){
        receiverList.first()->setCloseButtonVisible(true);
    }
}

void YbSendCoinsDialog::closeReceiver()
{
    Receiver *receiver = qobject_cast<Receiver *>(sender());
    receiver->close();
    receiverList.removeOne(receiver);
    delete receiver;
    receiver = NULL;

    receiverList.last()->setAddButtonVisible(true);
    if(receiverList.count() == 1){
        receiverList.first()->setCloseButtonVisible(false);
    }
}

void YbSendCoinsDialog::cancel()
{
    close();
}

void YbSendCoinsDialog::clear()
{
    foreach(Receiver *receiver, receiverList){
        if(receiver != NULL){
            receiver->clear();
        }
    }
    addAsLabel->clear();
}

void YbSendCoinsDialog::send()
{

}

Receiver::Receiver(QWidget *parent) :
    isDrawBorder(true), payTo(new QLineEdit(this)), money(new QDoubleSpinBox(this)),
    unit(new QComboBox(this)), QWidget(parent)
{
    this->setFixedHeight(120);
    this->setFixedWidth(500);
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(255, 255, 255));
    this->setPalette(pa);

    boldFont.setBold(true);

    setStyleSheet("QLineEdit{border: 2px groove rgb(211, 211, 211)} QDoubleSpinBox{border: 2px groove rgb(211, 211, 211);} QComboBox{border: 2px groove rgb(211, 211, 211);}");

    payTo->setToolTip(tr("输入地址"));
    payTo->setFixedHeight(25);
    payTo->setFixedWidth(370);

    QPixmap closePix(":icons/close");
    closeButton = new YbPushButton(closePix);
    QHBoxLayout *closeLayout = new QHBoxLayout;
    closeLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    closeLayout->addWidget(closeButton);
    closeLayout->addSpacing(20);
    connect(closeButton, SIGNAL(clicked()), this, SIGNAL(closeReceiver()));

    QPixmap addPix(":icons/addsend");
    addButton = new YbPushButton(addPix);
    QHBoxLayout *addLayout = new QHBoxLayout;
    addLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    addLayout->addWidget(addButton);
    addLayout->addSpacing(35);
    connect(addButton, SIGNAL(clicked()), this, SIGNAL(addReceiver()));

    QHBoxLayout *payToLayout = new QHBoxLayout;
    payToLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    payToLayout->addWidget(payTo);
    payToLayout->addSpacing(33);

    money->setFixedHeight(25);
    unit->setFixedHeight(25);
    QHBoxLayout *moneyLayout = new QHBoxLayout;
    //moneyLayout->addSpacing();
    QLabel *moneyText = new QLabel(tr("金额："));
    moneyText->setFont(boldFont);
    moneyLayout->addSpacing(20);
    moneyLayout->addWidget(moneyText);
    moneyLayout->addWidget(money);
    moneyLayout->addWidget(unit);
    moneyLayout->addSpacing(40);

    QHBoxLayout *receiverTextLayout = new QHBoxLayout;
    QLabel *receiverLabel = new QLabel(tr("接收者："));
    receiverLabel->setFont(boldFont);
    receiverTextLayout->addSpacing(20);
    receiverTextLayout->addWidget(receiverLabel);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(closeLayout);
    mainLayout->addLayout(payToLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(moneyLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(addLayout);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QHBoxLayout *wholeLayout = new QHBoxLayout;
    wholeLayout->setMargin(0);
    wholeLayout->addLayout(receiverTextLayout);
    wholeLayout->addLayout(mainLayout);
    setLayout(wholeLayout);
}

Receiver::~Receiver()
{
    if(closeButton != NULL){
        delete closeButton;
        closeButton = NULL;
    }
    if(addButton != NULL){
        delete addButton;
        addButton = NULL;
    }
    if(payTo != NULL){
        delete payTo;
        payTo = NULL;
    }
    if(money != NULL){
        delete money;
        money = NULL;
    }
    if(unit != NULL){
        unit = NULL;
    }
}

void Receiver::setIsDrawBorder(bool is)
{
    isDrawBorder = is;
}

void Receiver::setCloseButtonVisible(bool is)
{
    closeButton->setVisible(is);
}

void Receiver::setAddButtonVisible(bool is)
{
    addButton->setVisible(is);
}

void Receiver::clear()
{
    payTo->clear();
}

void Receiver::paintEvent(QPaintEvent *e)
{
    drawBorderRect();
}

void Receiver::drawBorderRect()
{
    if(isDrawBorder){
        QPainter painter(this);
        QRect rect;
        QPen pen;

        pen.setColor(QColor(211, 211, 211));
        rect.setTopLeft(QPoint(90, 10));
        rect.setWidth(380);
        rect.setHeight(100);
        painter.setPen(pen);
        painter.drawRect(rect);
    }
}


SendCoinsTitle::SendCoinsTitle(QWidget *parent) :
    title(new QLabel), titleInfo(new QLabel), QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(224, 238, 238));
    this->setPalette(pa);

    setFixedHeight(85);
    setFixedWidth(500);

    boldFont.setBold(true);

    title->setStyleSheet("font-size:20px;");
    title->setFont(boldFont);
    title->setText(tr("发送"));
    titleInfo->setText(tr("立即向任意元宝币地址发送元宝币"));

    QPixmap sendPix(":icons/sendindialog");
    QLabel *pixLabel = new QLabel;
    pixLabel->setPixmap(sendPix);
    pixLabel->setFixedSize(sendPix.size());
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->addSpacing(10);
    titleLayout->addWidget(pixLabel);
    titleLayout->addWidget(title);
    titleLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QHBoxLayout *infoLayout = new QHBoxLayout;
    infoLayout->addSpacing(10);
    infoLayout->addWidget(titleInfo);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(titleLayout);
    mainLayout->addLayout(infoLayout);

    setLayout(mainLayout);
}

SendCoinsTitle::~SendCoinsTitle()
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

void SendCoinsTitle::setTitle(const QString &title)
{
    this->title->setText(title);
}


SendCoinsButtonBar::SendCoinsButtonBar(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(224, 238, 238));
    this->setPalette(pa);

    setFixedHeight(60);
    setFixedWidth(500);

    cancelButton = new YbPushButton(tr("取消"), 60, 30, false, this);
    clearButton = new YbPushButton(tr("清空内容"), 60, 30, false, this);
    sendButton = new YbPushButton(tr("发送"), 60, 30, true, this);

    connect(cancelButton, SIGNAL(clicked()), this, SIGNAL(cancel()));
    connect(clearButton, SIGNAL(clicked()), this, SIGNAL(clear()));
    connect(sendButton, SIGNAL(clicked()), this, SIGNAL(send()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    mainLayout->addWidget(cancelButton);
    mainLayout->addWidget(clearButton);
    mainLayout->addWidget(sendButton);

    setLayout(mainLayout);
}

SendCoinsButtonBar::~SendCoinsButtonBar()
{
    if(cancelButton != NULL){
        delete cancelButton;
        cancelButton = NULL;
    }
    if(clearButton != NULL){
        delete clearButton;
        clearButton = NULL;
    }
    if(sendButton != NULL){
        delete sendButton;
        sendButton = NULL;
    }
}
