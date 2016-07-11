#include "receiversform.h"
#include "ui_receiversform.h"
#include "qvalidatedlineedit.h"
#include "guiutil.h"
#include "bitcoinunits.h"
#include "addressbookpage.h"
#include "walletmodel.h"
#include "optionsmodel.h"
#include "addresstablemodel.h"
#include "bitcoinamountfield.h"
#include "yblabelbutton.h"

#include <QApplication>
#include <QClipboard>
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
#include <QCoreApplication>
#include <QScrollBar>
#include <QMessageBox>
#include <QLocale>
#include <QTextDocument>

Receiver::Receiver(QWidget *parent) :
    isDrawBorder(true), payTo(new QValidatedLineEdit(this)), payAmount(new BitcoinAmountField(this)),
    model(0), QWidget(parent)
{
    createWidget();

#if QT_VERSION >= 0x040700
    payTo->setPlaceholderText(tr("输入地址"));
#endif
    setFocusPolicy(Qt::TabFocus);
    setFocusProxy(payTo);

    GUIUtil::setupAddressWidget(payTo, this);
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
    if(payAmount != NULL){
        delete payAmount;
        payAmount = NULL;
    }
}

void Receiver::setModel(WalletModel *model)
{
    this->model = model;
    clear();
}

bool Receiver::validate()
{
    // Check input validity
    bool retval = true;

    if(payAmount->validate())
    {
        retval = false;
    }
    else
    {
        if(payAmount->value() <= 0)
        {
            // Cannot send 0 coins or less
            payAmount->setValid(false);
            retval = false;
        }
    }

    if(!payTo->hasAcceptableInput() ||
       (model && !model->validateAddress(payTo->text())))
    {
        payTo->setValid(false);
        retval = false;
    }

    return retval;
}

SendCoinsRecipient Receiver::getValue()
{
    SendCoinsRecipient rv;
    QString label;
    emit addAsLabel(label);

    rv.address = payTo->text();
    rv.label = label;
    rv.amount = payAmount->value();

    return rv;
}

bool Receiver::isClear()
{
    return payTo->text().isEmpty();
}

void Receiver::setValue(const SendCoinsRecipient &value)
{
    payTo->setText(value.address);
    emit setAddAsLabel(value.label);
    payAmount->setValue(value.amount);
}

QWidget *Receiver::setupTabChain(QWidget *prev)
{
    QWidget::setTabOrder(prev, payTo);
    return payAmount->setupTabChain(payTo);
}

void Receiver::setFocus()
{
    payTo->setFocus();
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
    payAmount->clear();
    payTo->setFocus();
    if(model && model->getOptionsModel())
    {
        payAmount->setDisplayUnit(model->getOptionsModel()->getDisplayUnit());
    }
}

void Receiver::on_payTo_textChanged(const QString &address)
{
    if(!model)
        return;
    // Fill in label from address book, if address has an associated label
    QString associatedLabel = model->getAddressTableModel()->labelForAddress(address);
    if(!associatedLabel.isEmpty())
        emit setAddAsLabel(associatedLabel);
}

void Receiver::paintEvent(QPaintEvent *e)
{
    drawBorderRect();
}

void Receiver::createWidget()
{
    this->setMinimumHeight(150);
    this->setMinimumWidth(500);
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(255, 255, 255));
    this->setPalette(pa);

    setStyleSheet("QLineEdit{border: 2px groove rgb(211, 211, 211)} QDoubleSpinBox{border: 2px groove rgb(211, 211, 211);} QComboBox{border: 2px groove rgb(211, 211, 211);}");

    payTo->setToolTip(tr("输入地址"));
    payTo->setMinimumHeight(25);
    payTo->setMinimumWidth(370);

    QPixmap closePix(":icons/closesend");
    closeButton = new YbLabelButton(closePix);
    QHBoxLayout *closeLayout = new QHBoxLayout;
    closeLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    closeLayout->addWidget(closeButton);
