//// libs
// SDL
// must be
#include <SDL2/SDL.h>
#include <vector>

// png support in SDL2
// need to be installed before using
// #include <SDL2/SDL_image.h>

// optional
// debug/help with some things
// #include <typeinfo>

// callback
#include <iostream>

//// variables
// pass
#define pass (void)0

// global const
static int SCREEN_WIDTH = 640;
static int SCREEN_HEIGHT = 480;
static char* filePath = "john";
static char* flowerPath = "flower";
static char* smilePath = "smile";

// global
bool working = true;
int pressedKey;
int startX = 0;
int startY = 0;
int startWidth = 200;
int startHeight = 200;
int speed = 3;

// global SDL
SDL_Event event;
SDL_Window* window = NULL;

// optimized
SDL_Renderer* renderer = NULL;
std::vector<SDL_Texture*> filesToShow;
std::vector<SDL_Rect> rects;

// not texture
// SDL_Surface* screen_surface = NULL;
// SDL_Surface* filesToShow[] = {NULL};
// std::vector<SDL_Surface*> filesToShow;

//// functions
// use Init()
bool Init();

// use Load(filePath)
bool Load(char* path);

// use addRect(x, y, width, height)
void addRect(int x, int y, int width, int height);

// use Quit()
void Quit();

int main(int argc, char **argv) {
    // init
    if (!Init()) {
        // iostream
        std::cout << "Init() failed" << std::endl;
        return 1;
    }
    if (!Load(filePath)) {
        // iostream
        std::cout << "Load(" << filePath << ") failed" << std::endl;
        return 1;
    }

    if (!Load(smilePath)) {
        // iostream
        std::cout << "Load(" << smilePath << ") failed" << std::endl;
        return 1;
    }

    addRect(
        startX,
        startY,
        startWidth,
        startHeight
    ); // 0 johntangle

    // main process
    while (working) {

        // while user don't do exit
        while (SDL_PollEvent(&event) != 0) if (event.type == SDL_QUIT) working = false;

        // checking whats button is pressed
        // SDLK - SDL Key
        if (event.type = SDL_KEYDOWN) {
            pressedKey = event.key.keysym.sym;

            if (pressedKey == SDLK_UP) {
                rects[0].y -= speed; // john moving

                if (rects[0].w > startWidth) rects[0].w = startWidth; // john resizing
                else if (rects[0].w < startWidth) rects[0].w *= 1.05;

                pass; // pass like python pass
            } else if (pressedKey == SDLK_DOWN) {
                rects[0].y += speed;
                if (rects[0].w > (startWidth / 5)) rects[0].w /= 1.01;
                pass;
            } else if (pressedKey == SDLK_LEFT) {
                rects[0].x -= speed;
                if (rects[0].h > startHeight) rects[0].h = startHeight;
                else if (rects[0].h < startHeight) rects[0].h *= 1.05;
                pass;
            } else if (pressedKey == SDLK_RIGHT) {
                rects[0].x += speed;
                if (rects[0].h > (startHeight / 5)) rects[0].h /= 1.01;
                pass;
            } else if (pressedKey == SDLK_ESCAPE) {
                working = false;
            }
        }

        SDL_SetRenderDrawColor(
            renderer,
            0xFF,
            0xFF,
            0xFF,
            0xFF
        ); // white

        // clear window
        // draw background with color specified in Init -> SetRendererDrawColor
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(
            renderer,
            0x00,
            0x00,
            0x00,
            0x00
        ); // black

        addRect(
            400, // x
            350, // y
            50, // width
            50 // height
        ); // 1 filled rectangle

        SDL_RenderFillRect(renderer, &rects[1]); // filled rectangle

        addRect(
            300, // x
            350, // y
            50, // width
            50 // height
        ); // 2 contour rectangle

        SDL_RenderDrawRect(renderer, &rects[2]); // only contours rectangle

        SDL_RenderDrawLine(
            renderer, // renderer
            10, // pos1 x
            70, // pos1 y
            SCREEN_WIDTH-10, // pos2 x
            70 // pos2 y
        ); // draw line

//         addRect(
//             0, // x
//             0, // y
//             SCREEN_WIDTH / 2, // width
//             SCREEN_HEIGHT / 2 // height
//         ); // 3 left viewport

//         addRect(
//             0, // x
//             SCREEN_HEIGHT / 2, // y
//             SCREEN_WIDTH, // width
//             SCREEN_HEIGHT / 2 // height
//         ); // 4 right viewport

//         SDL_RenderSetViewport(
//             renderer, // renderer
//             &rects[3] // rectangle
//         ); // smile 1

//         // copy a portion of the texture to the current rendering target
//         SDL_RenderCopy(
//             renderer, // renderer
//             filesToShow[1], // file to show
//             NULL, // the source SDL_Rect structure or NULL for the entire texture
//             NULL // the destination SDL_Rect structure or NULL for the entire rendering target;
//                  // the texture will be stretched to fill the given rectangle
//         ); // smile 1

//         SDL_RenderSetViewport(
//             renderer, // renderer
//             &rects[4] // rectangle
//         ); // smile 2

//         // copy a portion of the texture to the current rendering target
//         SDL_RenderCopy(
//             renderer, // renderer
//             filesToShow[1], // file to show
//             NULL, // entire texture
//             NULL // entire rendering target
//         ); // smile 2

        addRect(
            0, // x
            0, // y
            SCREEN_WIDTH, // width
            SCREEN_HEIGHT // height
        ); // 5 start windowed screen
           // 3 if no assigned rectangles for smiles 1,2

        SDL_RenderSetViewport(
            renderer, // renderer
            // change on 5 if smiles` rectangles has been assigned
            // change on 3 if not
            &rects[3] // rectangle
        ); // for john

        SDL_RenderDrawPoint(
            renderer, // renderer
            150, // x
            150 // y
        ); // just point

        // draw one file on window
        SDL_RenderCopy(
            renderer, // renderer
            filesToShow[0], // texture
            NULL, // rectangle in window for draw
            &rects[0] // rectangle coords
        );

        // update window
        SDL_RenderPresent(renderer);

    }

    Quit();
    return 0;
}



