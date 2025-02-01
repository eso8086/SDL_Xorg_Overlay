//
// Created by el on 2/1/25.
//
#define SDL_MAIN_USE_CALLBACKS 1
#include "SDL3/SDL.h"
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>
#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xfixes.h>

#include <iostream>


#define WINDOW_W  1920
#define WINDOW_H  1080

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static Display* xdisplay = nullptr;
static Window xwindow;

void setXWindowShapeRegion(int x_off,
                           int y_off,
                           short rect_x,
                           short rect_y,
                           unsigned short rect_width,
                           unsigned short rect_height){
    XRectangle rect {rect_x, rect_y, rect_width, rect_height};
    XserverRegion region = XFixesCreateRegion(xdisplay, &rect, 1);
    XFixesSetWindowShapeRegion(xdisplay, xwindow, ShapeInput, x_off, y_off, region);
    XFixesDestroyRegion(xdisplay, region);
}


//startup
SDL_AppResult SDL_AppInit(void **appstate, int argc, char* argv[]){

    SDL_SetAppMetadata("My OpenGL App", "0.1", "com.example.opengl");

    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if(!SDL_CreateWindowAndRenderer("OpenGL",
                                    WINDOW_W,
                                    WINDOW_H,
                                    SDL_WINDOW_TRANSPARENT |
                                    SDL_WINDOW_BORDERLESS |
                                    SDL_WINDOW_ALWAYS_ON_TOP |
                                    SDL_WINDOW_UTILITY,
                                    &window,
                                    &renderer)){
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }


    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0) {
        xdisplay = (Display *)SDL_GetPointerProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
        if(xdisplay == nullptr){
            SDL_Log("Couldn't get xdisplay pointer");
            return SDL_APP_FAILURE;
        }
        xwindow = (Window)SDL_GetNumberProperty(SDL_GetWindowProperties(window), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
    }

    setXWindowShapeRegion(0, 0, 0, 0, 0, 0);

    return SDL_APP_CONTINUE;
}

//events
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event *event){
    if(event->type == SDL_EVENT_QUIT){
        return SDL_APP_SUCCESS;
    }
    if(event->type == SDL_EVENT_KEY_DOWN){
        if(event->key.key == SDLK_ESCAPE){
            return SDL_APP_SUCCESS;
        }
    }
    return SDL_APP_CONTINUE;
}

//frame
SDL_AppResult SDL_AppIterate(void* appstate){
//    const double now = ((double) SDL_GetTicks()) / 1000.0;
//    const auto red = (float) (0.5 + 0.5 * SDL_sin(now));
//    const auto green = (float) (0.5 + 0.5 * SDL_sin(SDL_PI_D * 2 / 3));
//    const auto blue = (float) (0.5 + 0.5 * SDL_sin(SDL_PI_D * 4 / 3));
//    SDL_SetRenderDrawColorFloat(renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);
//    SDL_RenderClear(renderer);


    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 50);

    SDL_FRect rect {200, 200, 200, 200};
    SDL_RenderFillRect(renderer, &rect);

    SDL_FPoint point {100, 100, };
    SDL_RenderPoint(renderer, point.x, point.y);


    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;
}

//clean up
void SDL_AppQuit(void* appstate, SDL_AppResult result){
    //clean up if needed
}
