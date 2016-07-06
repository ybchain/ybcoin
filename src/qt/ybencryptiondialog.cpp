#include "ybencryptiondialog.h"
#include "ybmessagedialogtitle.h"
#include "ybpushbutton.h"
#include "guiconstants.h"
#include "walletmodel.h"

#include <QMessageBox>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QFont>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QApplication>
#include <QDialog>

YbEncryptionDialog::YbEncryptionDialog(Mode mode, QWidget *parent) :
    mode(mode),
    model(0),
    fCapsLock(false),
    QDialog(parent)
{
    createWidget();

    encryption->setMaxLength(MAX_PASSPHRASE_SIZE);
    newEncryption->setMaxLength(MAX_PASSPHRASE_SIZE);
    reNewEncryption->setMaxLength(MAX_PASSPHRASE_SIZE);

    // Setup Caps Lock detection.
    encryption->installEventFilter(this);
    newEncryption->installEventFilter(this);
    reNewEncryption->installEventFilter(this);
//    ui->capsLabel->clear();

    switch(mode)
    {
        case Encrypt: // Ask passphrase x2
            setWindowTitle(tr("Encrypt wallet"));
            warningLabel->setText(tr("Enter the new passphrase to the wallet.<br/>Please use a passphrase of <b>10 or more random characters</b>, or <b>eight or more words</b>."));
            encryptionText->hide();
            encryption->hide();
            newEncryption->setFocus();
            break;
        case Unlock: // Ask passphrase
            setWindowTitle(tr("Unlock wallet"));
            warningLabel->setText(tr("This operation needs your wallet passphrase to unlock the wallet."));
            newEncryptionText->hide();
            newEncryption->hide();
            reNewEncryptionText->hide();
            reNewEncryption->hide();
            encryption->setFocus();
            break;
        case Decrypt:   // Ask passphrase
            setWindowTitle(tr("Decrypt wallet"));
            warningLabel->setText(tr("This operation needs your wallet passphrase to decrypt the wallet."));
            newEncryptionText->hide();
            newEncryption->hide();
            reNewEncryptionText->hide();
            reNewEncryption->hide();
            encryption->setFocus();
            break;
        case ChangePass: // Ask old passphrase + new passphrase x2
            setWindowTitle(tr("Change passphrase"));
            warningLabel->setText(tr("Enter the old and new passphrase to the wallet."));
            encryption->setFocus();
            break;
    }

    textChanged();
    connect(encryption, SIGNAL(textChanged(QString)), this, SLOT(textChanged()));
    connect(newEncryption, SIGNAL(textChanged(QString)), this, SLOT(textChanged()));
    connect(reNewEncryption, SIGNAL(textChanged(QString)), this, SLOT(textChanged()));
}

YbEncryptionDialog::~YbEncryptionDialog()
{
    if(title == NULL){
        delete title;
    }
    if(buttonBar == NULL){
        delete buttonBar;
    }
    if(warningLabel == NULL){
        delete warningLabel;
    }
    if(encryptionText == NULL){
        delete encryptionText;
    }
    if(newEncryptionText == NULL){
        delete newEncryptionText;
    }
    if(reNewEncryptionText == NULL){
        delete reNewEncryptionText;
    }
    if(encryption == NULL){
        delete encryption;
    }
    if(newEncryption == NULL){
        delete newEncryption;
    }
    if(reNewEncryption == NULL){
        delete reNewEncryption;
    }
}

