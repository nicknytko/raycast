//
//  window.cpp
//  raycast
//
//  Created by Nicolas Nytko on 9/23/15.
//  Copyright © 2015 Nicolas Nytko. All rights reserved.
//

#include "base.h"
#include "window.h"
#include "maze.h"

#include "config.h"

bool cWindow::initialize( )
{
    if (SDL_Init( SDL_INIT_EVERYTHING ) < 0)
        return false;
    
    pWindow = SDL_CreateWindow( "3d Maze", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    
    if (pWindow == NULL)
        return false;
    
    pScreen = SDL_GetWindowSurface( pWindow );
    
    if (pScreen == NULL)
        return false;
    
    pBuffer = SDL_CreateRGBSurface( 0, RENDER_W, RENDER_H, 32, 0, 0, 0, 0 );
    
    for (int i=0;i < 353;++i)
    {
        arrbKeys[i] = false;
    }
    
    return true;
}

void cWindow::flipBuffer( )
{
    SDL_BlitScaled( pBuffer, NULL, pScreen, NULL );
}

void cWindow::refresh( )
{
    SDL_UpdateWindowSurface( pWindow );
}

void cWindow::cleanup( )
{
    SDL_DestroyWindow( pWindow );
    SDL_Quit( );
}

bool cWindow::isQuit( )
{
    return bQuit;
}

void cWindow::getInput( )
{
    SDL_Event event;
    
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                bQuit = true;
                break;
            case SDL_KEYDOWN:
            {
                int nKeycode = event.key.keysym.sym;
                
                if (nKeycode >= KEYCODE_GAP_START)
                    nKeycode -= KEYCODE_GAP;
                
                arrbKeys[nKeycode] = true;
                
                break;
            }
            case SDL_KEYUP:
            {
                int nKeycode = event.key.keysym.sym;
                
                if (nKeycode >= KEYCODE_GAP_START)
                    nKeycode -= KEYCODE_GAP;
                
                arrbKeys[nKeycode] = false;
                
                break;
            }
        }
    }
}

bool cWindow::isKeyDown( int nKeycode )
{
    if (nKeycode >= KEYCODE_GAP_START)
        nKeycode -= KEYCODE_GAP;
    
    return arrbKeys[nKeycode];
}

void cWindow::drawRectangle( int x, int y, int w, int h, int r, int g, int b )
{
    SDL_Rect rTemp;
    rTemp.x = x;
    rTemp.y = y;
    rTemp.w = w;
    rTemp.h = h;
    
    if (SDL_FillRect( pScreen, &rTemp, SDL_MapRGB( pScreen->format, r, g, b ) ) < 0)
        printf("error filling rect\n");
}

void cWindow::drawVerticalLine( int x, int h, int r, int g, int b )
{
    SDL_Rect rTemp;
    rTemp.x = x;
    rTemp.y = RENDER_H_HALF - h/2;
    rTemp.w = 1;
    rTemp.h = h;
    
    SDL_FillRect( pBuffer, &rTemp, SDL_MapRGB( pBuffer->format, r, g, b ));
}

void cWindow::drawTexturedVerticalLine( int x, int h, SDL_Surface* pSurface, double u, double gamma )
{
    SDL_Rect rSrc, rDest;
    rSrc.x = int(u * pSurface->w);
    rSrc.y = 0;
    rSrc.w = 1;
    rSrc.h = pSurface->h;

    rDest.x = x;
    rDest.w = 1;
    
    if (h > RENDER_H)
    {
	rSrc.h = pSurface->h - (RENDER_H - h);
	rSrc.y = pSurface->h - rSrc.h;

	rDest.y = 0;
	rDest.h = RENDER_H;
    }
    else
    {
	rDest.y = RENDER_H_HALF - h/2;
	rDest.h = h;
    }
        
    SDL_SetSurfaceColorMod( pSurface, gamma*255, gamma*255, gamma*255);
    SDL_BlitScaled( pSurface, &rSrc, pBuffer, &rDest );
}

void cWindow::clearBuffer( )
{
    SDL_Rect rTemp;
    rTemp.x = 0;
    rTemp.y = 0;
    rTemp.w = RENDER_W;
    rTemp.h = RENDER_H;
    
    SDL_FillRect( pBuffer, &rTemp, NULL );
}

void cWindow::drawMaze( cMaze *pMaze )
{
    for (int y=0;y < pMaze->getHeight();++y)
    {
        for (int x=0;x < pMaze->getWidth();++x)
        {
            if (pMaze->getLevelData()[ x + y*pMaze->getWidth() ] == '#')
            {
                drawRectangle( x * 16, y * 16, 16, 16, 255, 255, 255 );
            }
        }
    }
}

SDL_Surface* cWindow::loadSurface( const char* szFilename )
{
    SDL_Surface* pLoaded = SDL_LoadBMP( szFilename );
    
    if (pLoaded == NULL)
    {
        printf("Could not load image %s.\n", szFilename );
        printf("%s\n", SDL_GetError());
        return NULL;
    }
    
    return SDL_ConvertSurface( pLoaded, pScreen->format, NULL );
}
