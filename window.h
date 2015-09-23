//
//  window.h
//  raycast
//
//  Created by Nicolas Nytko on 9/23/15.
//  Copyright © 2015 Nicolas Nytko. All rights reserved.
//

#ifndef window_h
#define window_h

class cWindow
{
private:
    SDL_Window* pWindow;
    SDL_Surface* pScreen;
    SDL_Surface* pBuffer;
    
    bool arrbKeys[353];
    const int KEYCODE_GAP = 1073741753;
    const int KEYCODE_GAP_START = 1073741881;
    
    bool bQuit = false;
public:
    bool initialize( );
    void cleanup( );
    
    void flipBuffer( );
    void refresh( );
    
    bool isQuit( );
    void getInput( );
    bool isKeyDown( int nKeycode );
    
    void drawMaze( class cMaze *pMaze );
    void drawRectangle( int x, int y, int w, int h, int r=0, int g=0, int b=0 );
    void drawVerticalLine( int x, int h, int r=0, int g=0, int b=0 );
    void drawTexturedVerticalLine( int x, int h, SDL_Surface* pSurface, double u, double gamma );
    
    void clearBuffer( );
    SDL_Surface* loadSurface( const char* szFilename );
};

#endif /* window_h */
