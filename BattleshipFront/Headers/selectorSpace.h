#ifndef SELECTORSPACE_H
#define SELECTORSPACE_H

#include <QLabel>
#include <QWidget>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QTimer>
#include "ship.h"

class SelectorSpace : public QWidget{
    Q_OBJECT

public:
    explicit SelectorSpace(QWidget *parent = nullptr);
    ~SelectorSpace();

    void rotateShip(QLabel *shipLabel);
    void setupShips();
    void clearShips();
    int getSelectedShipSize();
    int getSelectedShipIndex();
    bool isHorizontal();
    void markShipAsPlaced(int shipIndex);
    bool isSelectedShipHorizontal() const;
    void clearSelectedShip();
    //bool isVertical();
    //QLabel* getSelectedShip();

protected:
    //void mousePressEvent(QMouseEvent *event) override;
    //void mouseMoveEvent(QMouseEvent *event) override;
    //void mouseReleaseEvent(QMouseEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;//talvez tirar o override

private:
    int selectedShipSize = -1;
    int selectedShipIndex = -1;
    bool isDragging;
    QPoint clickOffset;
    QPoint initialPosition;
    bool Horizontal;

    QLabel* selectedShipLabel = nullptr;

    //QLabel* selectedLabel = nullptr;

    //QPixmap originalPixmap;
    QMap<QLabel*, QPixmap> originalPixmaps;
    QPixmap originalShipPixmaps;
    QMap<QLabel*, bool> shipRotation;
};

#endif // SELECTORSPACE_H
