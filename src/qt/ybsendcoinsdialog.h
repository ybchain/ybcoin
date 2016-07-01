#ifndef YBSENDCOINSDIALOG_H
#define YBSENDCOINSDIALOG_H

#include <QWidget>
#include <QList>

class WalletModel;
class SendCoinsRecipient;

class YbMessageDialogTitle;
class ReceiversForm;

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QHBoxLayout;
class YbPushButton;
class QLineEdit;
class QLabel;
class QFont;
class QUrl;
class QValidatedLineEdit;
QT_END_NAMESPACE

class SendCoinsButtonBar : public QWidget
{
    Q_OBJECT
public:
    explicit SendCoinsButtonBar(QWidget *parent = 0);
    ~SendCoinsButtonBar();

    QLabel *labelBalance;
    YbPushButton *cancelButton;
    YbPushButton *deleteButton;
    YbPushButton *clearButton;
    YbPushButton *sendButton;

signals:
    void cancel();
    void deleteAll();
    void clear();
    void send();

private:
    WalletModel *model;
    bool fNewRecipientAllowed;
};

class YbSendCoinsDialog : public QWidget
{
    Q_OBJECT
public:
    explicit YbSendCoinsDialog(QWidget *parent = 0);
    ~YbSendCoinsDialog();

    void setModel(WalletModel *model);

    /** Set up the tab chain manually, as Qt messes up the tab chain by default in some cases (issue http://bugreports.qt.nokia.com/browse/QTBUG-10907).
     */
    QWidget *setupTabChain(QWidget *prev);

    void handleURI(const QString &uri);

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void cancel();
    void deleteAll();
    void clear();
    void send();
    void getAddAsLabel(QString &str);
    void setAddAsLabel(const QString &str);

public slots:
    void setBalance(qint64 balance, qint64 stake, qint64 unconfirmedBalance);

private:
    void createWidget();

    WalletModel *model;
    QFont boldFont;
    ReceiversForm *receiversForm;
    YbMessageDialogTitle *title;
    SendCoinsButtonBar *buttonBar;
    QValidatedLineEdit *addAsLabel;
};

#endif // YBSENDCOINSDIALOG_H
