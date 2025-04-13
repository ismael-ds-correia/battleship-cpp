#include "./Headers/battleWindow.h"
#include "../Headers/mainwindow.h"
#include "ui_battleWindow.h"
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
    currentTurn(Turn::Player),
    ui(new Ui::BattleWindow){

    ui->setupUi(this);
    //setupUI();


    //PARTE DA INTERFACE
    ui->playerView->setRenderHint(QPainter::Antialiasing);
    ui->enemyView->setRenderHint(QPainter::Antialiasing);
    ui->playerView->setCacheMode(QGraphicsView::CacheBackground);
    ui->enemyView->setCacheMode(QGraphicsView::CacheBackground);

    //SOM DO JOGO
    soundManager = new SoundManager(this);
    soundManager->playBackgroundMusic();

    // Cria o renderizador para o tabuleiro do jogador
    playerScene = new QGraphicsScene(this);
    enemyScene = new QGraphicsScene(this);
    ui->playerView->setScene(playerScene);
    ui->enemyView->setScene(enemyScene);

    // Configura os renderizadores
    playerRenderer = new BoardRenderer(playerScene, shipBattleController, playerBoardController, nullptr, playerBattleController, soundManager);
    playerRenderer->setHideShips(false);
    playerRenderer->setInteractive(false);
    playerRenderer->renderCoordinates();

    enemyRenderer = new BoardRenderer(enemyScene, shipBattleController, enemyBoardController, nullptr, playerBattleController, soundManager, enemyBattleController, true);
    enemyRenderer->setHideShips(true);
    enemyRenderer->setInteractive(true);
    enemyRenderer->renderCoordinates();

    // Renderiza os tabuleiros
    playerRenderer->renderBoard();
    enemyRenderer->renderBoard();

    // Conexões de sinal/slot
    connect(playerBattleController, &PlayerController::shipDestroyed, enemyRenderer, &BoardRenderer::onShipDestroyed);
    connect(enemyRenderer, &BoardRenderer::gameOver, this, &BattleWindow::handleGameOver);
    connect(enemyRenderer, &BoardRenderer::cellClicked, this, &BattleWindow::onPlayerAttack);

    connect(playerController, &PlayerController::attackResult, enemyRenderer, &BoardRenderer::onAttackResult);
    connect(enemyController, &RobotController::attackResult, playerRenderer, &BoardRenderer::onAttackResult);

    connect(playerController, &PlayerController::attackResult, this, &BattleWindow::handlePlayerAttackResult);
    connect(enemyController, &RobotController::attackResult, this, &BattleWindow::handleEnemyAttackResult);

    connect(playerController, &PlayerController::shipDestroyed, playerRenderer, &BoardRenderer::onShipDestroyed);
    connect(enemyController, &RobotController::shipDestroyed, playerRenderer, &BoardRenderer::onShipDestroyed);
    updateTurn();
    setWindowTitle("Tela de Batalha");
}

BattleWindow::~BattleWindow() {
    delete ui;
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
        ui->labelTurnStatus->setText("🟢 Turno do Jogador");
        enemyRenderer->setInteractive(true);
    } else {
        ui->labelTurnStatus->setText("🔴 Turno da IA");
        enemyRenderer->setInteractive(false);
        QTimer::singleShot(1000, this, SLOT(enemyAttack()));
    }
}

void BattleWindow::onPlayerAttack(int row, int col) {
    qDebug() << "onPlayerAttack chamado. Linha:" << row << "Coluna:" << col;
    if (currentTurn != Turn::Player) {
        qDebug() << "Ataque ignorado pois não é o turno do jogador.";
        return;
    }
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
    // Verifica se o turno ainda é da IA.
    if (currentTurn != Turn::Enemy) {
        qDebug() << "enemyAttack chamado, mas o turno não é da IA. Abortando ataque.";
        return;
    }

    qDebug() << "BattleWindow::enemyAttack: Início do ataque da IA.";

    bool hit = enemyController->attackOpponent(playerController->getPlayer());
    qDebug() << "BattleWindow::enemyAttack: Resultado do ataque da IA, hit =" << hit;
}

void BattleWindow::handleEnemyAttackResult(int row, int col, bool hit) {
    if (hit) {
        if (playerController->getPlayer()->getFleet().isDestroyed()) {
            handleGameOver(false); // Jogador perdeu
        } else {
            // Se não, continua o turno da IA após um pequeno delay
            QTimer::singleShot(1000, this, SLOT(enemyAttack()));
        }
    } else {
        currentTurn = Turn::Player;
        updateTurn();
    }
}

void BattleWindow::handleGameOver(bool playerWon) {
    if (gameOverBox) return;

    QString winner = playerWon ? "Jogador" : "Robô";

    gameOverBox = new QMessageBox(this);
    gameOverBox->setWindowTitle("Fim de Jogo");
    gameOverBox->setText(QString("%1 venceu!").arg(winner));

    gameOverBox->setStyleSheet(
        "QLabel {"
        "    color: black;"         // cor desejada para o texto (por exemplo, preto)
        "    font: 18pt 'segoiUi';"   // aumenta o tamanho e usa a fonte 'suntage'
        "}"
        );

    QPushButton* restartButton = gameOverBox->addButton("Reiniciar", QMessageBox::AcceptRole);
    QPushButton* quitButton = gameOverBox->addButton("Sair", QMessageBox::RejectRole);

    connect(gameOverBox, &QMessageBox::buttonClicked, this, [this, restartButton](QAbstractButton* button){
        if (button == restartButton) {
            restartGame();
        } else {
            qApp->quit();
        }
        gameOverBox->deleteLater();
        gameOverBox = nullptr;
    });

    gameOverBox->show();
}

void BattleWindow::restartGame() {

    // Cria novos controladores com estado fresco
    Player* newPlayer = new Player("Jogador");
    BoardController* newBoardController = new BoardController(newPlayer);
    ShipController* newShipController = new ShipController();
    PlayerController* newPlayerController = new PlayerController(newPlayer);

    // Cria nova janela principal com os novos controladores
    MainWindow* mainWindow = new MainWindow(
        newBoardController,
        newShipController,
        newPlayerController
        );

    // Configura para auto-deleção
    mainWindow->setAttribute(Qt::WA_DeleteOnClose);
    mainWindow->show();

    // Destrói os controladores antigos
    //delete playerBoardController;
    //delete enemyBoardController;
    //delete shipController;
    //delete playerController;
    //delete enemyController;

    // Fecha esta janela
    this->close();
    this->deleteLater();
}

