#ifndef YBSENDCOINSDIALOG_H
#define YBSENDCOINSDIALOG_H

#include <QWidget>
#include <QList>

class YbMessageDialogTitle;
class QVBoxLayout;
class QHBoxLayout;
class YbPushButton;
class QLineEdit;
class QDoubleSpinBox;
class QComboBox;
class QLabel;
class QFont;


class SendCoinsButtonBar : public QWidget
{
    Q_OBJECT
public:
    explicit SendCoinsButtonBar(QWidget *parent = 0);
    ~SendCoinsButtonBar();

signals:
    void cancel();
    void clear();
    void send();

private:
    YbPushButton *cancelButton;
    YbPushButton *clearButton;
    YbPushButton *sendButton;
};

class Receiver : public QWidget
{
    Q_OBJECT
public:
    explicit Receiver(QWidget *parent = 0);
    ~Receiver();
    void setIsDrawBorder(bool is);
    void setCloseButtonVisible(bool is);
    void setAddButtonVisible(bool is);
    void clear();

protected:
    void paintEvent(QPaintEvent *e);

signals:
    void addReceiver();
    void closeReceiver();

private:
    QFont boldFont;
    bool isDrawBorder;
    YbPushButton *closeButton;
    YbPushButton *addButton;
    QLineEdit *payTo;
    QDoubleSpinBox *money;
    QComboBox *unit;

    void drawBorderRect();
};

class YbSendCoinsDialog : public QWidget
{
    Q_OBJECT
public:
    explicit YbSendCoinsDialog(QWidget *parent = 0);
    ~YbSendCoinsDialog();

private slots:
    void addReceiver();
    void closeReceiver();

    void cancel();
    void clear();
    void send();

private:
    QFont boldFont;
    YbMessageDialogTitle *title;
    SendCoinsButtonBar *buttonBar;
    QLineEdit *addAsLabel;
    QVBoxLayout *receiverLayout;
    QList<Receiver *> receiverList;
};

#endif // YBSENDCOINSDIALOG_H
