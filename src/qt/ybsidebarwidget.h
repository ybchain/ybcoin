#ifndef YBSIDEBARWIDGET_H
#define YBSIDEBARWIDGET_H

#include <QWidget>
#include <QList>

class YbToolButton;
class QVBoxLayout;
class QSpacerItem;
class QColor;
class QLabel;

class YbSideBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit YbSideBarWidget(QWidget *parent = 0);
    void setTitle(const QString &title, const int &height = 60, const int &width = 250);
    void addToolButton(YbToolButton *button);
    void addSpacerItem(QSpacerItem *spacerItem);

signals:

private:
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QList<YbToolButton *> buttonList;

private slots:
    void oneButtonClicked();
};

#endif // YBSIDEBARWIDGET_H
