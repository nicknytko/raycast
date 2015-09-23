//
//  maze.h
//  raycast
//
//  Created by Nicolas Nytko on 9/23/15.
//  Copyright © 2015 Nicolas Nytko. All rights reserved.
//

#ifndef maze_h
#define maze_h

#include <string>

struct srCastResults
{
    double dDistance, dNormal;
    double x, y, u;
    bool bHit;
    char cSymbol;
};

class cMaze
{
private:
    vec2d vDim;
    char* arrcData;
    
    void initArrays( int width, int height );
    
public:
    vec2d vPlayerSpawn;
    
    cMaze( );
    ~cMaze();
    
    void loadFromFile( std::string filename );
    char getCharAt( int x, int y );
    bool isSolid( int x, int y );
    srCastResults raycast( vec2d vOrigin, double dAngle );
    
    int getWidth(){ return vDim.x; }
    int getHeight(){ return vDim.y; }
    vec2d getDimensions(){ return vDim; }
    vec2d getPlayerSpawn(){ return vPlayerSpawn; }
    
    char* getLevelData(){ return arrcData; }
};


#endif /* maze_h */
