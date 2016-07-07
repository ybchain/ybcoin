#include "ybsendsigndialog.h"
#include "ybmessagedialogtitle.h"
#include "qvalidatedlineedit.h"
#include "ybpushbutton.h"
#include "guiutil.h"
#include "walletmodel.h"
#include "main.h"
#include "wallet.h"
#include "init.h"
#include "util.h"
#include "ybaddressbookpage.h"

#include <QLineEdit>
#include <QPlainTextEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>
#include <string>
#include <vector>
#include <QClipboard>
#include <QInputDialog>
#include <QList>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QApplication>
#include <QClipboard>

YbSendSignDialog::YbSendSignDialog(QWidget *parent) :
    QWidget(parent)
{
    createWidget();
    GUIUtil::setupAddressWidget(signFrom, this);
}

void YbSendSignDialog::setModel(WalletModel *model)
{
    this->model = model;
}

void YbSendSignDialog::setAddress(QString addr)
{
    signFrom->setText(addr);
    message->setFocus();
}

void YbSendSignDialog::paste()
{
    setAddress(QApplication::clipboard()->text());
}

void YbSendSignDialog::showAddr()
{
    YbAddressBookPage dlg(YbAddressBookPage::ForSending, YbAddressBookPage::ReceivingTab, this);
    dlg.setModel(model->getAddressTableModel());
    if(dlg.exec())
    {
        setAddress(dlg.getReturnValue());
    }
}

void YbSendSignDialog::close()
{
    QWidget::close();
}

void YbSendSignDialog::copyToClipboard()
{
    QApplication::clipboard()->setText(signature->text());
}

void YbSendSignDialog::signMessage()
{
    QString address = signFrom->text();

    CBitcoinAddress addr(address.toStdString());
    if (!addr.IsValid())
    {
        QMessageBox::critical(this, tr("Error signing"), tr("%1 is not a valid address.").arg(address),
                              QMessageBox::Abort, QMessageBox::Abort);
        return;
    }

    WalletModel::UnlockContext ctx(model->requestUnlock());
    if(!ctx.isValid())
    {
        // Unlock wallet was cancelled
        return;
    }

    CKey key;
    if (!pwalletMain->GetKey(addr, key))
    {
        QMessageBox::critical(this, tr("Error signing"), tr("Private key for %1 is not available.").arg(address),
                              QMessageBox::Abort, QMessageBox::Abort);
        return;
    }

    CDataStream ss(SER_GETHASH, 0);
    ss << strMessageMagic;
    ss << message->document()->toPlainText().toStdString();

    std::vector<unsigned char> vchSig;
    if (!key.SignCompact(Hash(ss.begin(), ss.end()), vchSig))
    {
        QMessageBox::critical(this, tr("Error signing"), tr("Sign failed"),
                              QMessageBox::Abort, QMessageBox::Abort);
    }

    signature->setText(QString::fromStdString(EncodeBase64(&vchSig[0], vchSig.size())));
    signature->setFont(GUIUtil::bitcoinAddressFont());
}

void YbSendSignDialog::createWidget()
{
    setMinimumHeight(400);
    setMinimumWidth(600);
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(255, 255, 255));
    this->setPalette(pa);
    setStyleSheet("QLineEdit{border: 2px groove rgb(211, 211, 211)} QPlainTextEdit{border: 2px groove rgb(211, 211, 211)}");

    QFont boldFont;
    boldFont.setBold(true);

    QString titleInfoStr(tr("您可以用您的地址对消息进行签名，以证明您是该地址的所有人，注意不要对模棱两可的消息签名，以免遭受钓鱼式的攻击。请确保消息真实明确的表达您的意愿。"));
    QPixmap sendPix(":icons/sendindialog");
    QString titleStr(tr("发送签名消息"));
    title = new YbMessageDialogTitle(sendPix, titleStr, titleInfoStr);

    QPixmap pastePix(":icons/paste");
    pasteButton = new YbPushButton(pastePix);
    connect(pasteButton, SIGNAL(clicked()), this, SLOT(paste()));

    QPixmap addrPix(":icons/addr");
    addrButton = new YbPushButton(addrPix);
    connect(addrButton, SIGNAL(clicked()), this, SLOT(showAddr()));

    QPixmap signPix(":icons/copysign");
    addrSignButton = new YbPushButton(signPix);
    connect(addrSignButton, SIGNAL(clicked()), this, SLOT(copyToClipboard()));

    signFrom = new QValidatedLineEdit(this);
    signature = new QLineEdit;
    message = new QPlainTextEdit;
    signature->setReadOnly(true);
    signature->setText(tr("点击\"消息签名\"获取签名"));

    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->setSpacing(0);
    QLabel *addrText = new QLabel(tr("地址："));
    addrText->setFont(boldFont);
    hlayout1->addSpacing(25);
    hlayout1->addWidget(addrText);
    hlayout1->addSpacing(25);
    hlayout1->addWidget(signFrom);
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
    hlayout2->addWidget(message);
    hlayout2->addSpacing(25);

    QHBoxLayout *hlayout3 = new QHBoxLayout;
    hlayout3->setSpacing(0);
    QLabel *addrSignText = new QLabel(tr("地址签名："));
    addrSignText->setFont(boldFont);
    hlayout3->addSpacing(25);
    hlayout3->addWidget(addrSignText);
    hlayout3->addWidget(signature);
    hlayout3->addWidget(addrSignButton);
    hlayout3->addSpacing(25);

    buttonBar = new SendSignButtonBar(this);
    connect(buttonBar, SIGNAL(closeSignDlg()), this, SLOT(close()));
    connect(buttonBar, SIGNAL(sign()), this, SLOT(signMessage()));

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


SendSignButtonBar::SendSignButtonBar(QWidget *parent)
{
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(224, 238, 238));
    this->setPalette(pa);

    setMinimumHeight(60);
    setMinimumWidth(600);

    closeButton = new YbPushButton(tr("关闭"), 60, 30, false, this);
    signButton = new YbPushButton(tr("消息签名"), 60, 30, true, this);
    connect(closeButton, SIGNAL(clicked()), this, SIGNAL(closeSignDlg()));
    connect(signButton, SIGNAL(clicked()), this, SIGNAL(sign()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(closeButton);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    mainLayout->addWidget(signButton);

    setLayout(mainLayout);
}
