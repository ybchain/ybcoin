#ifndef YBTABWIDGET_H
#define YBTABWIDGET_H

#include <QWidget>

class YbTabButton;
class StackedWidget;

class YbTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit YbTabWidget(QWidget *parent = 0);

private slots:
    void leftTabButtonChecked(bool is);
    void rightTabButtonChecked(bool is);

private:
    YbTabButton *leftTabButton;
    YbTabButton *rightTabButton;
    StackedWidget *stackedWidget;
};

#endif // YBTABWIDGET_H
