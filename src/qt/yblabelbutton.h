#ifndef YBLABELBUTTON_H
#define YBLABELBUTTON_H

#include <QLabel>

class YbLabelButton : public QLabel
{
    Q_OBJECT
public:
    explicit YbLabelButton(QPixmap pix, QWidget *parent = 0);

protected:
    void mouseReleaseEvent(QMouseEvent *e);

signals:
    void labelButtonClicked();
};

#endif // YBLABELBUTTON_H
