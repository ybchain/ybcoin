#ifndef YBTOOLBUTTON_H
#define YBTOOLBUTTON_H

#include <QWidget>

class QPixmap;
class QLabel;

class YbToolButton : public QWidget
{
    Q_OBJECT
public:
    explicit YbToolButton(QPixmap pixmap, QPixmap checkedPixmap, QString text, QWidget *parent = 0, bool isCheckable = true);
    void setChecked(bool isChecked);
    void setDefaultState();

signals:
    void toolClicked();

protected:
    void paintEvent(QPaintEvent *e);
    void enterEvent(QEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void leaveEvent(QEvent *e);

private:
    QLabel *m_picLabel;
    QLabel *m_textLabel;
    QPixmap m_pixmap;
    QPixmap m_checkedPixmap;
    bool m_isChecked;
    bool m_isCheckable;
    QFont normalFont;
    QFont boldFont;

    void setBkPalette(int r, int g, int b, int a = 255);
};

#endif // YBTOOLBUTTON_H
