#ifndef YBTOOLBARWIDGET_H
#define YBTOOLBARWIDGET_H

#include <QWidget>

class QHBoxLayout;
class QVBoxLayout;
class QLabel;
class QSpacerItem;
class QPushButton;
class YbSeparater;

class YbToolBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit YbToolBarWidget(QWidget *parent = 0);
    void addPushButton(QPushButton *pushButton);
    void addSpacerItem(QSpacerItem *spacerItem);

public slots:
    void setYbcNumber(const QString &str);

private:
    QVBoxLayout *wholeLayout;
    QHBoxLayout *mainLayout;
    QHBoxLayout *separaterLayout;
    QVBoxLayout *leftLayout;
    QHBoxLayout *pushButtonLayout;
    QVBoxLayout *rightLayout;
    QHBoxLayout *buttonLayout;
    QHBoxLayout *ybcLayout;
    QLabel *myWalletTextLabel;
    QLabel *ybcNumberLabel;
    YbSeparater *separater;

    QFont boldFont;
};

#endif // YBTOOLBARWIDGET_H
