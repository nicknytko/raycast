//
//  base.h
//  raycast
//
//  Created by Nicolas Nytko on 9/23/15.
//  Copyright © 2015 Nicolas Nytko. All rights reserved.
//

#ifndef base_h
#define base_h

#include <cmath>
#include "SDL2/SDL.h"

#define PI 3.14159

struct vec2d
{
    double x, y;
    
    vec2d()
    {
        x = 0.0;
        y = 0.0;
    }
    
    vec2d( double nX, double nY )
    {
        x = nX;
        y = nY;
    }
};

inline double sign( double x )
{
    if (x > 0)
        return 1;
    if (x < 0)
        return  -1;
    return 0;
}

inline double clamp( double x, double low, double high )
{
    if (x < low)
        return low;
    
    if (x > high)
        return high;
    
    return x;
}

#endif /* base_h */