bool Init() {
    // check SDL flags for Init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) return false;

    // for PNGs, but don't working for me
//     int flags = IMG_INIT_PNG;
//     if (!(IMG_Init(flags)&flags)) return false;

    window = SDL_CreateWindow(
            "Main не резиновый", // window name
            SDL_WINDOWPOS_UNDEFINED, // window pos on screen X
            SDL_WINDOWPOS_UNDEFINED, // window pos on screen Y
            SCREEN_WIDTH, // window width
            SCREEN_HEIGHT, // window height
            SDL_WINDOW_SHOWN // check SDL flags for CreateWindow
        );

    if (window == NULL) return false;

    renderer = SDL_CreateRenderer(
        window, // window
        -1, // driver index or -1 to initialize the first one supporting the requested flags
        SDL_RENDERER_ACCELERATED | // hardware acceleration
        SDL_RENDERER_PRESENTVSYNC // enable vsync
    );

    // index or -1 to initialize the first one supporting the requested flags
    // renderer flags:
    // SDL_RENDERER_ACCELERATED - hardware acceleration
    // SDL_RENDERER_PRESENTVSYNC - enable vsync
    // SDL_RENDERER_SOFTWARE - the renderer is a software fallback
    // SDL_RENDERER_TARGETTEXTURE - the renderer supports rendering to texture

    // getting info about window surface
//     screen_surface = SDL_GetWindowSurface(window);

    return true;
}

