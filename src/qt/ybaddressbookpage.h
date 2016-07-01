#ifndef YBADDRESSBOOKPAGE_H
#define YBADDRESSBOOKPAGE_H

#include <QDialog>
class AddressTableModel;
class YbPushButton;

QT_BEGIN_NAMESPACE
class QTableView;
class QItemSelection;
class QSortFilterProxyModel;
class QMenu;
QT_END_NAMESPACE

/** Widget that shows a list of sending or receiving addresses.
  */
class YbAddressBookPage : public QDialog
{
    Q_OBJECT
public:
    enum Tabs {
        SendingTab = 0,
        ReceivingTab = 1
    };

    enum Mode {
        ForSending, /**< Open address book to pick address for sending */
        ForEditing  /**< Open address book for editing */
    };
    explicit YbAddressBookPage(Mode mode, Tabs tab, QWidget *parent = 0);
    ~YbAddressBookPage();

    void setModel(AddressTableModel *model);
    const QString &getReturnValue() const { return returnValue; }

public slots:
    void done(int retval);
    void exportClicked();

signals:
    void showSignDialog(QString str);

private:
    void createWidget();

    QTableView *tableView;
    YbPushButton *newAddressButton;
    YbPushButton *copyAddressButton;
    YbPushButton *deleteAddressButton;
    YbPushButton *showQRCodeButton;
    YbPushButton *signMessageButton;
    AddressTableModel *model;
    Mode mode;
    Tabs tab;
    QString returnValue;
    QSortFilterProxyModel *proxyModel;
    QMenu *contextMenu;
    QAction *deleteAction;

private slots:
    void deleteAddress();
    void newAddress();
    /** Copy address of currently selected address entry to clipboard */
    void copyAddress();
    void signMessage();
    void selectionChanged();
    void showQRCode();
    /** Spawn contextual menu (right mouse menu) for address book entry */
    void contextualMenu(const QPoint &point);

    /** Copy label of currently selected address entry to clipboard */
    void onCopyLabelAction();
    /** Edit currently selected address entry */
    void onEditAction();
};

#endif // YBADDRESSBOOKPAGE_H
