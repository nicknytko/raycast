//
//  maze.cpp
//  raycast
//
//  Created by Nicolas Nytko on 9/23/15.
//  Copyright © 2015 Nicolas Nytko. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "base.h"
#include "maze.h"

using namespace std;

cMaze::cMaze( )
{
    vPlayerSpawn = vec2d( 0, 0 );
}

cMaze::~cMaze()
{
    if (arrcData != NULL)
        delete[] arrcData;
}

void cMaze::initArrays( int width, int height )
{
    vDim = vec2d( width, height );
    arrcData = new char[ width * height ];
    
    for (int i=0;i < (width * height);++i)
    {
        arrcData[i] = ' ';
    }
}

char cMaze::getCharAt( int x, int y )
{
    if (x < 0)
        return '#';
    if (y < 0)
        return '#';
    if (x >= vDim.x)
        return '#';
    if (y >= vDim.y)
        return '#';
    
    return arrcData[ int( (y * vDim.x) + x ) ];
}

bool cMaze::isSolid( int x, int y )
{
    char c = getCharAt( x, y );
    
    if (c == 'E')
        return false;
    
    if (c == ' ')
        return false;
    
    return true;
}

void cMaze::loadFromFile( string filename )
{
    ifstream fFile;
    fFile.open( filename.c_str() );
    
    int nWidth, nHeight;
    fFile >> nWidth;
    fFile >> nHeight;
    
    initArrays( nWidth, nHeight );
    
    int nIterator = 0;
    char c;
    while ( fFile.get( c ) )
    {
        switch (c)
        {
            case '\n':
                continue;
            case '@':
                vPlayerSpawn.y = (int)(nIterator / vDim.x);
                vPlayerSpawn.x = (nIterator - (int)(vPlayerSpawn.y * vDim.x)) + 0.5;
                vPlayerSpawn.y += 0.5;
                
                nIterator++;
                break;
            default:
                arrcData[nIterator++] = c;
                break;
        }
    }
    
    fFile.close();
}

srCastResults cMaze::raycast( vec2d vOrigin, double dAngle )
{
    double dRayY = sin( dAngle ), dRayX = cos( dAngle );
    int nGridX = int( vOrigin.x ), nGridY = int( vOrigin.y );
    double dHor, dVer;
    
    double dDeltaX = sqrt( 1 + (dRayY*dRayY) / (dRayX*dRayX) );
    double dDeltaY = sqrt( 1 + (dRayX*dRayX) / (dRayY*dRayY) );
    
    int nStepX = sign( dRayX );
    int nStepY = sign( dRayY );
    
    if (dRayX < 0)
        dHor = (vOrigin.x - (double)nGridX) * dDeltaX;
    else
        dHor = (((double)nGridX + 1) - vOrigin.x) * dDeltaX;
    
    if (dRayY < 0)
        dVer = (vOrigin.y - (double)nGridY) * dDeltaY;
    else
        dVer = (((double)nGridY + 1) - vOrigin.y) * dDeltaY;
    
    bool bHit = false;
    bool bHor = false;
    int nLoops = 0;
    char cSymbol = 0;
    
    while (!bHit)
    {
        if (dHor < dVer)
        {
            dHor += dDeltaX;
            nGridX += nStepX;
            bHor = true;
        }
        else
        {
            dVer += dDeltaY;
            nGridY += nStepY;
            bHor = false;
        }
        
        cSymbol = getCharAt( nGridX, nGridY );
        if (cSymbol != ' ')
            bHit = true;
        
        nLoops++;
        if (nLoops > 30)
            break;
    }
    
    srCastResults crReturn;
    crReturn.bHit = bHit;
    
    if (!bHit)
        return crReturn;
    
    crReturn.cSymbol = cSymbol;
    
    if (bHor)
    {
        double dSideX = nGridX;
        
        if (nStepX < 0)
        {
            crReturn.dNormal = 0;
            dSideX++;
        }
        else
            crReturn.dNormal = PI;
        
        crReturn.x = dSideX;
        crReturn.y = vOrigin.y + ( tan( dAngle ) * ( dSideX - vOrigin.x ) );
        crReturn.u = crReturn.y - nGridY;
    }
    else
    {
        double dSideY = nGridY;
        
        if (nStepY < 0)
        {
            crReturn.dNormal = PI * 1.5;
            dSideY++;
        }
        else
            crReturn.dNormal = PI/2;
        
        crReturn.y = dSideY;
        crReturn.x = vOrigin.x + (tan( dAngle + 1.57079632679 ) * ( vOrigin.y - dSideY ));
        crReturn.u = crReturn.x - nGridX;
    }
    
    crReturn.dDistance = sqrt( pow(crReturn.y - vOrigin.y, 2) + pow(crReturn.x - vOrigin.x, 2) );
    
    return crReturn;
}