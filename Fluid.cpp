#include "Fluid.h"
#include "functions.h"
#include <iostream>

Fluid::Fluid()
{

    Vx = nullptr;
    Vy = nullptr;

    Vx0 = nullptr;
    Vy0 = nullptr;

    density = nullptr;
    s = nullptr;
}

Fluid::Fluid(Settings setting, int diff, int viscosity, float dt){

    //assigning function arguments
    this -> size = setting.N;
    this -> dt = dt;
    this -> diffusion = diff;
    this -> viscosity = viscosity;

    //setting density arrays
    s = new float[size * size];
    density = new float[size * size];

    //setting velocity arrays
    Vx = new float[size * size];
    Vy = new float[size * size];

    //setting initial arrays
    Vx0 = new float[size * size];
    Vy0 = new float[size * size];

    //indicates a fluid square has been made
    created = true;
}

Fluid::~Fluid(){

    //deleting allocated memory
    if (created){

        //deleting velocity values
        delete [] Vx;
        delete [] Vy;

        //deleting initial velocity values
        delete [] Vx0;
        delete [] Vy0;

        //deleting density values
        delete [] density;
        delete [] s;



    }
}

//adding density

void Fluid::addDensity(int x, int y, float amount, Settings setting){

    //setting index
    int index = IX(x, y, setting.N);

    //adding density amount
    density[index] += amount;

}

//adding velocity

void Fluid::addVelocity(int x, int y, float amountX, float amountY, Settings setting){

    //setting index
    int index = IX(x, y, setting.N);

    //adding amount values
    Vx[index] += amountX;
    Vy[index] += amountY;

    //std::cout << Vx[index] << std::endl;



}

//time step function
void Fluid::timeStep(Settings setting){

    float visc = viscosity;
    float diff = diffusion;
    float dt = this -> dt;
    float *Vx = this -> Vx;
    float *Vy = this -> Vy;
    float *Vx0 = this -> Vx0;
    float *Vy0 = this -> Vy0;
    float *s = this -> s;
    float *density = this -> density;

    diffuse(1, Vx0, Vx, visc, dt, setting);
    diffuse(2, Vy0, Vy, visc, dt, setting);


    project(Vx0, Vy0, Vx, Vy, setting);

    advect(1, Vx, Vx0, Vx0, Vy0, dt, setting);
    advect(2, Vy, Vy0, Vx0, Vy0, dt, setting);


    project(Vx, Vy, Vx0, Vy0, setting);


    diffuse(0, s, density, diff, dt, setting);
    advect(0, density, s, Vx, Vy, dt, setting);

}

//below are all the SDL functions

bool Fluid::isRunning(){

    return running;
}

void Fluid::initSystems(){

    int flag = 0;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {

        std::cout << "Systems initialized....." << std::endl;

        _window = SDL_CreateWindow("Fluid Simulation", SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED, size * 10, size * 10, 0);

        SDL_WarpMouseInWindow(_window, (size * 10) / 2, (size * 10) / 2);

        if (_window){

            std::cout << "Window created!" << std::endl;
        }

        _render = SDL_CreateRenderer(_window, -1, flag);

        if(_render){

            std::cout << "Renderer created!" << std::endl;

        }

        running = true;

    }

    else{
        running = false;
    }

}

void Fluid::addDye(Settings setting){

    addDensity((size * 10) / 2, (size * 10) / 2, 100, setting);
    addVelocity((size * 10) / 2, (size * 10) / 2, 3, 3, setting);


}

void Fluid::handleEvents(Settings setting){


    SDL_Event event;
    SDL_PollEvent(&event);
    int prev_pos_x = mouse_x;
    int prev_pos_y = mouse_y;
    float velocity_x;
    float velocity_y;


    switch (event.type){

        case SDL_QUIT:

            running = false;
            break;

        case SDL_MOUSEMOTION:


            mouse_x = event.button.x;
            mouse_y = event.button.y;

            velocity_x = mouse_x - prev_pos_x;
            velocity_y = mouse_y - prev_pos_y;

            if (velocity_x > 10){
                velocity_x = 10.0;
            }

            if(velocity_y > 10){
                velocity_y = 10.0;
            }

            if(velocity_x < -10){
                velocity_x = -10.0;
            }

            if(velocity_y < -10){
                velocity_y = -10.0;
            }


            if ((mouse_x >= 0 && mouse_x < size * 10) && (mouse_y >= 0 && mouse_y < size * 10)){

                mouseDown = true;

                    while(mouseDown){


                        addDensity(mouse_x / 10, mouse_y / 10, 100, setting);

                        break;

                    }
            }



            addVelocity(mouse_x / 10 , mouse_y / 10, velocity_x, velocity_y, setting);


            prev_pos_x = mouse_x;
            prev_pos_y = mouse_y;

            break;


        default:
            break;
    }
}


void Fluid::renderAllPoints(){
    int color;
    for (int i = 0; i <= size - 1; ++i){
        for (int j = 0; j <= size - 1; ++j){
            color = density[IX(j, i, size)] * 10;
            SDL_SetRenderDrawColor(_render, color, color, color, 0);
            SDL_RenderDrawPoint(_render, j * 10, i * 10);
        }
    }
}


void Fluid::render(){


    SDL_SetRenderDrawColor(_render, 0, 0, 0, 255);
    SDL_RenderClear(_render);

    renderAllPoints();

    SDL_RenderPresent(_render);
}

void Fluid::update(Settings setting){
    timeStep(setting);
}

void Fluid::clean(){

    SDL_DestroyWindow(_window);
    SDL_DestroyRenderer(_render);
    SDL_Quit();

    std::cout << "Exiting program..." << std::endl;
    std::cout << "Program cleaned!" << std::endl;


}


