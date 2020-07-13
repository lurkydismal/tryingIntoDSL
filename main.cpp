//// libs
/// must be
// SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

/// optional
// debug/help with some things
// #include <typeinfo>

/// callback
#include <iostream>

//// variables
// pass
#define pass (void)0

// How many frames time values to keep
// The higher the value the smoother the result is...
// Don't make it 0 or less :)
#define FRAME_VALUES 10

using namespace std;

/// global const
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

/// global
// main process loop
bool working = true;

// loaded place
bool mainMenuLoad = false;
bool playLoad = false;

// for creating text textures
TTF_Font* Arial;

int fontSize = 25;

SDL_Color white;
SDL_Color black;
SDL_Color yellow;
SDL_Color red;
SDL_Color green;

// an array to store frame times
Uint32 frameTimes[FRAME_VALUES];

// last calculated SDL_GetTicks()
Uint32 frameTimeLast;

// total frames rendered
Uint32 frameCount = 0;

/// global places
// mainMenu lvl0
string place = "mainMenu";

/// global SDL
SDL_Event event;
SDL_Window* window;

/// optimized
SDL_Renderer* renderer;
SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
vector<SDL_Texture*> filesToShow;
vector<SDL_Texture*> textToShow;
vector<SDL_Rect> rects;
vector<string> filesPath;

/// classes
struct GoingOn {
        int pressedKey;
        int x;
        int y;
        bool clicked;
} eventGoing ;

class Button {
    private:
        SDL_Rect rect;
        SDL_Texture* idle;
        SDL_Texture* hover;
        int endX;
        int endY;
    public:
        Button(SDL_Rect rect, SDL_Texture* idle, SDL_Texture* hover) {
            this->rect = rect;
            this->endX = rect.x + rect.w;
            this->endY = rect.y + rect.h;
            this->idle = idle;
            if (hover) this->hover = hover;
        }

        bool Update(bool needRect) {
            if (
                ((this->endX > eventGoing.x) && (eventGoing.x > this->rect.x)) &&
                ((this->endY > eventGoing.y) && (eventGoing.y > this->rect.y))
            )
            {
                if (needRect) SDL_RenderCopy(renderer, this->hover, NULL, &this->rect);
                else SDL_RenderCopy(renderer, this->hover, NULL, NULL);
                return true;
            } else {
                if (needRect) SDL_RenderCopy(renderer, this->idle, NULL, &this->rect);
                else SDL_RenderCopy(renderer, this->idle, NULL, NULL);
                return false;
            }
        }
};

vector<Button> buttons;

//// functions
/// main
// use Init()
void Init();

// use Load(filePath)
void Load(string);

// use Quit()
void Quit();

/// useful
// use Sleep(time)
void Sleep(int);

// use addRect(x, y, width, height)
void addRect(int, int, int, int);

// use CreateText(text, font || NULL, color || NULL)
SDL_Texture* CreateText(string, TTF_Font*, SDL_Color);

// use CreateText(text, rect)
void CreateText(string, SDL_Rect);

// use FPS()
float FPS();

/// gameplay
// use MainMenu()
void MainMenu();

// use Play()
void Play();

int main(int argc, char **argv) {
    // init
    Init();

    SDL_Rect fpsRect;
    fpsRect.x = 990;
    fpsRect.y = 5;
    fpsRect.w = 25;
    fpsRect.h = 25;

    // main process
    while (working) {

        // while user don't do exit
        while (SDL_PollEvent(&event) != 0) if (event.type == SDL_QUIT) working = false;

        // checking whats button is going on
        // SDLK - SDL Key
        if (event.type == SDL_KEYUP) {
            eventGoing.pressedKey = event.key.keysym.sym;
            eventGoing.x = NULL;
            eventGoing.y = NULL;
            eventGoing.clicked = false;
            event.type = NULL;
//             cout << SDL_GetScancodeName(event.key.keysym.scancode) << endl << SDL_GetKeyName(event.key.keysym.sym) << endl << endl;

        } else if (event.type == SDL_MOUSEBUTTONUP) {
            eventGoing.x = event.button.x;
            eventGoing.y = event.button.y;
            eventGoing.clicked = true;
            eventGoing.pressedKey = NULL;
            event.type = NULL;
        }

        if (place == "mainMenu") MainMenu();
        else if (place == "lvl0") Play();

//         SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
//         SDL_RenderClear(renderer);
//         cout << trunc(FPS()) << endl;
        int temp_fps = (int) (FPS() + 0.5);
        CreateText(
            to_string(temp_fps),
            fpsRect
        );

        SDL_RenderPresent(renderer);
    }

    Quit();
    return 0;
}



