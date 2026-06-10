#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#define ROOM1 1;


enum Dir{UP, DOWN, LEFT, RIGHT, STATICK};
enum Room{Room1, Room2, Room3, Room4, Game, NONE};


void Create(SDL_Renderer* render, 
    SDL_Texture* Texture1, SDL_Rect* Win1, SDL_Rect* Pos1,
    SDL_Texture* Texture2, SDL_Rect* Win2, SDL_Rect* Pos2,
    SDL_Texture* Texture3, SDL_Rect* Win3, SDL_Rect* Pos3) {
    SDL_RenderCopy(render, Texture1, Win1, Pos1 );
    SDL_RenderCopy(render, Texture2, Win2, Pos2);
    SDL_RenderCopy(render, Texture3, Win3, Pos3);
}
void Animation(SDL_Renderer* render, SDL_Texture* Texture, int speed, int sprite, int curs, SDL_Rect Win, SDL_Rect Pos) {
    Uint32 lastUpdate = SDL_GetTicks();
    Uint32 curTik = SDL_GetTicks();
    if (curTik - lastUpdate > speed) {
        curs = (curs + 1) % sprite;
        Win.x = Win.w * curs;
        lastUpdate = curTik;
    }
    SDL_RenderCopy(render, Texture, &Win, &Pos);
}
void Create(SDL_Renderer* render,
    SDL_Texture* Texture1, SDL_Rect* Win1, SDL_Rect* Pos1) {
    SDL_RenderCopy(render, Texture1, Win1, Pos1);
}
void Destroy(SDL_Texture* Texture1,
    SDL_Texture* Texture2,
    SDL_Texture* Texture3) {
    SDL_DestroyTexture(Texture1);
    SDL_DestroyTexture(Texture2);
    SDL_DestroyTexture(Texture3);
}
void Motion(SDL_Rect &POS, SDL_Event event) {
    POS.x = event.motion.x - POS.w / 2;
    POS.y = event.motion.y - POS.h / 2;
}

