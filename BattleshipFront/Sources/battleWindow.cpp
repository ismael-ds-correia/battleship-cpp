#include "./Headers/battleWindow.h"
#include "../Headers/mainwindow.h"
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QApplication>
#include <QPushButton>

BattleWindow::BattleWindow(
    BoardController* playerBoardController,
    BoardController* enemyBoardController,
    ShipController* shipBattleController,
    PlayerController* playerBattleController,
    PlayerController* enemyBattleController,
    QWidget* parent)
    : QMainWindow(parent),
    playerBoardController(playerBoardController),
    enemyBoardController(enemyBoardController),
    shipController(shipBattleController),
    playerController(playerBattleController),
    enemyController(enemyBattleController) {
    setupUI();

    // Cria o renderizador para o tabuleiro do jogador (exibindo os navios)
    playerRenderer = new BoardRenderer(playerScene, shipBattleController, playerBoardController, nullptr, playerBattleController);
    playerRenderer->setHideShips(false); //false para mostrar os navios
    playerRenderer->setInteractive(false); //desabilita o clique no próprio tabuleiro
    playerRenderer->renderCoordinates();

    // Cria o renderizador para o tabuleiro do robô (oculta os navios)
    enemyRenderer = new BoardRenderer(enemyScene, shipBattleController, enemyBoardController, nullptr, playerBattleController, enemyBattleController, true);
    enemyRenderer->setHideShips(false); // ativa a flag para esconder os navios
    enemyRenderer->setInteractive(true); //habilita o clique no tabuleiro inimigo
    enemyRenderer->renderCoordinates();

    // Renderiza os tabuleiros
    playerRenderer->renderBoard();
    enemyRenderer->renderBoard();

    connect(playerBattleController, &PlayerController::attackResult,
            enemyRenderer, &BoardRenderer::onAttackResult);
    connect(enemyRenderer, &BoardRenderer::gameOver, this, &BattleWindow::handleGameOver);
    connect(playerBattleController, &PlayerController::shipDestroyed,
            enemyRenderer, &BoardRenderer::onShipDestroyed);
    //adicionar connect do robô para as posicoes adjacentes quando for implementado

    setWindowTitle("Tela de Batalha");
}

BattleWindow::~BattleWindow() {
    delete playerRenderer;
    delete enemyRenderer;
}

void BattleWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(centralWidget);

    playerView = new QGraphicsView(centralWidget);
    enemyView = new QGraphicsView(centralWidget);

    playerScene = new QGraphicsScene(this);
    enemyScene = new QGraphicsScene(this);

    playerView->setScene(playerScene);
    enemyView->setScene(enemyScene);

    layout->addWidget(playerView);
    layout->addWidget(enemyView);

    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

void BattleWindow::handleGameOver(bool playerWon) {
    QString winner = playerWon ? "Jogador" : "Robô";

    QMessageBox msgBox;
    msgBox.setWindowTitle("Fim de Jogo");
    msgBox.setText(QString("%1 venceu!").arg(winner));
    QPushButton* restartButton = msgBox.addButton("Reiniciar", QMessageBox::AcceptRole);
    QPushButton* quitButton = msgBox.addButton("Sair", QMessageBox::RejectRole);

    msgBox.exec();

    if (msgBox.clickedButton() == restartButton) {
        restartGame();
    } else if (msgBox.clickedButton() == quitButton) {
        qApp->quit();
    }
}

//PRECISAMOS CONSERTAR O BUG INCOMUM QUE O FIM DE JOGO NÃO É ACIONADO EM ALGUNS CASOS

void BattleWindow::restartGame() {
    // Cria uma nova instância da MainWindow com os controladores do jogador
    MainWindow* mainWindow = new MainWindow(playerBoardController, shipController, playerController);
    mainWindow->show();

    // Fecha a BattleWindow atual
    this->close();
}

