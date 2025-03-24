#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>

// Forward declarations para evitar includes desnecessários
class BattleWindow;
class PlayerController;
class RobotController;

class GameController : public QObject {
    Q_OBJECT

public:
    explicit GameController(
        BattleWindow* battleWindow,
        PlayerController* playerController,
        RobotController* robotController,
        QObject* parent = nullptr
        );

    // Retorna true se for o turno do jogador, false se for o turno da IA
    bool isPlayerTurn() const;

signals:
    // Sinal para notificar a mudança de turno (pode ser usado para atualizar a interface)
    void turnChanged(bool isPlayerTurn);

public slots:
    // Slots para processar os ataques realizados por cada controlador
    void onPlayerAttack();
    void onEnemyAttack();

    // Alterna o turno entre o jogador e a IA
    void nextTurn();

private:
    BattleWindow* battleWindow;         // Referência para a janela de batalha (opcional, se precisar interagir com a UI)
    PlayerController* playerController; // Controlador do jogador
    RobotController* robotController;  // Controlador do inimigo/IA
    bool playerTurn;                    // true: turno do jogador, false: turno da IA
};

#endif // GAMECONTROLLER_H
