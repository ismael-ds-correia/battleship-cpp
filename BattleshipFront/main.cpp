#include "Headers/mainwindow.h"

#include <QApplication>
#include "boardController.h"
#include "shipController.h"
#include "playerController.h"
#include <QLoggingCategory>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    app.setStyleSheet(
        "QMainWindow { background-color: #202020; }"
        "QLabel { color: white; }"
        "QPushButton { "
        "   background-color: #333; "
        "   color: #fff; "
        "   padding: 5px; "
        "   border: 1px solid #555; "
        "   border-radius: 25px; "
        "} "
        "QPushButton:hover { "
        "   background-color: #444; "
        "}"
        );

    Player player1("jogador1");

    BoardController p1BoardController(&player1); //instancia o controllador
    ShipController shipController;
    PlayerController playerController(&player1);

    MainWindow mainWindow(&p1BoardController, &shipController, &playerController); //cria a janela do jogo
    mainWindow.show();

    //QLoggingCategory::setFilterRules(QStringLiteral("qt.multimedia.*=true"));

    return app.exec();
}