bool Load(char* path) {
    SDL_Surface* fileToShow = NULL;
    // i don't know how to concat path and ext
    char truePath[sizeof(path)];

    for (int i = 0; i < sizeof(path); i++) truePath[i] = path[i];

    // extension
    char ext[] = ".bmp";
//     char ext[] = ".png";

    // for BMPs
    fileToShow = SDL_LoadBMP(strcat(truePath, ext));

    // for PNGs
//     fileToShow = IMG_Load(strcat(truePath, ext));
//     fileToShow = SDL_ConvertSurface(fileToShow, screen_surface->format, NULL);

    if (fileToShow == NULL) return false;

    // if you want you can use vector insert
    filesToShow.push_back(SDL_CreateTextureFromSurface(renderer, fileToShow));

    return true;
}

void addRect(int x, int y, int width, int height) {
    SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;

    rects.push_back(rect);
}

void Quit() {
//// Not texture
//     SDL_FreeSurface(); - delete some surfaces from window

//     for (int i = 0; i < filesToShow.size(); i++) {
//         SDL_FreeSurface(filesToShow[i]);
//     }

//     screen_surface = NULL;

//// Textures
    // SDL_Destroy_Texture()

    for (int i = 0; i < filesToShow.size(); i++) {
        SDL_DestroyTexture(filesToShow[i]);
    }

    filesToShow.clear();

//// Closing programm and free memory

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_Quit();

    // for SDL2_image
//     IMG_Quit();
}

// Numbers as i am learned

//// 4
// SDL_Rect rect;
// working only in functions
// rect.x = startX;
// rect.y = startY;
// rect.w = startWidth;
// rect.h = startHeight;

// window background
// SDL_FillRect(
//     screen_surface, // surface
//     NULL, // rectangle
//     SDL_MapRGB(
//         screen_surface->format,
//         255, 255, 255
//     )
// ); // white

// draw img on window
// SDL_BlitSurface(
//     filesToShow[0], // img
//     NULL, // the SDL_Rect structure representing the rectangle to be copied, or NULL to copy the entire surface
//     screen_surface, // surface
//     &rect // rectangle
// );

// img scaling
// SDL_BlitScaled(
//     filesToShow[0], // img
//     NULL, // copy the entire surface
//     screen_surface, // surface
//     &rect // rectangle
// );

// update window
// SDL_UpdateWindowSurface(window);



//// 3
// SDL_Rect bg_flower;
// bg_flower.w = SCREEN_WIDTH;
// bg_flower.h = SCREEN_HEIGHT;
// bg_flower.x = 0;
// bg_flower.y = 0;

// SDL_BlitScaled(
//     flower, // img
//     NULL, // copy the entire surface
//     scr, // surface
//     &bg_flower // rectangle
// );



//// 2
// SDL_BlitSurface(
//     fileToShow, // img
//     NULL, // copy the entire surface
//     screen_surface, // surface
//     NULL // rectangle
// );

// SDL_UpdateWindowSurface(window);

// freezing window on 2000 ms
// SDL_Delay(2000);



//// 1
    // init all flags
//     if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
//         return 1;
//     }

//     window = SDL_CreateWindow(
//         "Hello, SDL 2!",
//         SDL_WINDOWPOS_UNDEFINED,
//         SDL_WINDOWPOS_UNDEFINED,
//         SCREEN_WIDTH,
//         SCREEN_HEIGHT,
//         SDL_WINDOW_SHOWN
//     ); // Hello, SDL2! the window

//     if (window == NULL) {
//         return 1;
//     }

//     screen_surface = SDL_GetWindowSurface(window);
//     SDL_FillRect(
//         screen_surface, // surface
//         NULL, // the SDL_Rect structure representing the rectangle to fill, or NULL to fill the entire surface
//         SDL_MapRGB(
//             screen_surface->format,
//             0, 255, 0
//         ) // green
//     );

//     SDL_UpdateWindowSurface(window);

//     freeze window on some time in ms
//     SDL_Delay(2000);

//     SDL_DestroyWindow(window)
//     SDL_Quit();
// }
