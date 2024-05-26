#include "AbstractController.h"

void AbstractController::getControl(AbstractController *controller, const QString &flag) {
    if(controller != nullptr) {
        controller->dispose();
    }
    initializeController(flag);
}

void AbstractController::interconnectControllers(AbstractController *menuController, AbstractController *gameController) {
    connect(menuController, &AbstractController::passControl, gameController,
            &AbstractController::getControl);
    connect(gameController, &AbstractController::passControl, menuController,
            &AbstractController::getControl);
}