void Init() {
    // check SDL flags for Init
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        cout << SDL_GetError() << endl;
        cout << "Init() failed" << endl;
    }

    // for PNGs
    int flags = IMG_INIT_PNG;
    if (!(IMG_Init(flags)&flags)) {
        cout << IMG_GetError() << endl;
        cout << "Init() failed" << endl;
    }

    if (TTF_Init() == -1) {
        cout << TTF_GetError() << endl;
        cout << "Init() failed" << endl;
    }

    window = SDL_CreateWindow(
            "Main не резиновый", // window name
            SDL_WINDOWPOS_UNDEFINED, // window pos on screen X
            SDL_WINDOWPOS_UNDEFINED, // window pos on screen Y
            SCREEN_WIDTH, // window width
            SCREEN_HEIGHT, // window height
//             SDL_WINDOW_RESIZABLE | // resizable
            SDL_WINDOW_SHOWN | // shown
            SDL_WINDOW_MOUSE_FOCUS // mouse focused
        );

    if (window == NULL) {
        cout << SDL_GetError() << endl;
        cout << "Init() failed" << endl;
    }

    renderer = SDL_CreateRenderer(
        window, // window
        -1, // driver index or -1 to initialize the first one supporting the requested flags
        SDL_RENDERER_ACCELERATED | // hardware acceleration
        SDL_RENDERER_PRESENTVSYNC // enable vsync
    );

    // renderer flags:
    // SDL_RENDERER_ACCELERATED - hardware acceleration
    // SDL_RENDERER_PRESENTVSYNC - enable vsync
    // SDL_RENDERER_SOFTWARE - the renderer is a software fallback
    // SDL_RENDERER_TARGETTEXTURE - the renderer supports rendering to texture

    // Set all frame times to 0ms.
    memset(frameTimes, 0, sizeof(frameTimes));
    frameTimeLast = SDL_GetTicks();

    // for text
    Arial = TTF_OpenFont("other/DejaVuSans.ttf", fontSize);

    if (Arial == NULL) {
        cout << TTF_GetError() << endl;
        cout << "Init() failed" << endl;
    }

    white = {255, 255, 255};
    black = {0, 0, 0};
    yellow = {255, 255, 0};
    red = {255, 0, 0};
    green = {0, 255, 0};
}

void Load(string path) {
    SDL_Surface* fileToShow = NULL;

    // i don't know how to concat path and ext
    char truePath[sizeof(path)];

    for (int i = 0; i < sizeof(path); i++) truePath[i] = path[i];

    // extension
    char ext[] = ".png";

    // for PNGs
    fileToShow = IMG_Load(strcat(truePath, ext));
    fileToShow = SDL_ConvertSurface(fileToShow, format, 0);

    if (fileToShow == NULL) {
        cout << IMG_GetError() << endl;
        cout << "Load(" << path << ") failed" << endl;
    }

    filesToShow.push_back(SDL_CreateTextureFromSurface(renderer, fileToShow));
}

void Quit() {
//// Textures
    // SDL_Destroy_Texture()

    for (int i = 0; i < filesToShow.size(); i++) {
        SDL_DestroyTexture(filesToShow[i]);
    }

    filesToShow.clear();

//// Closing programm

    SDL_DestroyWindow(window);

    SDL_DestroyRenderer(renderer);

    SDL_Quit();
    IMG_Quit();
}

void Sleep(int time) {
    this_thread::sleep_for(chrono::milliseconds(time));
}

void addRect(int x, int y, int width, int height) {
    SDL_Rect rect;

    rect.x = x;
    rect.y = y;
    if (width) rect.w = width;
    if (height) rect.h = height;

    rects.push_back(rect);
}

SDL_Texture* CreateText(string text, TTF_Font* fontLocal, SDL_Color colorLocal) {
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(fontLocal, text.c_str(), colorLocal);
    SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    return message;
}

void CreateText(string text, SDL_Rect dstrect) {
    SDL_RenderCopy(
        renderer,
        CreateText(
            text,
            Arial,
            black
        ),
        NULL,
        &dstrect
    );
}

