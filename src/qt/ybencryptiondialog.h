#ifndef YBENCRYPTIONDIALOG_H
#define YBENCRYPTIONDIALOG_H

#include <QWidget>
#include <QDialog>

class WalletModel;
class YbPushButton;
class QLabel;
class QLineEdit;
class YbMessageDialogTitle;

class EncryptionButtonBar : public QWidget
{
    Q_OBJECT
public:
    explicit EncryptionButtonBar(QWidget *parent = 0);
    ~EncryptionButtonBar();

    YbPushButton *closeButton;
    YbPushButton *okButton;

signals:
    void ok();
    void cancel();
};

/** Multifunctional dialog to ask for passphrases. Used for encryption, unlocking, and changing the passphrase.
 */
class YbEncryptionDialog : public QDialog
{
    Q_OBJECT

public:
    enum Mode {
        Encrypt,    /**< Ask passphrase twice and encrypt */
        Unlock,     /**< Ask passphrase and unlock */
        ChangePass, /**< Ask old passphrase + new passphrase twice */
        Decrypt     /**< Ask passphrase and decrypt wallet */
    };

public:
    explicit YbEncryptionDialog(Mode mode, QWidget *parent = 0);
    ~YbEncryptionDialog();

    void setModel(WalletModel *model);

public slots:
    void accept();
    void closeDlg();

private:
    void createWidget();

    YbMessageDialogTitle *title;
    EncryptionButtonBar *buttonBar;
    QLabel *warningLabel;
    QLabel *encryptionText;
    QLabel *newEncryptionText;
    QLabel *reNewEncryptionText;
    QLineEdit *encryption;
    QLineEdit *newEncryption;
    QLineEdit *reNewEncryption;
    Mode mode;
    WalletModel *model;
    bool fCapsLock;

private slots:
    void textChanged();
    bool event(QEvent *event);
    bool eventFilter(QObject *, QEvent *event);
};


#endif // YBENCRYPTIONDIALOG_H
