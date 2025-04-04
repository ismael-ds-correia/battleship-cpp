#ifndef BATTLEWINDOW_H
#define BATTLEWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "boardRenderer.h"
#include "boardController.h"
#include "playerController.h"
#include "robotController.h"
#include "shipController.h"
#include "gameController.h"

class BattleWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit BattleWindow(
        BoardController* playerBoardController,
        BoardController* enemyBoardController,
        ShipController* shipController,
        PlayerController* playerController,
        RobotController* enemyController,
        QWidget* parent = nullptr
    );

    ~BattleWindow();

private:
    //Interface
    QGraphicsView* playerView;
    QGraphicsView* enemyView;
    QGraphicsScene* playerScene;
    QGraphicsScene* enemyScene;

    BoardRenderer* playerRenderer;
    BoardRenderer* enemyRenderer;

    //logica de jogo
    BoardController* playerBoardController;
    BoardController* enemyBoardController;
    ShipController* shipController;
    PlayerController* playerController;
    RobotController* enemyController;
    GameController* gameController;

    void setupUI();

private slots:
    void handleGameOver(bool playerWon);
    void restartGame(); // Função para reiniciar o jogo
};

#endif // BATTLEWINDOW_H
