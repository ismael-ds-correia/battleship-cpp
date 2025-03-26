#ifndef BATTLEWINDOW_H
#define BATTLEWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "boardRenderer.h"
#include "boardController.h"
#include "playerController.h"
#include "shipController.h"

class BattleWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit BattleWindow(BoardController* playerBoardController,
                          BoardController* enemyBoardController,
                          ShipController* shipController,
                          PlayerController* playerController,
                          PlayerController* enemyController,
                          QWidget* parent = nullptr);
    ~BattleWindow();

private:
    QGraphicsView* playerView;
    QGraphicsView* enemyView;
    QGraphicsScene* playerScene;
    QGraphicsScene* enemyScene;

    BoardRenderer* playerRenderer;
    BoardRenderer* enemyRenderer;

    BoardController* playerBoardController;
    BoardController* enemyBoardController;
    ShipController* shipController;
    PlayerController* playerController;
    PlayerController* enemyController;

    void setupUI();

private slots:
    void handleGameOver(bool playerWon);
    void restartGame(); // Função para reiniciar o jogo
};

#endif // BATTLEWINDOW_H