//    closeLayout->addSpacing(1);
    connect(closeButton, SIGNAL(labelButtonClicked()), this, SIGNAL(closeReceiver()));

    QPixmap addPix(":icons/addsend");
    addButton = new YbLabelButton(addPix);
    QHBoxLayout *addLayout = new QHBoxLayout;
    addLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    addLayout->addWidget(addButton);
    addLayout->addSpacing(10);
    connect(addButton, SIGNAL(labelButtonClicked()), this, SIGNAL(addReceiver()));

    QLabel *receiverLabel = new QLabel(tr("receiver:"));

    QHBoxLayout *payToLayout = new QHBoxLayout;
    payToLayout->addSpacing(20);
    payToLayout->addWidget(receiverLabel);
    //payToLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    payToLayout->addWidget(payTo);
    payToLayout->addSpacing(33);

    payAmount->setMinimumHeight(25);
    payAmount->setMinimumWidth(370);
    QHBoxLayout *moneyLayout = new QHBoxLayout;
    //moneyLayout->addSpacing();
    QLabel *moneyText = new QLabel(tr("money:"));
    moneyLayout->addSpacing(20);
    moneyLayout->addWidget(moneyText);
    moneyLayout->addSpacing(12);
    moneyLayout->addWidget(payAmount);
    moneyLayout->addSpacing(40);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(closeLayout);
    mainLayout->addLayout(payToLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(moneyLayout);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(addLayout);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

    setLayout(mainLayout);
}

void Receiver::drawBorderRect()
{
    if(isDrawBorder){
        QPainter painter(this);
        QRect rect;
        QPen pen;

        pen.setColor(QColor(211, 211, 211));
        rect.setTopLeft(QPoint(10, 20));
        rect.setBottomRight(QPoint(this->width() - 15, this->height() - 20));
        painter.setPen(pen);
        painter.drawRect(rect);
    }
}


ReceiversForm::ReceiversForm(QWidget *parent) :
    QWidget(parent),
    model(0),
    ui(new Ui::ReceiversForm)
{
    createWidget();

    if(receiverList.isEmpty()){
        addReceiver();
    }

    fNewRecipientAllowed = true;
}

ReceiversForm::~ReceiversForm()
{
    delete ui;
    while(receiverList.count()){
        delete receiverList.takeAt(0);
    }
    if(layout != NULL){
        delete layout;
        layout = NULL;
    }
}

void ReceiversForm::setModel(WalletModel *model)
{
    this->model = model;

    for(int i = 0; i < receiverList.count(); ++i)
    {
        if(receiverList.at(i))
        {
            receiverList.at(i)->setModel(model);
        }
    }
}

void ReceiversForm::clear()
{
    foreach(Receiver *receiver, receiverList){
        if(receiver != NULL){
            receiver->clear();
        }
    }
}

void ReceiversForm::pasteEntry(const SendCoinsRecipient &rv)
{
    if (!fNewRecipientAllowed)
        return;

    Receiver *receiver = 0;
    // Replace the first entry if it is still unused
    if(receiverList.count() == 1)
    {
        Receiver *first = receiverList.at(0);
        if(first->isClear())
        {
            receiver = first;
        }
    }
    if(!receiver)
    {
        receiver = addReceiver();
    }

    receiver->setValue(rv);
}

void ReceiversForm::handleURI(const QString &uri)
{
    SendCoinsRecipient rv;
    if(!GUIUtil::parseBitcoinURI(uri, &rv))
    {
        return;
    }
    pasteEntry(rv);
}

void ReceiversForm::deleteAll()
{
    // Remove entries until only one left
    while(receiverList.count())
    {
        delete receiverList.takeAt(0);
    }
    addReceiver();
}

