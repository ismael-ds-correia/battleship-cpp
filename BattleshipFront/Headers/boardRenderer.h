#ifndef BOARDRENDERER_H
#define BOARDRENDERER_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QLabel>
#include "boardController.h"
#include "shipController.h"
#include "selectorSpace.h"
#include "playerController.h"


class BoardRenderer : public QGraphicsView {
    Q_OBJECT

public:
    BoardRenderer(
        QGraphicsScene* scene,
        ShipController* shipController,
        BoardController* boardController,
        SelectorSpace* selectorSpace,
        PlayerController* playerController
    );

    void setupBoardSelector();
    void renderBoard();
    void loadTextures();
    void renderWater();
    void renderShips();
    void setHideShips(bool hide);
    void renderCoordinates();


public slots:
    void handleCellClick(int row, int col); //função de testes, remover depois

private:
    QGraphicsScene* scene;
    ShipController* shipController;
    BoardController* boardController;
    SelectorSpace* selectorSpace;
    PlayerController* playerController;

    //Ship* selectedShip = nullptr;
    QLabel* selectedShipLabel;
    Ship tempShip;

    //
    QPixmap battleshipTextureH;
    QPixmap submarineTextureH;
    QPixmap cruiserTextureH;
    QPixmap carrierTextureH;

    QPixmap battleshipTextureV;
    QPixmap submarineTextureV;
    QPixmap cruiserTextureV;
    QPixmap carrierTextureV;

    QPixmap scaledBattleshipTextureH;
    QPixmap scaledSubmarineTextureH;
    QPixmap scaledCruiserTextureH;
    QPixmap scaledCarrierTextureH;

    QPixmap scaledBattleshipTextureV;
    QPixmap scaledSubmarineTextureV;
    QPixmap scaledCruiserTextureV;
    QPixmap scaledCarrierTextureV;

    //

    QPixmap waterTexture;
    QPixmap scaledWaterTexture;

    bool hideShips;


};

#endif // BOARDRENDERER_H
