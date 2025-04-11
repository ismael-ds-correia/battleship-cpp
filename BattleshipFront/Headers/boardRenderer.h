#ifndef BOARDRENDERER_H
#define BOARDRENDERER_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QLabel>
#include "Headers/boardCell.h"
#include "boardController.h"
#include "shipController.h"
#include "selectorSpace.h"
#include "playerController.h"
#include "robotController.h"
#include "soundManager.h"

class BoardRenderer : public QGraphicsView {
    Q_OBJECT

public:
    BoardRenderer(
        QGraphicsScene* scene,
        ShipController* shipController,
        BoardController* boardController,
        SelectorSpace* selectorSpace,
        PlayerController* playerController,
        SoundManager* soundManager,
        RobotController* enemyController = nullptr,
        bool attackMode = false
    );

    void setupBoardSelector();
    void renderBoard();
    void loadTextures();
    void renderWater();
    void renderShips();
    void setHideShips(bool hide);
    void renderCoordinates();
    void onAttackResult(int row, int col, bool hit);
    QPointF calculatePosition(int row, int col) const;

    void setInteractive(bool interactive); //para bloquear o clique no próprio tabuleiro

    void setSelectorSpace(SelectorSpace* newSelectorSpace);
    void updatePlacementIndicators();

public slots:
    void handleCellClick(int row, int col); //função de testes, remover depois
    void onShipDestroyed(Ship* ship);



signals:
    void gameOver(bool playerWon);
    void cellClicked(int row, int col);

private:
    QGraphicsScene* scene;
    ShipController* shipController;
    BoardController* boardController;
    SelectorSpace* selectorSpace;
    PlayerController* playerController;

    std::vector<QGraphicsRectItem*> placementIndicators;

    bool attackMode;
    PlayerController* attackerController;
    RobotController* enemyController; //MANGA


    QVector<BoardCell*> waterCells;
    QVector<QGraphicsItem*> shipItems;

    //Ship* selectedShip = nullptr;
    QLabel* selectedShipLabel;
    Ship tempShip;

    // texturas
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

    QPixmap waterHitTexture;
    QPixmap shipHitTexture;
    QPixmap scaledWaterHitTexture;
    QPixmap scaledShipHitTexture;

    //

    SoundManager* soundManager;

    bool hideShips;
    bool isInteractive;

};

#endif // BOARDRENDERER_H