float FPS() {
    Uint32 frameTimesIndex;
    Uint32 getTicks;
    Uint32 count;
    // the value you want
    float fps = 0;

    // frametimesindex is the position in the array. It ranges from 0 to FRAME_VALUES.
    // This value rotates back to 0 after it hits FRAME_VALUES.
    frameTimesIndex = frameCount % FRAME_VALUES;

    // store the current time
    getTicks = SDL_GetTicks();

    // save the frame time value
    frameTimes[frameTimesIndex] = getTicks - frameTimeLast;

    // save the last frame time for the next fpsthink
    frameTimeLast = getTicks;

    // increment the frame count
    frameCount++;

    // Work out the current framerate

    // The code below could be moved into another function if you don't need the value every frame.

    // I've included a test to see if the whole array has been written to or not. This will stop
    // strange values on the first few (FRAME_VALUES) frames.
    if (frameCount < FRAME_VALUES) count = frameCount;
    else count = FRAME_VALUES;

    // add up all the values and divide to get the average frame time.
    for (Uint32 i = 0; i < count; i++) fps += frameTimes[i];

    fps /= count;

    // now to make it an actual frames per second value...
    fps = 1000.f / fps;

    return fps;
}

// mainMenu
void MainMenu() {
    if (!mainMenuLoad) {
        rects.clear();
        filesToShow.clear();

        // buttons
        addRect(398, 472, 229, 95); // 0
        addRect(401, 575, 223, 73); // 1

        Load("mainMenu/idle_play"); // 0
//         Load("mainMenu/hover_play"); // 1
        Load("mainMenu/idle_exit"); // 2 || 1
//         Load("mainMenu/hover_exit"); // 3

        Button temp_button1(rects[0], filesToShow[0], nullptr);
        buttons.push_back(temp_button1); // play - 0

        Button temp_button2(rects[1], filesToShow[1], nullptr);
        buttons.push_back(temp_button2); // exit - 1

        mainMenuLoad = true;
        rects.clear();
        filesToShow.clear();

        // filesToShow[0] - bg
        Load("mainMenu/bg");
    }

    SDL_RenderCopy(renderer, filesToShow[0], NULL, NULL);

    // false - no rects for buttons
    if (buttons[0].Update(true)) {
        mainMenuLoad = false;
        place = "lvl0";
    } // play
    else if (buttons[1].Update(true)) {
        working = false;
    } // exit
}

// lvl0
void Play() {
    if (!playLoad) {
        rects.clear();
        filesToShow.clear();

        // filesToShow[0] - bg
        Load("lvl0/bg");

        // text box
        addRect(floor((SCREEN_WIDTH-575)/2), floor(SCREEN_HEIGHT-150)/1.8, 585, 250); // 0
        addRect(floor((SCREEN_WIDTH-575)/2), floor(SCREEN_HEIGHT-150)/1.2, 585, 150); // 1
        Load("other/text_box"); // 1

        // text
        addRect((rects[0].x+15), (rects[0].y+20), (rects[0].w-30), fontSize); // 2
        addRect((rects[1].x+15), (rects[1].y+15), (rects[1].w-30), fontSize); // 3
        addRect((rects[0].x+15), (rects[0].y+20+fontSize+10), (rects[0].w-30), fontSize); // 4
        addRect((rects[1].x+15), (rects[1].y+15+fontSize+7), (rects[1].w-30), fontSize); // 5

        Load("lvl0/text/test"); // 2
        Load("lvl0/text/test"); // 3

        playLoad = true;
    }

    SDL_RenderCopy(renderer, filesToShow[0], NULL, NULL);

//     SDL_RenderCopy(renderer, filesToShow[1], NULL, &rects[0]); // box 1

//     SDL_RenderCopy(renderer, filesToShow[2], NULL, &rects[2]); // text 1

//     SDL_RenderCopy(renderer, filesToShow[2], NULL, &rects[4]); // text 2

    SDL_RenderCopy(renderer, filesToShow[1], NULL, &rects[1]); // box 2

    SDL_RenderCopy(renderer, filesToShow[3], NULL, &rects[3]); // text 1

    SDL_RenderCopy(renderer, filesToShow[3], NULL, &rects[5]); // text 2
}