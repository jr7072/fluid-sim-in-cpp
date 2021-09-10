#pragma once
#include "structs.h"
#include "Fluid.h"
#include <cmath>


int IX(int x, int y, int N){

    return x + y * N;
}

void set_bnd(int b, float *x, int N){

    for (int i = 1; i < N; i++){
        x[IX(i, 0, N)] = b == 2 ? -x[IX(i, 1, N)] : x[IX(i, 1, N)];
        x[IX(i, N-1, N)] = b == 2 ? -x[IX(i, N-2, N)] : x[IX(i, N-2, N)];
    }

    for (int j = 1; j < N-1; j++){

        x[IX(0 , j, N)] = b == 1 ? -x[IX(1, j, N)] : x[IX(1, j, N)];
        x[IX(N - 1, j, N)] = b == 1 ? -x[IX(N-2, j, N)] : x[IX(N-2, j, N)];
    }

    x[IX(0, 0, N)] = 0.5f * (x[IX(0, 1, N)] + x[IX(1, 0, N)]);
    x[IX(0, N-1, N)] = 0.5f * (x[IX(1, N-1, N)] + x[IX(0, N-2, N)]);
    x[IX(N-1, 0, N)] = 0.5f * (x[IX(N-2, 0, N)] + x[IX(N-1, 1, N)]);
    x[IX(N-1, N-1, N)] = 0.5f * (x[IX(N-1, N-2, N)] + x[IX(N-2, N-1, N)]);
}

void lin_solve(int b, float *x, float *x0, float a, float c, Settings setting){

    float cRecip = 1.0 / c;

    for (int k = 0 ; k < setting.iter; k++){
        for (int j = 1; j < setting.N - 1; j++){
            for (int i = 1; i < setting.N - 1; i++){
                x[IX(i, j, setting.N)] = (x0[IX(i, j, setting.N)] + a * (x[IX(i + 1, j, setting.N)]+
                                x[IX(i - 1, j, setting.N)]+ x[IX(i, j + 1, setting.N)]+
                                x[IX(i, j - 1, setting.N)])) * cRecip;
            }
        }

        set_bnd (b, x, setting.N);
    }
}

void project(float *velocX, float *velocY, float *p, float *div, Settings setting){

    for (int j = 1; j < setting.N - 1; ++j){
        for (int i = 1; i < setting.N -1; ++i){
            div[IX(i, j, setting.N)] = -0.5f * (
                        velocX[IX(i+1, j, setting.N)]
                        -velocX[IX(i-1, j, setting.N)]
                        +velocY[IX(i, j+1, setting.N)]
                        -velocY[IX(i, j-1, setting.N)]
                        )/ setting.N;
            p[IX(i, j, setting.N)] = 0;
        }
    }

    set_bnd(0, div, setting.N);
    set_bnd(0, p, setting.N);

    lin_solve(0, p, div, 1, 6, setting);

    for (int j = 0; j < setting.N - 1; ++j){
        for (int i = 0; i < setting.N - 1; ++i){

            velocX[IX(i, j, setting.N)] -= 0.5f * (p[IX(i+1, j, setting.N)]
                                                   -p[IX(i-1, j, setting.N)]) * setting.N;
            velocY[IX(i, j, setting.N)] -= 0.5f * (p[IX(i, j+1, setting.N)]
                                                   -p[IX(i, j-1, setting.N)]) * setting.N;
        }
    }

    set_bnd(1, velocX, setting.N);
    set_bnd(2, velocY, setting.N);

}

void advect(int b, float *d, float *d0, float *velocX, float *velocY, float dt, Settings setting){

    float i0, i1, j0, j1;
    int N = setting.N;

    float dtx = dt * (N - 2);
    float dty = dt * (N - 2);

    float s0, s1, t0, t1;
    float tmp1, tmp2, x, y;

    float Nfloat = N;
    float ifloat, jfloat;
    int i, j;

    for(j = 1, jfloat = 1; j < N - 1; j++, jfloat++){
        for (i = 1, ifloat = 1; i < N - 1; i++, ifloat++){

            tmp1 = dtx * velocX[IX(i, j, N)];
            tmp2 = dty * velocY[IX(i, j, N)];
            x = ifloat - tmp1;
            y = jfloat - tmp2;

            if(x < 0.5f) x = 0.5f;
            if(x > Nfloat + 0.5f) x = Nfloat + 0.5f;
            i0 = floor(x);
            i1 = i0 + 1.0f;
            if(y < 0.5f) y = 0.5f;
            if(y > Nfloat + 0.5f) y = Nfloat + 0.5f;
            j0 = floor(y);
            j1 = j0 + 1.0f;

            s1 = x - i0;
            s0 = 1.0f - s1;
            t1 = y - j0;
            t0 = 1.0f - t1;

            int i0i = int(i0);
            int i1i = int(i1);
            int j0i = int(j0);
            int j1i = int(j1);

            d[IX(i, j, N)] =

                s0 * (t0 * d0[IX(i0i, j0i, N)] + t1 * d0[IX(i0i, j1i, N)])
                + s1 * (t0 * d0[IX(i1i, j0i, N)] + t1 * d0[IX(i1i, j1i, N)]);
        }
    }

    set_bnd(b, d, N);

}

void diffuse(int b, float *x, float *x0, float diff, float dt, Settings setting){

    float a = dt * diff * (setting.N - 2) * (setting.N - 2);
    lin_solve(b, x, x0, a, 1 + 6 * a, setting);
}


