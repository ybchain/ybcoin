#ifndef YBADDRESSBOOKPAGE_H
#define YBADDRESSBOOKPAGE_H

#include <QWidget>

class YbPushButton;

class YbAddressBookPage : public QWidget
{
    Q_OBJECT
public:
    explicit YbAddressBookPage(QWidget *parent = 0);

private:
    YbPushButton *newAddressButton;

};

#endif // YBADDRESSBOOKPAGE_H
