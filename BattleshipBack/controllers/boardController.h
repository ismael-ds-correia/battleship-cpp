#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "board.h"
#include <QObject>

class BoardController : public QObject {

    Q_OBJECT // Necessário para usar sinais e slots do Qt
    private:
        Board board; // Instância do tabuleiro.
    public:
        BoardController(); // Construtor
        ~BoardController(); // Destrutor

    // Eventos recebidos do frontend:
    void placeShip(int x, int y, Ship& ship); // Posiciona um navio no tabuleiro.
    void attackPosition(int x, int y);       // Ataca uma posição específica.

    // Dados enviados ao frontend:
    Position (&getBoardState())[10][10];     // Retorna o estado atual do tabuleiro.

signals:
    void boardUpdated();  // Sinal emitido quando o tabuleiro é atualizado.
    void attackResult(bool hit); // Sinal emitido após um ataque (acerto ou erro).
};

#endif
