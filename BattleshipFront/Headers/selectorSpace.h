#ifndef SELECTORSPACE_H
#define SELECTORSPACE_H

#include <QLabel>
#include <QWidget>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QTimer>

#include "draggableShip.h"

class SelectorSpace : public QWidget{
    Q_OBJECT

public:
    explicit SelectorSpace(QWidget *parent = nullptr);
    ~SelectorSpace();

    void rotateShip(QLabel *shipLabel);
    void setupShips();

protected:
    //void mousePressEvent(QMouseEvent *event) override;
    //void mouseMoveEvent(QMouseEvent *event) override;
    //void mouseReleaseEvent(QMouseEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;//talvez tirar o override

private:
    bool isHorizontal;
    bool isDragging;
    QPoint clickOffset;
    QPoint initialPosition;

    QGraphicsScene *scene;
    QPixmap originalPixmap;
    QPixmap originalShipPixmaps;
    QMap<QLabel*, bool> shipRotation;
};

#endif // SELECTORSPACE_H
