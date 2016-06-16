#ifndef YBTABWIDGET_H
#define YBTABWIDGET_H

#include <QWidget>

class YbTabButton;
class YbStackedWidget;

class YbTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit YbTabWidget(QWidget *leftWidget = NULL, QWidget *rightWidget = NULL, QWidget *parent = 0);

private slots:
    void leftTabButtonChecked(bool is);
    void rightTabButtonChecked(bool is);

private:
    YbTabButton *leftTabButton;
    YbTabButton *rightTabButton;
    YbStackedWidget *stackedWidget;
    QWidget *m_leftWidget;
    QWidget *m_rightWidget;
};

#endif // YBTABWIDGET_H
