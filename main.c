#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <unistd.h>

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;

int startX, startY, dW, dH;
int wX, wY;

bool init() 
{
    if (!SDL_Init(SDL_INIT_VIDEO)) return false;

    window = SDL_CreateWindow("DVD", 640, 480, 0);
    if (!window) return false;

    screen = SDL_GetWindowSurface(window);
    return true;
}

void updateBounds()
{
    int count;
    SDL_DisplayID* displays = SDL_GetDisplays(&count);
    if (displays) {
        SDL_Rect usable;
        SDL_GetDisplayUsableBounds(displays[0], &usable);
        
        startX = usable.x - 10;
        startY = usable.y - 10;
        dW = usable.w - 10;
        dH = usable.h - 10;

        SDL_free(displays);
    }
}

void moveWindow() 
{
    static int velX = 1;
    static int velY = 1;
    const int winW = 640;
    const int winH = 480;

    wX += velX;
    wY += velY;

    if (wX + winW >= startX + dW) {
        wX = startX + dW - winW;
        velX *= -1;
    } else if (wX <= startX) {
        wX = startX;
        velX *= -1;
    }

    if (wY + winH >= startY + dH) {
        wY = startY + dH - winH;
        velY *= -1;
    } else if (wY <= startY) {
        wY = startY;
        velY *= -1;
    }

    SDL_SetWindowPosition(window, wX, wY);
}

void mainLoop() 
{
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) quit = true;
        }
        SDL_FillSurfaceRect(screen, NULL, SDL_MapSurfaceRGB(screen, 255, 255, 255));
        SDL_UpdateWindowSurface(window);
        moveWindow();
    }
}

int main(int argc, char** argv) 
{
    if (!init()) return -1;
    
    updateBounds();
    mainLoop();
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