void YbEncryptionDialog::accept()
{
    SecureString oldpass, newpass1, newpass2;
    if(!model)
        return;
    oldpass.reserve(MAX_PASSPHRASE_SIZE);
    newpass1.reserve(MAX_PASSPHRASE_SIZE);
    newpass2.reserve(MAX_PASSPHRASE_SIZE);
    // TODO: get rid of this .c_str() by implementing SecureString::operator=(std::string)
    // Alternately, find a way to make this input mlock()'d to begin with.
    oldpass.assign(encryption->text().toStdString().c_str());
    newpass1.assign(newEncryption->text().toStdString().c_str());
    newpass2.assign(reNewEncryption->text().toStdString().c_str());

    switch(mode)
    {
    case Encrypt: {
        if(newpass1.empty() || newpass2.empty())
        {
            // Cannot encrypt with empty passphrase
            break;
        }
        QMessageBox::StandardButton retval = QMessageBox::question(this, tr("Confirm wallet encryption"),
                 tr("WARNING: If you encrypt your wallet and lose your passphrase, you will <b>LOSE ALL OF YOUR ybcoin</b>!\nAre you sure you wish to encrypt your wallet?"),
                 QMessageBox::Yes|QMessageBox::Cancel,
                 QMessageBox::Cancel);
        if(retval == QMessageBox::Yes)
        {
            if(newpass1 == newpass2)
            {
                if(model->setWalletEncrypted(true, newpass1))
                {
                    QMessageBox::warning(this, tr("Wallet encrypted"),
                                         tr("ybcoin will close now to finish the encryption process. Remember that encrypting your wallet cannot fully protect your ybcoin from being stolen by malware infecting your computer."));
                    QApplication::quit();
                }
                else
                {
                    QMessageBox::critical(this, tr("Wallet encryption failed"),
                                         tr("Wallet encryption failed due to an internal error. Your wallet was not encrypted."));
                }
                QDialog::accept(); // Success
            }
            else
            {
                QMessageBox::critical(this, tr("Wallet encryption failed"),
                                     tr("The supplied passphrases do not match."));
            }
        }
        else
        {
            QDialog::reject(); // Cancelled
        }
        } break;
    case Unlock:
        if(!model->setWalletLocked(false, oldpass))
        {
            QMessageBox::critical(this, tr("Wallet unlock failed"),
                                  tr("The passphrase entered for the wallet decryption was incorrect."));
        }
        else
        {
            QDialog::accept(); // Success
        }
        break;
    case Decrypt:
        if(!model->setWalletEncrypted(false, oldpass))
        {
            QMessageBox::critical(this, tr("Wallet decryption failed"),
                                  tr("The passphrase entered for the wallet decryption was incorrect."));
        }
        else
        {
            QDialog::accept(); // Success
        }
        break;
    case ChangePass:
        if(newpass1 == newpass2)
        {
            if(model->changePassphrase(oldpass, newpass1))
            {
                QMessageBox::information(this, tr("Wallet encrypted"),
                                     tr("Wallet passphrase was succesfully changed."));
                QDialog::accept(); // Success
            }
            else
            {
                QMessageBox::critical(this, tr("Wallet encryption failed"),
                                     tr("The passphrase entered for the wallet decryption was incorrect."));
            }
        }
        else
        {
            QMessageBox::critical(this, tr("Wallet encryption failed"),
                                 tr("The supplied passphrases do not match."));
        }
        break;
    }
}

void YbEncryptionDialog::closeDlg()
{
    QDialog::reject();
}

void YbEncryptionDialog::setModel(WalletModel *model)
{
    this->model = model;
}

