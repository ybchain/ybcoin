#ifndef YBTABWIDGET_H
#define YBTABWIDGET_H

#include <QWidget>

class YbTabButton;
class YbStackedWidget;
class YbConsole;
class YbInformation;

class YbTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit YbTabWidget(YbInformation *leftWidget = NULL, YbConsole *rightWidget = NULL, QWidget *parent = 0);

private slots:
    void leftTabButtonChecked(bool is);
    void rightTabButtonChecked(bool is);

private:
    YbTabButton *leftTabButton;
    YbTabButton *rightTabButton;
    YbStackedWidget *stackedWidget;
    YbInformation *m_leftWidget;
    YbConsole *m_rightWidget;
};

#endif // YBTABWIDGET_H
