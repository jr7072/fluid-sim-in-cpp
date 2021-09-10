#pragma once
#include <SDL.h>
#include "structs.h"

class Fluid
{
    public:

        Fluid();
        Fluid(Settings, int, int, float);
        ~Fluid();

        //adding density to cells
        void addDensity (int, int, float, Settings);

        //to add velocity
        void addVelocity (int, int, float, float, Settings);

        void addDye(Settings);

        //step function
        void timeStep(Settings);

        //SDL functions
        void initSystems();
        void update(Settings);
        void handleEvents(Settings);
        void render();
        void clean();
        //SDL get status
        bool isRunning();
        void renderAllPoints();


    private:

        //SDL variables
        SDL_Window *_window;
        SDL_Renderer *_render;
        bool running;
        int mouse_x;
        int mouse_y;
        bool mouseDown = false;

        //velocity pointers
        float *Vx;
        float *Vy;

        //initial velocity pointers
        float *Vx0;
        float *Vy0;

        //density pointers
        float *density;
        float *s;//initial density pointer

        //fluid properties
        int diffusion;
        int viscosity;

        //box properties;
        int size;

        //change in time;
        float dt;

        bool created = false;
};


