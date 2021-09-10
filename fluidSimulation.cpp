#include <iostream>
#include "Fluid.h"
#include "structs.h"

using namespace std;


int main(int argc, char** argv) {

    Settings setting;


    Fluid fluidSquare(setting, 0, 0, 0.1);

    fluidSquare.initSystems();
    fluidSquare.addDye(setting);

    while (fluidSquare.isRunning()){

        fluidSquare.update(setting);
        fluidSquare.handleEvents(setting);
        fluidSquare.render();
    }

    fluidSquare.clean();



    return 0;
}
