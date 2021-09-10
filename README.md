# fluid-sim-in-cpp

## Table of Contents
--
+ overview
+ technologies
+ setup

## Overview
This is a fluid simulation based on the papers of Mike Ash (link below). It is primarly written in c and i refactored it to c++.
I also added a visual render using the SDL library. The simulation written is in two dimensions the paper written by Mike Ash represents
a three dimensional simulation.

## Technologies
1. C++ 11
2. SDL Library

## setup

Below I will provide an article to setup SDL for Code Blocks

Code Blocks - https://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/codeblocks/index.php

code blocks is the easiest way to get this running visual studio is a bit more complicated 
and many guides are outdated.

## Discaimer

The simulation breaks due to fast movements in order to prevent the probability
of this happening I have bounded the velocity of each cell.

```

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
```

these bound the velocity for the y and x direction
although I don't think this is the issue I have found out that
it reduces the probability of the simulation crashing.

The issue might stem from the SDL library itself with the rendering of each cell.

Change at risk of crashing simulation lol.

ps. if you find the bug dont be afraid to create a pull request.

