#ifndef YBRECEIVECOINSDIALOG_H
#define YBRECEIVECOINSDIALOG_H

#include <QWidget>

class YbMessageDialogTitle;
class YbPushButton;
class QLineEdit;

class ReceiveCoinsButtonBar : public QWidget
{
    Q_OBJECT
public:
    explicit ReceiveCoinsButtonBar(QWidget *parent = 0);

signals:
    void sendSign();
    void cancel();
    void clear();

private:
    YbPushButton *sendSignButton;
    YbPushButton *cancelButton;
    YbPushButton *clearButton;
};

class YbReceiveCoinsDialog : public QWidget
{
    Q_OBJECT
public:
    explicit YbReceiveCoinsDialog(QWidget *parent = 0);

    void setLineEditFocus();


private slots:
    void paste();
    void clear();
    void sendSign();
    void cancel();

signals:
    void showSendSign(QString addr);

private:
    void createWidget();

    YbMessageDialogTitle *title;
    ReceiveCoinsButtonBar *buttonBar;
    YbPushButton *pasteButton;
    QLineEdit *signFrom;
};

#endif // YBRECEIVECOINSDIALOG_H