void YbEncryptionDialog::createWidget()
{
    this->setMinimumHeight(250);
    this->setMinimumWidth(500);
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(255, 255, 255));
    this->setPalette(pa);
    setStyleSheet("QLineEdit{border: 2px groove rgb(211, 211, 211)} QPlainTextEdit{border: 2px groove rgb(211, 211, 211)}");

    QFont boldFont;
    boldFont.setBold(true);

    QString titleText(tr("加密钱包"));
    title = new YbMessageDialogTitle(titleText, this);

    buttonBar = new EncryptionButtonBar(this);
    connect(buttonBar, SIGNAL(ok()), this, SLOT(accept()));
    connect(buttonBar, SIGNAL(cancel()), this, SLOT(closeDlg()));
    buttonBar->okButton->setDefault(true);

    warningLabel = new QLabel(this);
    QHBoxLayout *warningLayout = new QHBoxLayout;
    warningLayout->addSpacing(20);
    warningLayout->addWidget(warningLabel);
    warningLayout->addSpacing(20);

    encryptionText = new QLabel(this);
    encryptionText->setText(tr("输入口令："));
    encryptionText->setFont(boldFont);

    newEncryptionText = new QLabel(this);
    newEncryptionText->setText(tr("新口令："));
    newEncryptionText->setFont(boldFont);

    reNewEncryptionText = new QLabel(this);
    reNewEncryptionText->setText(tr("重复新口令："));
    reNewEncryptionText->setFont(boldFont);

    encryption = new QLineEdit(this);
    encryption->setEchoMode(QLineEdit::Password);
    newEncryption = new QLineEdit(this);
    newEncryption->setEchoMode(QLineEdit::Password);
    reNewEncryption = new QLineEdit(this);
    reNewEncryption->setEchoMode(QLineEdit::Password);

    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->addSpacing(20);
    hlayout1->addWidget(encryptionText);
    hlayout1->addWidget(encryption);
    hlayout1->addSpacing(20);

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->addSpacing(20);
    hlayout2->addWidget(newEncryptionText);
    hlayout2->addWidget(newEncryption);
    hlayout2->addSpacing(20);

    QHBoxLayout *hlayout3 = new QHBoxLayout;
    hlayout3->addSpacing(20);
    hlayout3->addWidget(reNewEncryptionText);
    hlayout3->addWidget(reNewEncryption);
    hlayout3->addSpacing(20);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addWidget(title);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addLayout(warningLayout);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addSpacing(10);
    mainLayout->addLayout(hlayout1);
    mainLayout->addLayout(hlayout2);
    mainLayout->addLayout(hlayout3);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(buttonBar);

    setLayout(mainLayout);
}

void YbEncryptionDialog::textChanged()
{
    // Validate input, set Ok button to enabled when accepable
    bool acceptable = false;
    switch(mode)
    {
    case Encrypt: // New passphrase x2
        acceptable = !newEncryption->text().isEmpty() && !reNewEncryption->text().isEmpty();
        break;
    case Unlock: // Old passphrase x1
    case Decrypt:
        acceptable = !encryption->text().isEmpty();
        break;
    case ChangePass: // Old passphrase x1, new passphrase x2
        acceptable = !encryption->text().isEmpty() && !newEncryption->text().isEmpty() && !reNewEncryption->text().isEmpty();
        break;
    }
    buttonBar->okButton->setEnabled(acceptable);
}

bool YbEncryptionDialog::event(QEvent *event)
{
    // Detect Caps Lock key press.
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_CapsLock) {
            fCapsLock = !fCapsLock;
        }
//        if (fCapsLock) {
//            ui->capsLabel->setText(tr("Warning: The Caps Lock key is on."));
//        } else {
//            ui->capsLabel->clear();
//        }
    }
    return QWidget::event(event);
}

bool YbEncryptionDialog::eventFilter(QObject *, QEvent *event)
{
    /* Detect Caps Lock.
     * There is no good OS-independent way to check a key state in Qt, but we
     * can detect Caps Lock by checking for the following condition:
     * Shift key is down and the result is a lower case character, or
     * Shift key is not down and the result is an upper case character.
     */
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        QString str = ke->text();
        if (str.length() != 0) {
            const QChar *psz = str.unicode();
            bool fShift = (ke->modifiers() & Qt::ShiftModifier) != 0;
            if ((fShift && psz->isLower()) || (!fShift && psz->isUpper())) {
                fCapsLock = true;
                //ui->capsLabel->setText(tr("Warning: The Caps Lock key is on."));
            } else if (psz->isLetter()) {
                fCapsLock = false;
//                ui->capsLabel->clear();
            }
        }
    }
    return false;
}


EncryptionButtonBar::EncryptionButtonBar(QWidget *parent)
    : QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(224, 238, 238));
    this->setPalette(pa);

    setMinimumHeight(60);
    setMinimumWidth(500);

    closeButton = new YbPushButton(tr("取消"), 60, 30, false, this);
    okButton = new YbPushButton(tr("完成"), 60, 30, true, this);

    connect(okButton, SIGNAL(clicked()), this, SIGNAL(ok()));
    connect(closeButton, SIGNAL(clicked()), this, SIGNAL(cancel()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(closeButton);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    mainLayout->addWidget(okButton);

    setLayout(mainLayout);
}

EncryptionButtonBar::~EncryptionButtonBar()
{
    if(closeButton == NULL){
        delete closeButton;
    }
    if(okButton == NULL){
        delete okButton;
    }
}
