#include "ybsendcoinsdialog.h"
#include "ybpushbutton.h"
#include "ybmessagedialogtitle.h"
#include "receiversform.h"
#include "qvalidatedlineedit.h"
#include "bitcoinunits.h"
#include "walletmodel.h"
#include "optionsmodel.h"

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
    addAsLabel(new QValidatedLineEdit(this)), buttonBar(new SendCoinsButtonBar), QWidget(parent)
{
    createWidget();

#if QT_VERSION >= 0x040700
    addAsLabel->setPlaceholderText(tr("输入标签"));
#endif
}

YbSendCoinsDialog::~YbSendCoinsDialog()
{
    if(receiversForm != NULL){
        delete receiversForm;
        receiversForm = NULL;
    }
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
}

void YbSendCoinsDialog::setModel(WalletModel *model)
{
    this->model = model;
    receiversForm->setModel(model);

    if(model)
    {
        setBalance(model->getBalance(), model->getStake(), model->getUnconfirmedBalance());
        connect(model, SIGNAL(balanceChanged(qint64, qint64, qint64)), this, SLOT(setBalance(qint64, qint64, qint64)));
    }
}

QWidget *YbSendCoinsDialog::setupTabChain(QWidget *prev)
{
    for(int i = 0; i < receiversForm->receiverList.count(); ++i)
    {
        if(receiversForm->receiverList.at(i))
        {
            prev = receiversForm->receiverList.at(i)->setupTabChain(prev);
        }
    }
    QWidget::setTabOrder(prev, buttonBar->cancelButton);
    QWidget::setTabOrder(buttonBar->cancelButton, buttonBar->clearButton);
    QWidget::setTabOrder(buttonBar->clearButton, buttonBar->sendButton);
    return buttonBar->sendButton;
}

void YbSendCoinsDialog::handleURI(const QString &uri)
{
    receiversForm->handleURI(uri);
}

void YbSendCoinsDialog::closeEvent(QCloseEvent *e)
{
    (void)e;
    cancel();
}

void YbSendCoinsDialog::cancel()
{
    deleteAll();
    close();
}

void YbSendCoinsDialog::deleteAll()
{
    receiversForm->deleteAll();
}

void YbSendCoinsDialog::clear()
{
    receiversForm->clear();
    addAsLabel->clear();
}

void YbSendCoinsDialog::send()
{
    receiversForm->send();
}

void YbSendCoinsDialog::getAddAsLabel(QString &str)
{
    str = addAsLabel->text();
}

void YbSendCoinsDialog::setAddAsLabel(const QString &str)
{
    addAsLabel->setText(str);
}

void YbSendCoinsDialog::setBalance(qint64 balance, qint64 stake, qint64 unconfirmedBalance)
{
    Q_UNUSED(stake);
    Q_UNUSED(unconfirmedBalance);
    if(!model || !model->getOptionsModel())
        return;

    int unit = model->getOptionsModel()->getDisplayUnit();
    buttonBar->labelBalance->setText(BitcoinUnits::formatWithUnit(unit, balance));
}

void YbSendCoinsDialog::createWidget()
{
    QPixmap sendPix(":icons/sendindialog");
    QString titleStr(tr("发送"));
    QString titleInfoStr(tr("立即向任意元宝币地址发送元宝币。"));
    title = new YbMessageDialogTitle(sendPix, titleStr, titleInfoStr, this);
    this->setMinimumHeight(570);
    this->setMinimumWidth(550);
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(255, 255, 255));
    setStyleSheet("QLineEdit{border: 2px groove rgb(211, 211, 211)} QScrollArea{border: hide}");
    this->setPalette(pa);

    connect(buttonBar, SIGNAL(cancel()), this, SLOT(cancel()));
    connect(buttonBar, SIGNAL(deleteAll()), this, SLOT(deleteAll()));
    connect(buttonBar, SIGNAL(clear()), this, SLOT(clear()));
    connect(buttonBar, SIGNAL(send()), this, SLOT(send()));

    receiversForm = new ReceiversForm(this);
    connect(receiversForm, SIGNAL(addAsLabel(QString&)), this, SLOT(getAddAsLabel(QString&)));

    boldFont.setBold(true);

    addAsLabel->setMinimumHeight(25);
    addAsLabel->setMinimumWidth(382);
    addAsLabel->setToolTip(tr("输入标签"));
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
    mainLayout->addWidget(receiversForm);
    mainLayout->addLayout(labelLayout);
    mainLayout->addWidget(buttonBar);

    setLayout(mainLayout);
}

SendCoinsButtonBar::SendCoinsButtonBar(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(224, 238, 238));
    this->setPalette(pa);

    setMinimumHeight(60);
    setMinimumWidth(500);

    QLabel *label = new QLabel(this);
    label->setText(tr("余额："));
    labelBalance = new QLabel(this);
    labelBalance->setText(tr("0.00 YBC"));
    QHBoxLayout *labelLayout = new QHBoxLayout;
    labelLayout->setMargin(0);
    labelLayout->addWidget(label);
    labelLayout->addWidget(labelBalance);

    cancelButton = new YbPushButton(tr("取消"), 60, 30, false, this);
    deleteButton = new YbPushButton(tr("删除全部"), 60, 30, false, this);
    clearButton = new YbPushButton(tr("清空内容"), 60, 30, false, this);
    sendButton = new YbPushButton(tr("发送"), 60, 30, true, this);

    connect(cancelButton, SIGNAL(clicked()), this, SIGNAL(cancel()));
    connect(deleteButton, SIGNAL(clicked()), this, SIGNAL(deleteAll()));
    connect(clearButton, SIGNAL(clicked()), this, SIGNAL(clear()));
    connect(sendButton, SIGNAL(clicked()), this, SIGNAL(send()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    mainLayout->addLayout(labelLayout);
    mainLayout->addWidget(cancelButton);
    mainLayout->addWidget(deleteButton);
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
