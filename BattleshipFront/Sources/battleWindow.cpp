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
    RobotController* enemyBattleController,
    QWidget* parent)
    : QMainWindow(parent),
    playerBoardController(playerBoardController),
    enemyBoardController(enemyBoardController),
    shipController(shipBattleController),
    playerController(playerBattleController),
    enemyController(enemyBattleController),
    currentTurn(Turn::Player){

    setupUI();

    // Cria o renderizador para o tabuleiro do jogador (exibindo os navios)
    playerRenderer = new BoardRenderer(playerScene, shipBattleController, playerBoardController, nullptr, playerBattleController);
    playerRenderer->setHideShips(false);
    playerRenderer->setInteractive(false);
    playerRenderer->renderCoordinates();

    enemyRenderer = new BoardRenderer(enemyScene, shipBattleController, enemyBoardController, nullptr, playerBattleController, enemyBattleController, true);
    enemyRenderer->setHideShips(false);
    enemyRenderer->setInteractive(true);
    enemyRenderer->renderCoordinates();


    // Renderiza os tabuleiros
    playerRenderer->renderBoard();
    enemyRenderer->renderBoard();

    connect(playerBattleController, &PlayerController::attackResult, enemyRenderer, &BoardRenderer::onAttackResult);
    connect(playerBattleController, &PlayerController::shipDestroyed, enemyRenderer, &BoardRenderer::onShipDestroyed);
    connect(enemyRenderer, &BoardRenderer::gameOver, this, &BattleWindow::handleGameOver);
    connect(enemyRenderer, &BoardRenderer::cellClicked, this, &BattleWindow::onPlayerAttack);

    connect(playerController, &PlayerController::attackResult, enemyRenderer, &BoardRenderer::onAttackResult);
    // Atualiza o tabuleiro do jogador quando a IA ataca:
    connect(enemyController, &RobotController::attackResult, playerRenderer, &BoardRenderer::onAttackResult);

    connect(playerController, &PlayerController::attackResult, this, &BattleWindow::handlePlayerAttackResult);
    connect(enemyController, &RobotController::attackResult, this, &BattleWindow::handleEnemyAttackResult);

    connect(playerController, &PlayerController::shipDestroyed, enemyRenderer, &BoardRenderer::onShipDestroyed);
    connect(enemyController, &RobotController::shipDestroyed, enemyRenderer, &BoardRenderer::onShipDestroyed);



    updateTurn();
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

void BattleWindow::updateTurn() {
    qDebug() << "Atualizando turno. Turno atual:" << (currentTurn == Turn::Player ? "Player" : "Enemy");
    if (currentTurn == Turn::Player) {
        enemyRenderer->setInteractive(true);
        qDebug() << "Turno do jogador. Interatividade do tabuleiro inimigo habilitada.";
    } else {
        enemyRenderer->setInteractive(false);
        qDebug() << "Turno da IA. Interatividade do tabuleiro inimigo desabilitada.";
        QTimer::singleShot(1000, this, SLOT(enemyAttack()));
    }
}

// Slot chamado quando o jogador realiza um ataque (por exemplo, via clique)
void BattleWindow::onPlayerAttack(int row, int col) {
    qDebug() << "onPlayerAttack chamado. Linha:" << row << "Coluna:" << col;
    if (currentTurn != Turn::Player) {
        qDebug() << "Ataque ignorado pois não é o turno do jogador.";
        return;
    }

    // Tiramos isso pq se não o turno muda mesmo se acertar o ataque
    //currentTurn = Turn::Enemy;
    //updateTurn();
}

// Para tratarmos do ataque do jogador
void BattleWindow::handlePlayerAttackResult(int row, int col, bool hit) {
    if (!hit) {
        currentTurn = Turn::Enemy;
        updateTurn();
    } else {
        // Verifica se todos os navios inimigos foram destruídos
        if (enemyController->getPlayer()->getFleet().isDestroyed()) {
            handleGameOver(true); // Jogador venceu
        }
    }
}

// Slot para o ataque da IA

void BattleWindow::enemyAttack() {
    qDebug() << "BattleWindow::enemyAttack: Início do ataque da IA.";

    bool hit = enemyController->attackOpponent(playerController->getPlayer());
    qDebug() << "BattleWindow::enemyAttack: Resultado do ataque da IA, hit =" << hit;

    // Mesma coisa com o ataque do bot
    //currentTurn = Turn::Player;
    //updateTurn();
    //qDebug() << "BattleWindow::enemyAttack: Turno alterado para Player e updateTurn() chamado.";
}


void BattleWindow::handleEnemyAttackResult(int row, int col, bool hit) {
    if (hit) {
        // Verifica se todos os navios do jogador foram destruídos
        if (playerController->getPlayer()->getFleet().isDestroyed()) {
            qDebug() << "Fim de jogo, robô venceu";
            handleGameOver(false); // Robô venceu
        } else {
            // Continua o turno do robô
            QTimer::singleShot(1000, this, SLOT(enemyAttack()));
        }
    } else {
        currentTurn = Turn::Player;
        updateTurn();
    }
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

