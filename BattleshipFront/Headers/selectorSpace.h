#ifndef SELECTORSPACE_H
#define SELECTORSPACE_H

#include <QLabel>
#include <QWidget>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>
#include "ship.h"

struct ShipInfo {
    QString texturePath;
    QSize size; //tamanho do barco para o front
    int shipSize; // tamanho do barco para o back-end
};

class SelectorSpace : public QWidget{
    Q_OBJECT

public:
    explicit SelectorSpace(QWidget *parent = nullptr);
    ~SelectorSpace();

    void rotateShip(QLabel *shipLabel);
    void setupShips();
    void clearShips();
    void clearSelectedShip();
    void restoreShip(Ship &ship);
    void markShipAsPlaced(int shipIndex);
    int getSelectedShipSize();
    int getSelectedShipIndex();
    bool isHorizontal();
    bool isSelectedShipHorizontal() const;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;//talvez tirar o override

private:
    int selectedShipSize = -1;
    int selectedShipIndex = -1;
    bool isDragging; //testar se ainda é necessario
    bool Horizontal;
    QPoint clickOffset;
    QPoint initialPosition;

    QLabel* selectedShipLabel = nullptr;

    QMap<QLabel*, QPixmap> originalPixmaps;
    QMap<QLabel*, bool> shipRotation;
    QPixmap originalShipPixmaps;

    QVector<ShipInfo> shipInfos;

    //PARTE DA ANIMACAO DO SELECTORSPACE
    QGraphicsDropShadowEffect* currentEffect = nullptr;
    QPropertyAnimation* blurAnimation = nullptr;
    QPropertyAnimation* colorAnimation = nullptr;

public slots:
    void onPlacementFailed();

signals:
    void shipSelected();
};

#endif // SELECTORSPACE_H
