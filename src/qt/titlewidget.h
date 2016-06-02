#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>

class QHBoxLayout;
class QLabel;
class PushButton;
class SysButton;
class QMenu;

class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = 0);

    void setMenu(QMenu *m);

signals:
    void showMin();
    void showMax();
    void showSkin();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

private:
    QPoint pressedPoint;
    bool isMove;

    PushButton *btnSkin;
    SysButton *btnMenuBar;
    SysButton *btnMin;
    SysButton *btnMax;
    SysButton *btnClose;
    QHBoxLayout *mainLayout;
    QLabel *versionText;
    QMenu *menu;
//    QPixmap pixmap;

    void createButtons();
    void createMainLayout();

private slots:
    void showMenu();
};

#endif // TITLEWIDGET_H
