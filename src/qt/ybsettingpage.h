#ifndef YBSETTINGPAGE_H
#define YBSETTINGPAGE_H

#include <QWidget>

class YbPushButton;
class YbLabel;
class QLabel;

class YbSettingPage : public QWidget
{
    Q_OBJECT
public:
    explicit YbSettingPage(QWidget *parent = 0);

private:
    QLabel *qlabel1;
    QLabel *qlabel2;
    QLabel *qlabel3;
    QLabel *textLabel1;
    QLabel *textLabel2;
    QLabel *textLabel3;
    YbLabel *label1;
    YbLabel *label2;
    YbLabel *label3;
    YbLabel *warningLabel;
    YbPushButton *button1;
    YbPushButton *button2;
    YbPushButton *button3;
};

#endif // YBSETTINGPAGE_H
