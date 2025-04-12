#ifndef BATTLEWINDOW_H
#define BATTLEWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "boardRenderer.h"
#include "boardController.h"
#include "playerController.h"
#include "qmessagebox.h"
#include "shipController.h"
#include "robotController.h"

enum class Turn { Player, Enemy };

class BattleWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit BattleWindow(
        BoardController* playerBoardController,
        BoardController* enemyBoardController,
        ShipController* shipController,
        PlayerController* playerController,
        RobotController* enemyController,
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
    RobotController* enemyController;

    void setupUI();

    Turn currentTurn;

    QMessageBox* gameOverBox = nullptr;
    QTimer* enemyTurnTimer = nullptr;

private slots:
    void handleGameOver(bool playerWon);
    void restartGame();
    void onPlayerAttack(int row, int col);
    void handlePlayerAttackResult(int row, int col, bool hit);
    void enemyAttack();
    void handleEnemyAttackResult(int row, int col, bool hit);
    void updateTurn();
};

#endif // BATTLEWINDOW_H
