#ifndef YBSEPARATER_H
#define YBSEPARATER_H

#include <QWidget>

class YbSeparater : public QWidget
{
    Q_OBJECT
public:
    explicit YbSeparater(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *e);

private:
    void drawLine();
};

#endif // YBSEPARATER_H
