#ifndef RECEIVERSFORM_H
#define RECEIVERSFORM_H

#include <QWidget>
#include <QList>

namespace Ui {
class ReceiversForm;
}

class WalletModel;
class SendCoinsRecipient;
class BitcoinAmountField;
class YbLabelButton;

QT_BEGIN_NAMESPACE
class QLineEdit;
class QDoubleSpinBox;
class QComboBox;
class QVBoxLayout;
class QValidatedLineEdit;
QT_END_NAMESPACE

/** A single entry in the dialog for sending bitcoins. */
class Receiver : public QWidget
{
    Q_OBJECT
public:
    explicit Receiver(QWidget *parent = 0);
    ~Receiver();

    void setModel(WalletModel *model);
    bool validate();
    SendCoinsRecipient getValue();

    /** Return whether the entry is still empty and unedited */
    bool isClear();

    void setValue(const SendCoinsRecipient &value);

    /** Set up the tab chain manually, as Qt messes up the tab chain by default in some cases (issue http://bugreports.qt.nokia.com/browse/QTBUG-10907).
     */
    QWidget *setupTabChain(QWidget *prev);

    void setFocus();

    void setIsDrawBorder(bool is);
    void setCloseButtonVisible(bool is);
    void setAddButtonVisible(bool is);

public slots:
    void clear();

private slots:
    void on_payTo_textChanged(const QString &address);

protected:
    void paintEvent(QPaintEvent *e);

signals:
    void addReceiver();
    void closeReceiver();

    void addAsLabel(QString &str);
    void setAddAsLabel(const QString &str);

private:
    void createWidget();

    bool isDrawBorder;
    YbLabelButton *closeButton;
    YbLabelButton *addButton;
    QValidatedLineEdit *payTo;
    BitcoinAmountField *payAmount;
    WalletModel *model;

    void drawBorderRect();
};

class ReceiversForm : public QWidget
{
    Q_OBJECT

public:
    explicit ReceiversForm(QWidget *parent = 0);
    ~ReceiversForm();

    void setModel(WalletModel *model);

    void clear();

    QList<Receiver *> receiverList;

    void pasteEntry(const SendCoinsRecipient &rv);
    void handleURI(const QString &uri);

    void deleteAll();
    void send();

signals:
    void addAsLabel(QString &str);
    void setAddAsLabel(const QString &str);

private slots:
    Receiver *addReceiver();
    void closeReceiver();

private:
    void createWidget();

    Ui::ReceiversForm *ui;
    QVBoxLayout *layout;

    WalletModel *model;
    bool fNewRecipientAllowed;
};

#endif // RECEIVERSFORM_H