/*int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_Window* window = nullptr;
    const int WinX = 1920;
    const int WinY = 1080;
    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WinX, WinY, 0);
    SDL_Renderer* renderer = nullptr;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); // рендер игрового окна

    bool open = true; //  для цикла
    bool isWalk = false;
    const int SPRITE_COUT = 5;
    const int A_SPEED = 350;
    int curSprite = 0;
    int An = 0;
    SDL_Event event;
    bool cardOn1 = false, cardOn2 = false, cardOn3 = false;

    SDL_Surface* cursor = IMG_Load("images/c1.gif");
    SDL_Cursor* cur = SDL_CreateColorCursor(cursor, 0, 0);
    SDL_SetCursor(cur);
    SDL_FreeSurface(cursor);

    SDL_Texture* logo_game = IMG_LoadTexture(renderer, "images/logo-game.png");

    SDL_Rect logo_gameWin = { 0, 0, 1922, 1080 };
    SDL_Rect logo_gamePos = { 0, 0, 1920, 1080 };

    SDL_Texture* logo_cat = IMG_LoadTexture(renderer, "images/logo-cat.png");


    SDL_Rect logo_catWin = { 0, 0, 574, 642 };
    SDL_Rect logo_catPos = { 100, 500, 574, 642 };

    SDL_Texture* cat_Set = IMG_LoadTexture(renderer, "images/cat-set.png");
    SDL_Rect cat_SetWin = { 0, 0, 242, 370 };
    SDL_Rect cat_SetPos = { 1400, 550, 242, 370 };

    SDL_Texture* cat_Ex = IMG_LoadTexture(renderer, "images/cat-ex.png");
    SDL_Rect cat_ExWin = { 1770, 0, 354, 418 };
    SDL_Rect cat_ExPos = { 1532, 600, 354, 418 };

    SDL_Texture* cat_game = IMG_LoadTexture(renderer, "images/cat-game.png");
    SDL_Rect cat_gameWin = { 1770, 0, 354, 418 };
    SDL_Rect cat_gamePos = { 1175, 600, 354, 418 };

    SDL_Texture* Room_1 = IMG_LoadTexture(renderer, "images/Room1.png");
    SDL_Texture* Room_2 = IMG_LoadTexture(renderer, "images/Room2.png");
    SDL_Rect Room_2_Win = { 0, 0, 512, 288 };
    SDL_Rect Room_2_Pos = { 0, 0, WinX, WinY };
    SDL_Rect Room_1_Win = { 0, 0, 128, 128 };
    SDL_Rect Room_1_Pos = { WinX/2 - 1024/2, WinY/2 - 1024/2, 1024, 1024 };


    
    SDL_Texture* Cat_Statick = IMG_LoadTexture(renderer, "images/catstatick.png");
    SDL_Texture* Cat_Side = IMG_LoadTexture(renderer, "images/catside.png");
    SDL_Texture* Cat_Up = IMG_LoadTexture(renderer, "images/catup.png");
    SDL_Texture* Cat_Down = IMG_LoadTexture(renderer, "images/catdown.png");
    SDL_Rect Player_CatWin = { 0, 0, 32, 32 };
    SDL_Rect Player_CatPos = { WinX / 2 - 64 / 2, WinY / 2 - 64 / 2, 200, 200 };

    SDL_Texture* Cat_Gansta = IMG_LoadTexture(renderer, "images/Gangsta-Cat.png");
    SDL_Rect Gansta_CatWin = { 0, 0, 32, 32 };
    SDL_Rect Gansta1_CatPos = { 735, 355, 150, 150 };
    SDL_Rect Gansta2_CatPos = { 1070, 355, 150, 150 };

    bool YCW = true;

    SDL_RendererFlip flip = SDL_FLIP_NONE;

    Uint32 lastUpdate = SDL_GetTicks();

    int dir = 0;
    int Num_Room = NONE;

    while (open) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                open = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (event.button.x >= cat_SetPos.x && event.button.y >= cat_SetPos.y &&
                        event.button.x <= cat_SetPos.x + cat_SetPos.w && event.button.y <= cat_SetPos.y + cat_SetPos.h) {
                        cardOn1 = true;
                    }
                    else if (event.button.x >= cat_ExPos.x && event.button.y >= cat_ExPos.y &&
                        event.button.x <= cat_ExPos.x + cat_ExPos.w && event.button.y <= cat_ExPos.y + cat_ExPos.h) {
                        cat_ExWin.x = 0;
                        cardOn2 = true;
                    }
                    else if (event.button.x >= cat_gamePos.x && event.button.y >= cat_gamePos.y &&
                        event.button.x <= cat_gamePos.x + cat_gamePos.w / 2 && event.button.y <= cat_gamePos.y + cat_gamePos.h / 2) {
                        cat_gameWin.x = 0;
                        cardOn3 = true;
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (cardOn1) {
                        cardOn1 = false;
                    }
                    if (cardOn2) {
                        if (cat_ExPos.x >= logo_catPos.x && cat_ExPos.y >= logo_catPos.y
                            && cat_ExPos.x <= logo_catPos.x + logo_catPos.w / 2 && cat_ExPos.y <= logo_catPos.y + logo_catPos.h / 2) {
                            open = false;
                        }
                        cardOn2 = false;
                    }
                    if (cardOn3) {
                        if (cat_gamePos.x >= logo_catPos.x && cat_gamePos.y >= logo_catPos.y
                            && cat_gamePos.x <= logo_catPos.x + logo_catPos.w / 2 && cat_gamePos.y <= logo_catPos.y + logo_catPos.h / 2) {
                            logo_cat = logo_game = cat_Ex = cat_game = cat_Set = NULL;
                            Num_Room = Room1;
                        }
                        cardOn3 = false;
                    }
                    cat_SetPos = { 1400, 550, 242, 370 };
                    cat_ExPos = { 1532, 600, 354, 418 };
                    cat_gamePos = { 1175, 600, 354, 418 };
                }
                break;
            case SDL_MOUSEMOTION:
                
                if (event.button.x >= logo_catPos.x && event.button.y >= logo_catPos.y &&
                    event.button.x <= logo_catPos.x + logo_catPos.w && event.button.y <= logo_catPos.y + logo_catPos.h) {
                    if (event.button.x > (logo_catPos.x + logo_catPos.w) / 2) {
                        logo_catWin.x = logo_catWin.w * 1;
                    }
                    else {
                        logo_catWin.x = logo_catWin.w * 2;
                    }
                }
                else if (event.button.x <= logo_catPos.x || event.button.y <= logo_catPos.h ||
                    event.button.x >= logo_catPos.x + logo_catPos.w || event.button.y >= logo_catPos.y + logo_catPos.h) {
                    logo_catWin.x = logo_catWin.w * 0;
                }

                if (cardOn1) {
                    Motion(cat_SetPos, event);
                }
                if (cardOn2) {
                    Motion(cat_ExPos, event);
                }
                if (cardOn3) {
                    Motion(cat_gamePos, event);
                }
                break;
            default:
                break;
            }
        }



        SDL_SetRenderDrawColor(renderer, 255, 100, 255, 255);
        SDL_RenderClear(renderer);

        Uint32 curTik = SDL_GetTicks();

        if (curTik - lastUpdate > A_SPEED) {
            curSprite = (curSprite + 1) % SPRITE_COUT;
            logo_gameWin.x = logo_gameWin.w * curSprite;
            if (!cardOn2) {
                cat_ExWin.x = cat_ExWin.w * curSprite + 1770;
            }
            else {
                cat_ExWin.x = cat_ExWin.w * curSprite;
            }
            if (!cardOn3) {
                cat_gameWin.x = cat_gameWin.w * curSprite + 1770;
            }
            else {
                cat_gameWin.x = cat_gameWin.w * curSprite;
            }
            cat_SetWin.x = cat_SetWin.w * curSprite;
            lastUpdate = curTik;
        }
        Create(renderer, logo_game, &logo_gameWin, &logo_gamePos);

        Create(renderer, logo_cat, &logo_catWin, &logo_catPos);

        Create(renderer, cat_Set, &cat_SetWin, &cat_SetPos, cat_Ex, &cat_ExWin, &cat_ExPos, cat_game, &cat_gameWin, &cat_gamePos);

        if (cardOn1) {
            Create(renderer, cat_game, &cat_gameWin, &cat_gamePos, cat_Ex, &cat_ExWin, &cat_ExPos, cat_Set, &cat_SetWin, &cat_SetPos);
        }
        if (cardOn2) {
            Create(renderer, cat_Set, &cat_SetWin, &cat_SetPos, cat_game, &cat_gameWin, &cat_gamePos, cat_Ex, &cat_ExWin, &cat_ExPos);
        }
        if (cardOn3) {
            Create(renderer, cat_Set, &cat_SetWin, &cat_SetPos, cat_Ex, &cat_ExWin, &cat_ExPos, cat_game, &cat_gameWin, &cat_gamePos);
        }
        
        if (logo_game == NULL) {


            dir = STATICK;
            const Uint8* keyboards = SDL_GetKeyboardState(NULL);
            const int MS = 5;
            int X = 0, Y = 0;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);


            if (keyboards[SDL_SCANCODE_A]) {
                flip = SDL_FLIP_HORIZONTAL;
                dir = LEFT;
                X = -MS;
            }
            if (keyboards[SDL_SCANCODE_W]) {
                dir = UP;
                Y = -MS;
            }
            if (keyboards[SDL_SCANCODE_D]) {
                flip = SDL_FLIP_NONE;
                dir = RIGHT;
                X = MS;
            }
            if (keyboards[SDL_SCANCODE_S]) {
                dir = DOWN;
                Y = MS;
            }
            if (keyboards[SDL_SCANCODE_E]) {
                switch (Num_Room) {
                case Room1:
                    if (Player_CatPos.x >= 788 && Player_CatPos.x <= 943 && Player_CatPos.y <= 665 && Player_CatPos.y >= 660) {
                        Player_CatPos.w = 155;
                        Player_CatPos.h = 155;
                        Player_CatPos.x = 20;
                        Player_CatPos.y = 700;
                        Num_Room = Room2;
                    }
                    break;
                case Room2:
                    if ((Player_CatPos.y >= 683 || Player_CatPos.y <= 738) && Player_CatPos.x == 15) {
                        Player_CatPos.w = 200;
                        Player_CatPos.h = 200;
                        Player_CatPos.x = 800;
                        Player_CatPos.y = 650;
                        Num_Room = Room1;
                    }
                    if ((Player_CatPos.x >= 520 || Player_CatPos.x <= 535) && Player_CatPos.y == 775) {
                        Player_CatPos.w = 155;
                        Player_CatPos.h = 155;
                        Player_CatPos.x = 20;
                        Player_CatPos.y = 700;
                        Num_Room = Room3;
                    }
                    if ((Player_CatPos.x >= 1450 || Player_CatPos.x <= 1460) && Player_CatPos.y == 775) {
                        Player_CatPos.w = 155;
                        Player_CatPos.h = 155;
                        Player_CatPos.x = 20;
                        Player_CatPos.y = 700;
                        Num_Room = Room4;
                    }
                    break;
                case Room3:
                    break;
                case Room4:
                    break;
                }
            }

            if (X != 0 || Y != 0) {
                isWalk = true;
            }
            else {
                isWalk = false;
            }

            Player_CatPos.x += X;
            Player_CatPos.y += Y;

            switch (Num_Room) {
            case Room1:

                if (Player_CatPos.x <= 528 ||
                    Player_CatPos.x >= 1193 ||
                    Player_CatPos.y <= 198 ||
                    Player_CatPos.y >= 668 ||
                    (Player_CatPos.y <= 388 && Player_CatPos.x >= 958) ||
                    (Player_CatPos.y <= 568 && Player_CatPos.y >= 323 && Player_CatPos.x <= 648) ||
                    (Player_CatPos.x <= 688 && Player_CatPos.y <= 263)) {
                    YCW = false;
                }
                if (Player_CatPos.x >= 788 && Player_CatPos.x <= 943 && Player_CatPos.y <= 665 && Player_CatPos.y >= 660) {
                    Room_1_Win.x = Room_1_Win.w;
                }
                else {
                    Room_1_Win.x = 0;
                }
                Create(renderer, Room_1, &Room_1_Win, &Room_1_Pos);
                break;
            case Room2:
                if (Player_CatPos.x <= 13 ||
                    Player_CatPos.y >= 780 ||
                    Player_CatPos.x >= 1800 ||
                    Player_CatPos.y <= 360 ||
                    (Player_CatPos.y <= 640 && Player_CatPos.x <= 695) ||
                    (Player_CatPos.y <= 640 && Player_CatPos.x >= 1070)) {
                    YCW = false;
                }
                if (Player_CatPos.y == 365 && ( (Player_CatPos.x >= 700 && Player_CatPos.x <= 765) || (Player_CatPos.x >= 1000 && Player_CatPos.x <= 1065) ) ) {
                    Gansta_CatWin.x = Gansta_CatWin.w * 2;
                }
                if (Player_CatPos.y >= 683 && Player_CatPos.y <= 738 && Player_CatPos.x == 15) {
                    Room_2_Win.x = Room_2_Win.w;
                }
                else if (Player_CatPos.x >= 520 && Player_CatPos.x <= 535 && Player_CatPos.y == 775){
                    Room_2_Win.x = Room_2_Win.w * 3;
                }
                else if (Player_CatPos.x >= 1450 && Player_CatPos.x <= 1460 && Player_CatPos.y == 775) {
                    Room_2_Win.x = Room_2_Win.w * 4;
                }
                else if (Player_CatPos.x >= 805 && Player_CatPos.x <= 980 && Player_CatPos.y == 365) {
                    Room_2_Win.x = Room_2_Win.w * 2;
                }
                
                else {
                    Room_2_Win.x = 0;
                }
                Create(renderer, Room_2, &Room_2_Win, &Room_2_Pos);
                SDL_RenderCopy(renderer, Cat_Gansta, &Player_CatWin, &Gansta1_CatPos);
                SDL_RenderCopyEx(renderer, Cat_Gansta, &Player_CatWin, &Gansta2_CatPos, 0, NULL, SDL_FLIP_HORIZONTAL);
                break;
            }



            if (!YCW) {
                Player_CatPos.x -= X;
                Player_CatPos.y -= Y;
                YCW = true;
            }

            std::cout << "X: " << Player_CatPos.x << " Y: " << Player_CatPos.y << std::endl;



            if (curTik - lastUpdate > 225) {
                An = (An + 1) % 2;
                Player_CatWin.x = Player_CatWin.w * An;
                Gansta_CatWin.x = Gansta_CatWin.w * An;
                lastUpdate = curTik;
            }




            if (isWalk) {
                switch (dir) {
                case LEFT:
                    SDL_RenderCopyEx(renderer, Cat_Side, &Player_CatWin, &Player_CatPos, 0, NULL, flip);
                    break;
                case UP:
                    SDL_RenderCopy(renderer, Cat_Up, &Player_CatWin, &Player_CatPos);
                    break;
                case DOWN:
                    SDL_RenderCopy(renderer, Cat_Down, &Player_CatWin, &Player_CatPos);
                    break;
                case RIGHT:
                    SDL_RenderCopy(renderer, Cat_Side, &Player_CatWin, &Player_CatPos);
                    break;
                default:
                    break;
                }
            }
            else {
                SDL_RenderCopyEx(renderer, Cat_Statick, &Player_CatWin, &Player_CatPos, 0, NULL, flip);
            }
        }
        
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
        }
        
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    }*/



