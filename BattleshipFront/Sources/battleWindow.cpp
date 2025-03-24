#include "./Headers/battleWindow.h"
#include "gameController.h"
#include <QHBoxLayout>
#include <QWidget>

BattleWindow::BattleWindow(
    BoardController* playerBoardController,
    BoardController* enemyBoardController,
    ShipController* shipController,
    PlayerController* playerController,
    RobotController* enemyController,
    QWidget* parent)
    : QMainWindow(parent),
    playerBoardController(playerBoardController),
    enemyBoardController(enemyBoardController),
    shipController(shipController),
    playerController(playerController),
    enemyController(enemyController),
    gameController(nullptr) {
    setupUI();

    // Configura o renderizador para o tabuleiro do jogador (exibindo os navios)
    playerRenderer = new BoardRenderer(playerScene, shipController, playerBoardController, nullptr, playerController);
    playerRenderer->setHideShips(false);  // false para mostrar os navios
    playerRenderer->setInteractive(false);  // desabilita o clique no próprio tabuleiro
    playerRenderer->renderCoordinates();

    // Configura o renderizador para o tabuleiro do inimigo (ou IA)
    enemyRenderer = new BoardRenderer(enemyScene, shipController, enemyBoardController, nullptr, playerController, enemyController, true);
    enemyRenderer->setHideShips(false);  // ativa a flag para esconder os navios (ou não, dependendo da lógica)
    enemyRenderer->setInteractive(true);   // habilita o clique no tabuleiro inimigo
    enemyRenderer->renderCoordinates();

    // Renderiza os tabuleiros
    playerRenderer->renderBoard();
    enemyRenderer->renderBoard();

    // Conecta o sinal de resultado de ataque do jogador para atualizar o tabuleiro inimigo
    connect(playerController, &PlayerController::attackResult,
            enemyRenderer, &BoardRenderer::onAttackResult);

    // Instancia o GameController no corpo do construtor
    gameController = new GameController(this, playerController, enemyController, this);

    // Conecta o sinal de mudança de turno para atualizar a interatividade do tabuleiro inimigo
    connect(gameController, &GameController::turnChanged, this, [=](bool isPlayerTurn) {
        enemyRenderer->setInteractive(isPlayerTurn);
        // Aqui você pode adicionar outras atualizações na interface, como exibir o turno atual
    });

    setWindowTitle("Tela de Batalha");
}

BattleWindow::~BattleWindow() {
    delete playerRenderer;
    delete enemyRenderer;
    delete gameController;
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
