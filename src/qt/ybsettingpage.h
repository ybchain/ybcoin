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

    void createWidget();

    void setButton1Enabled(bool is);
    void setButton2Enabled(bool is);
    void setButton3Enabled(bool is);

    void setButton1Checked(bool is);
    void setButton2Checked(bool is);

signals:
    void button1Clicked(bool);
    void button2Clicked(bool);
    void button3Clicked();

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
