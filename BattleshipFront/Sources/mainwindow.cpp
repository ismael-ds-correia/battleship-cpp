#include "../Headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "../Headers/draggableShip.h"

MainWindow::MainWindow(BoardController* boardController, ShipController* shipController, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), boardController(boardController), shipController(shipController) {

    ui->setupUi(this);//instancia inicial do Ui

    //cria o Scene do tabuleiro
    scene = new QGraphicsScene(this);
    ui->boardView->setScene(scene);

    //cria o Scene do selector
    selectorScene = new QGraphicsScene(this);
    ui->selectorView->setScene(selectorScene);
    ui->selectorView->setSceneRect(0, 0, ui->selectorView->width(), ui->selectorView->height());

    setWindowTitle("Batalha batalhesca");

    boardRenderer = new BoardRenderer(scene, shipController, boardController);

    //conecta as interações do front com os comandos do back
    connect(ui->randomizer, &QPushButton::clicked, this, &MainWindow::onRandomizeButtonClicked);
    connect(boardController, &BoardController::boardUpdated, this, &MainWindow::updateBoard);

    //sessão de testes
    selectorSpace = new SelectorSpace(this);
    ui->selectorContainer->setLayout(new QVBoxLayout);
    ui->selectorContainer->layout()->addWidget(selectorSpace);
    selectorSpace->show();
    //QPixmap boatTexture("../../Textures/subH.png");
    //DraggableShip *testShip = new DraggableShip(boatTexture, 1);
    //testShip->setPos(10, 10);
    //selectorScene->addItem(testShip);


    /*Ship testShip1("teste1", 3);
    Ship testShip2("teste2", 4);
    testShip2.setOrientation(false);
    boardController->placeShip(2, 3, testShip1);
    boardController->placeShip(6, 2, testShip2);*/

    updateBoard();
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::onRandomizeButtonClicked() {
    boardController->randomizeShips();
}

void MainWindow::updateBoard() {
    boardRenderer->renderBoard();
}