void ReceiversForm::send()
{
    QList<SendCoinsRecipient> recipients;
    bool valid = true;

    if(!model)
        return;

    for(int i = 0; i < receiverList.count(); ++i)
    {
        Receiver *receiver = receiverList.at(i);
        if(receiver)
        {
            if(receiver->validate())
            {
                recipients.append(receiver->getValue());
            }
            else
            {
                valid = false;
            }
        }
    }

    if(!valid || recipients.isEmpty())
    {
        return;
    }

    // Format confirmation message
    QStringList formatted;
    foreach(const SendCoinsRecipient &rcp, recipients)
    {
#if  QT_VERSION < 0x050000
        formatted.append(tr("<b>%1</b> to %2 (%3)").arg(BitcoinUnits::formatWithUnit(BitcoinUnits::BTC, rcp.amount), Qt::escape(rcp.label), rcp.address));
#else
        formatted.append(tr("<b>%1</b> to %2 (%3)").arg(BitcoinUnits::formatWithUnit(BitcoinUnits::BTC, rcp.amount), rcp.label.toHtmlEscaped(), rcp.address));
#endif
    }

    fNewRecipientAllowed = false;

    QMessageBox::StandardButton retval = QMessageBox::question(this, tr("Confirm send shares"),
                          tr("Are you sure you want to send %1?").arg(formatted.join(tr(" and "))),
          QMessageBox::Yes|QMessageBox::Cancel,
          QMessageBox::Cancel);

    if(retval != QMessageBox::Yes)
    {
        fNewRecipientAllowed = true;
        return;
    }

    WalletModel::UnlockContext ctx(model->requestUnlock());
    if(!ctx.isValid())
    {
        // Unlock wallet was cancelled
        fNewRecipientAllowed = true;
        return;
    }

    WalletModel::SendCoinsReturn sendstatus = model->sendCoins(recipients);
    switch(sendstatus.status)
    {
    case WalletModel::InvalidAddress:
        QMessageBox::warning(this, tr("Send Shares"),
            tr("The recepient address is not valid, please recheck."),
            QMessageBox::Ok, QMessageBox::Ok);
        break;
    case WalletModel::InvalidAmount:
        QMessageBox::warning(this, tr("Send Shares"),
            tr("The amount to pay must be at least one cent (0.01)."),
            QMessageBox::Ok, QMessageBox::Ok);
        break;
    case WalletModel::AmountExceedsBalance:
        QMessageBox::warning(this, tr("Send Shares"),
            tr("Amount exceeds your balance"),
            QMessageBox::Ok, QMessageBox::Ok);
        break;
    case WalletModel::AmountWithFeeExceedsBalance:
        QMessageBox::warning(this, tr("Send Shares"),
            tr("Total exceeds your balance when the %1 transaction fee is included").
            arg(BitcoinUnits::formatWithUnit(BitcoinUnits::BTC, sendstatus.fee)),
            QMessageBox::Ok, QMessageBox::Ok);
        break;
    case WalletModel::DuplicateAddress:
        QMessageBox::warning(this, tr("Send Shares"),
            tr("Duplicate address found, can only send to each address once in one send operation"),
            QMessageBox::Ok, QMessageBox::Ok);
        break;
    case WalletModel::TransactionCreationFailed:
        QMessageBox::warning(this, tr("Send Shares"),
            tr("Error: Transaction creation failed  "),
            QMessageBox::Ok, QMessageBox::Ok);
        break;
    case WalletModel::TransactionCommitFailed:
        QMessageBox::warning(this, tr("Send Shares"),
            tr("Error: The transaction was rejected.  This might happen if some of the shares in your portfolio were already spent, such as if you used a copy of wallet.dat and shares were spent in the copy but not marked as spent here."),
            QMessageBox::Ok, QMessageBox::Ok);
        break;
    case WalletModel::Aborted: // User aborted, nothing to do
        break;
    case WalletModel::OK:
        break;
    }
    fNewRecipientAllowed = true;
}

Receiver *ReceiversForm::addReceiver()
{
    Receiver *receiver = new Receiver(this);
    receiver->setModel(model);
    connect(receiver, SIGNAL(addReceiver()), this, SLOT(addReceiver()));
    connect(receiver, SIGNAL(closeReceiver()), this, SLOT(closeReceiver()));
    connect(receiver, SIGNAL(addAsLabel(QString&)), this, SIGNAL(addAsLabel(QString&)));
    connect(receiver, SIGNAL(setAddAsLabel(QString)), this, SIGNAL(setAddAsLabel(QString)));
    if(!receiverList.isEmpty()){
        receiver->setCloseButtonVisible(true);
    }else{
        receiver->setCloseButtonVisible(false);
    }

    layout->addWidget(receiver);
    receiverList.append(receiver);

    for(int i = receiverList.count() - 2; i >= 0; --i){
        receiverList.at(i)->setAddButtonVisible(false);
    }
    if(receiverList.count() > 1){
        receiverList.first()->setCloseButtonVisible(true);
    }
    ui->scrollAreaWidgetContents->resize(ui->scrollAreaWidgetContents->sizeHint());
    QCoreApplication::instance()->processEvents();
    QScrollBar* bar = ui->scrollArea->verticalScrollBar();
    if (bar)
        bar->setSliderPosition(bar->maximum());
    receiver->setFocus();
    return receiver;
}

void ReceiversForm::closeReceiver()
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

void ReceiversForm::createWidget()
{
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(255, 255, 255));
    this->setPalette(pa);
    ui->setupUi(this);
    ui->widget->setPalette(pa);
    ui->widget->setStyleSheet("QWidget{background-color: rgb(255, 255, 255)}");
    layout = new QVBoxLayout;
    layout->setMargin(0);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layout);
    mainLayout->addSpacerItem(new QSpacerItem(20, 16777212, QSizePolicy::Minimum, QSizePolicy::Maximum));
    ui->widget->setLayout(mainLayout);
}
