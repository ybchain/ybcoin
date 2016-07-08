#ifndef YBSENDSIGNDIALOG_H
#define YBSENDSIGNDIALOG_H

#include <QDialog>

class WalletModel;
class YbMessageDialogTitle;
class YbPushButton;
class QValidatedLineEdit;

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPlainTextEdit;
QT_END_NAMESPACE

class SendSignButtonBar : public QWidget
{
    Q_OBJECT
public:
    explicit SendSignButtonBar(QWidget *parent = 0);

signals:
    void closeSignDlg();
    void sign();

private:
    YbPushButton *closeButton;
    YbPushButton *signButton;
};

class YbSendSignDialog : public QDialog
{
    Q_OBJECT
public:
    explicit YbSendSignDialog(QWidget *parent = 0);

    void setModel(WalletModel *model);

    void setAddress(QString addr);

private slots:
    void paste();
    void showAddr();

    void close();
    void copyToClipboard();
    void signMessage();

private:
    void createWidget();

    YbMessageDialogTitle *title;
    YbPushButton *pasteButton;
    YbPushButton *addrButton;
    YbPushButton *copySignButton;
    QValidatedLineEdit *signFrom;
    QLineEdit *signature;
    QPlainTextEdit *message;
    SendSignButtonBar *buttonBar;
    WalletModel *model;
};

#endif // YBSENDSIGNDIALOG_H
