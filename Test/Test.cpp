// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#define SDL_MAIN_HANDLED



#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif 



//#include <stdio.h>
#include "include/sdlpp/sdlpp.hpp"


//TODO: ADD sdl2_tff

static const int SCREEN_WIDTH = 1280 / 2;
static const int SCREEN_HEIGHT = 720 / 2;

static const int SCALE = SCREEN_HEIGHT / 4;

sdl::Window w("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
sdl::Renderer r(w.get(), -1, 0);

void game() {
    auto currentTick = SDL_GetTicks() / 14;
    auto playerY = 0;
    auto aiY = 0;
    auto pongX = SCREEN_WIDTH / 2;
    auto pongY = SCREEN_HEIGHT / 2;
    auto pongVX = 5;
    auto pongVY = 5;
    auto aiScore = 0;
    auto playerScore = 0;
    sdl::EventHandler e;
    /*e.mouseMotion = [&playerY](const SDL_MouseMotionEvent& e)
    {
        playerY = e.y - 50;
    };*/
    auto done = false;
    e.quit = [&done](const SDL_QuitEvent&)
    {
        done = true;
    };

    auto paused = false;
    e.keyDown = [&paused](const SDL_KeyboardEvent& e) {
        if (e.keysym.sym == SDLK_ESCAPE) {
            paused = !paused;
        }
    };

    e.windowEvent = [&paused](const SDL_WindowEvent& e) {
        if (e.event == SDL_WINDOWEVENT_MOVED) {
            paused = true;
        }

        if (e.event == SDL_WINDOWEVENT_FOCUS_LOST) {
            paused = true;
        }
    };


    while (!done)
    {
        while (e.poll()) {}
        auto oldTick = currentTick;
        currentTick = SDL_GetTicks() / 15;

        if (paused == false) {
            currentTick = SDL_GetTicks() / 15;
            for (; oldTick < currentTick; ++oldTick)
            {
                auto keystate = SDL_GetKeyboardState(NULL);



                if (keystate[SDL_SCANCODE_UP]) {
                    playerY -= 4;
                }
                if (keystate[SDL_SCANCODE_DOWN]) {
                    playerY += 4;
                }
                if (playerY < 0) {
                    playerY = 0;
                }
                if (playerY > SCREEN_HEIGHT - SCALE) {
                    playerY = SCREEN_HEIGHT - SCALE;
                }
                pongX += pongVX;
                pongY += pongVY;
                if (pongY < 0 && pongVY < 0)
                    pongVY *= -1;
                if (pongY > SCREEN_HEIGHT && pongVY > 0)
                    pongVY *= -1;
                if (pongX < 0  && pongVX < 0)
                {
                    pongVX *= -1;
                    if (pongY < playerY || pongY > playerY + SCALE)
                    {
                        aiScore++;
                        pongX = SCREEN_WIDTH / 2;
                        pongY = SCREEN_HEIGHT / 2;
                    }
                }
                if (pongX > SCREEN_WIDTH  && pongVX > 0)
                {
                    pongVX *= -1;
                    if (pongY < aiY || pongY > aiY + SCALE)
                    {
                        playerScore++;
                        pongX = SCREEN_WIDTH / 2;
                        pongY = SCREEN_HEIGHT / 2;
                    }
                }
                if (aiY + 50 < pongY) {
                    aiY += 4;
                }
                else {
                    aiY -= 4;
                }
                if (aiY < 0) {
                    aiY = 0;
                }
                if (aiY > SCREEN_HEIGHT - SCALE) {
                    aiY = SCREEN_HEIGHT - SCALE;
                }
            }
            r.setDrawColor(0x53, 0xa3, 0x73, 0xff);
            r.clear();
            r.setDrawColor(0xff, 0xff, 0xff, 0xff);
            SDL_Rect rect;

            rect = { 0, playerY, 10, SCALE };
            r.fillRect(&rect);
            rect = { SCREEN_WIDTH - 10, aiY, 10, SCALE };
            r.fillRect(&rect);

            r.setDrawColor(0xd9, 0x48, 0x48, 0xff);
            rect = { pongX - 10, pongY - 10, 20, 20 };
            r.fillRect(&rect);

            r.setDrawColor(0x9c, 0xa8, 0xf7, 0xff);
            for (auto i = 0; i < playerScore; ++i)
            {
                rect = { 20 + (i * 40), 20, 20, 20 };
                r.fillRect(&rect);
            }
            for (auto i = 0; i < aiScore; ++i)
            {
                rect = { SCREEN_WIDTH - 40 - (i * 40), 20, 20, 20 };
                r.fillRect(&rect);
            }
            r.present();
        }
    }
   
#ifdef __EMSCRIPTEN__
    emscripten_cancel_main_loop();    
#endif
}

int main(int argc, char *argv[])
{
    SDL_SetMainReady();
    sdl::Init init(SDL_INIT_EVERYTHING);
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(game, 0, 1);
#else
    game();
#endif 
    SDL_Quit();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
