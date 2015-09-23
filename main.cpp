#include "base.h"

#include <iostream>
#include <fstream>
#include <unistd.h>

#include "maze.h"
#include "window.h"
#include "config.h"

using namespace std;

class cPlayer
{
public:
    cPlayer( vec2d vSetPos )
    {
        vPos = vec2d( vSetPos.x, vSetPos.y );
        
        nScore = 0;
    }
    
    cPlayer( )
    {
        vPos = vec2d( 0.0, 0.0 );
        
        nScore = 0;
    }
    
    vec2d vPos;
    double dAngle;
    char cSymbol;
    
    int nScore;
};

int main( int argc, char** argv )
{
    cWindow* pWindow = new cWindow();
    if (!pWindow->initialize())
        return 1;
    
    cMaze* pMaze = new cMaze( );
    pMaze->loadFromFile( "maze_data" );
    
    cPlayer* pPlayer = new cPlayer( pMaze->getPlayerSpawn() );
    pPlayer->dAngle = 1;
    
    SDL_Surface* pWallTexture = pWindow->loadSurface( "wall.bmp" );
    
    bool bRunning = true;
    
    while (bRunning)
    {
        pWindow->getInput( );
        
        if (pWindow->isKeyDown(SDLK_LEFT))
            pPlayer->dAngle -= TURN_SPEED;
        
        if (pWindow->isKeyDown(SDLK_RIGHT))
            pPlayer->dAngle += TURN_SPEED;
        
        if (pWindow->isKeyDown(SDLK_UP))
        {
            double dCos = cos(pPlayer->dAngle) * MOVE_SPEED;
            double dSin = sin(pPlayer->dAngle) * MOVE_SPEED;
            
            if (!pMaze->isSolid(pPlayer->vPos.x + dCos,pPlayer->vPos.y))
                pPlayer->vPos.x += dCos;
            
            if (!pMaze->isSolid(pPlayer->vPos.x,pPlayer->vPos.y + dSin))
                pPlayer->vPos.y += dSin;
        }
        if (pWindow->isKeyDown(SDLK_DOWN))
        {
            double dCos = -cos(pPlayer->dAngle) * MOVE_SPEED;
            double dSin = -sin(pPlayer->dAngle) * MOVE_SPEED;
            
            if (!pMaze->isSolid(pPlayer->vPos.x + dCos,pPlayer->vPos.y))
                pPlayer->vPos.x += dCos;
            
            if (!pMaze->isSolid(pPlayer->vPos.x,pPlayer->vPos.y + dSin))
                pPlayer->vPos.y += dSin;
        }
        
        pWindow->clearBuffer();
        
        double dAngleStart = -0.5 * FOV_RAD;
        double dAngleDelta = (1.0 / RENDER_W) * FOV_RAD;
        
        for (int i=0;i < RENDER_W;++i)
        {
            double dAngle = dAngleStart + dAngleDelta * double(i);
            
            srCastResults cr = pMaze->raycast( pPlayer->vPos, dAngle + pPlayer->dAngle );
            
            if (!cr.bHit)
                continue;
            
            double dDistance = cr.dDistance * cos( dAngle );
            double dHeight = RENDER_H_HALF / dDistance;
            double dGamma = clamp((dHeight / RENDER_H_HALF) * 500,0,255);
            if (cr.dNormal == 0 || (cr.dNormal > 3 && cr.dNormal < 4))
                dGamma *= 0.75;
            
            if (cr.cSymbol == 'E')
                pWindow->drawVerticalLine( i, dHeight, 0, dGamma, 0 );
            else
                pWindow->drawTexturedVerticalLine( i, dHeight, pWallTexture, cr.u , (dGamma / 255.0) );
        }
        
        pWindow->flipBuffer();
        
        pWindow->drawMaze( pMaze );
        pWindow->drawRectangle( pPlayer->vPos.x*16 - 4, pPlayer->vPos.y*16 - 4, 8, 8, 0, 0, 255 );
        pWindow->drawRectangle( (pPlayer->vPos.x*16 - 4) + cos(pPlayer->dAngle)*15, (pPlayer->vPos.y*16 - 4) + sin(pPlayer->dAngle)*15, 8, 8, 255, 0, 0 );
        
        pWindow->refresh( );

        SDL_Delay( 16 );
        
        if (pWindow->isQuit())
            bRunning = false;
    }
    
    delete pPlayer;
    delete pMaze;
    
    pWindow->cleanup();
    delete pWindow;
    
    return 0;
}
