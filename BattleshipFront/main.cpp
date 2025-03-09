#include "Headers/mainwindow.h"

#include <QApplication>
#include "boardController.h"
#include "shipController.h"
#include "playerController.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Player player1("jogador1");

    BoardController p1BoardController(&player1); //instancia o controllador
    ShipController shipController;
    PlayerController playerController(&player1);

    MainWindow mainWindow(&p1BoardController, &shipController, &playerController); //cria a janela do jogo
    mainWindow.show();

    return app.exec();
}
