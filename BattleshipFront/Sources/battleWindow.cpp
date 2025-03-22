#include "./Headers/battleWindow.h"
#include <QHBoxLayout>
#include <QWidget>

BattleWindow::BattleWindow(
    BoardController* playerBoardController,
    BoardController* enemyBoardController,
    ShipController* shipController,
    PlayerController* playerController,
    PlayerController* enemyController,
    QWidget* parent)
    : QMainWindow(parent),
    playerBoardController(playerBoardController),
    enemyBoardController(enemyBoardController),
    shipController(shipController),
    playerController(playerController),
    enemyController(enemyController) {
    setupUI();

    // Cria o renderizador para o tabuleiro do jogador (exibindo os navios)
    playerRenderer = new BoardRenderer(playerScene, shipController, playerBoardController, nullptr, playerController);
    playerRenderer->setHideShips(false); //false para mostrar os navios
    playerRenderer->setInteractive(false); //desabilita o clique no próprio tabuleiro
    playerRenderer->renderCoordinates();

    // Cria o renderizador para o tabuleiro do robô (oculta os navios)
    enemyRenderer = new BoardRenderer(enemyScene, shipController, enemyBoardController, nullptr, playerController, enemyController, true);
    enemyRenderer->setHideShips(false); // ativa a flag para esconder os navios
    enemyRenderer->setInteractive(true); //habilita o clique no tabuleiro inimigo
    enemyRenderer->renderCoordinates();

    // Renderiza os tabuleiros
    playerRenderer->renderBoard();
    enemyRenderer->renderBoard();

    connect(playerController, &PlayerController::attackResult,
            enemyRenderer, &BoardRenderer::onAttackResult);

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
