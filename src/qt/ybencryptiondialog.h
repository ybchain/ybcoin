#ifndef YBENCRYPTIONDIALOG_H
#define YBENCRYPTIONDIALOG_H

#include <QWidget>

class YbPushButton;
class QLabel;
class QLineEdit;
class YbMessageDialogTitle;

class EncryptionButtonBar : public QWidget
{
    Q_OBJECT
public:
    explicit EncryptionButtonBar(QWidget *parent = 0);

signals:
    void close();
    void ok();

private:
    YbPushButton *closeButton;
    YbPushButton *okButton;
};

class YbEncryptionDialog : public QWidget
{
    Q_OBJECT
public:
    explicit YbEncryptionDialog(QWidget *parent = 0);

private:
    YbMessageDialogTitle *title;
    EncryptionButtonBar *buttonBar;
    QLabel *encryptionText;
    QLabel *newEncryptionText;
    QLabel *reNewEncryptionText;
    QLineEdit *encryption;
    QLineEdit *newEncryption;
    QLineEdit *reNewEncryption;
};


#endif // YBENCRYPTIONDIALOG_H
