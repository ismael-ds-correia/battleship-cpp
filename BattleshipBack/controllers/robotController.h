#ifndef ROBOTCONTROLLER_H
#define ROBOTCONTROLLER_H

#include "playerController.h"

// A classe RobotController herda de PlayerController, pois a IA é um tipo de Player.
// Assim, ela mantém a interface padrão, mas sobrescreve ou adiciona comportamentos específicos.
class RobotController : public PlayerController {
    Q_OBJECT
public:
    explicit RobotController(Player* player, QObject* parent = nullptr);
    virtual ~RobotController();

    // Método para a IA atacar sem precisar de coordenadas (sendo escolhido internamente)
    bool attackOpponent(Player* opponent);  //talvez usar Override
};

#endif // ROBOTCONTROLLER_H
