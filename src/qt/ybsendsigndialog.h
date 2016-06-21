#ifndef YBSENDSIGNDIALOG_H
#define YBSENDSIGNDIALOG_H

#include <QWidget>

class YbMessageDialogTitle;
class YbPushButton;
class QLineEdit;
class QPlainTextEdit;

class SendSignButtonBar : public QWidget
{
    Q_OBJECT
public:
    explicit SendSignButtonBar(QWidget *parent = 0);

signals:
    void back();
    void send();

private:
    YbPushButton *backButton;
    YbPushButton *sendButton;
};

class YbSendSignDialog : public QWidget
{
    Q_OBJECT
public:
    explicit YbSendSignDialog(QWidget *parent = 0);

private slots:
    void paste();
    void showAddr();
    void showSign();

private:
    YbMessageDialogTitle *title;
    YbPushButton *pasteButton;
    YbPushButton *addrButton;
    YbPushButton *addrSignButton;
    QLineEdit *addrEdit;
    QLineEdit *addrSignEdit;
    QPlainTextEdit *label;
    SendSignButtonBar *buttonBar;
};

#endif // YBSENDSIGNDIALOG_H
