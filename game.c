#include <stdio.h>
#include <time.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

// Create a "Robot" structure to store data about your robots.
typedef struct {
  char *name;
  int life, x, y;
} Robot;

// Create a "Boot" structure to store data about your boots.
typedef struct {
  int x, y;
} Boot;

// Create a "GameState" structure to store all data in the game.
typedef struct {
  SDL_Renderer *renderer;
  Robot eve;
  Robot wallE;
  Boot boots[10];
  SDL_Texture *eveTexture;
  SDL_Texture *walleTexture;
  SDL_Texture *bootTexture;
} GameState;

// Initialize game details.
void loadGame(GameState *gameState) {
  // Instantiate GameState objects.
  gameState->eve.x = 120;
  gameState->eve.y = 40;
  gameState->wallE.x = 300;
  gameState->wallE.y = 175;

  for (int i = 0; i < 10; i++) {
    gameState->boots[i].x = i*75;
    gameState->boots[i].y = 425;
  }

  SDL_Surface *eveSurface;
  SDL_Surface *walleSurface;
  SDL_Surface *bootSurface;

  // Create an SDL Surface from the Eve image. If not found, kill the game.
  eveSurface = IMG_Load("assets/eve.png");
  if (eveSurface == NULL) {
    printf("Cannot find Eve image.\n");
    SDL_Quit();
    exit(1);
  }

  // Create an SDL Surface from the Wall-E image. If not found, kill the game.
  walleSurface = IMG_Load("assets/wallE.png");
  if (walleSurface == NULL) {
    printf("Cannot find Wall-E image.\n");
    SDL_Quit();
    exit(1);
  }

  // Create an SDL Surface from the Boot image. If not found, kill the game.
  bootSurface = IMG_Load("assets/boot.png");
  if (bootSurface == NULL) {
    printf("Cannot find Boot image.\n");
    SDL_Quit();
    exit(1);
  }

  // Create an SDL Texture from your Surface and store it in the GameState structure.
  gameState->eveTexture = SDL_CreateTextureFromSurface(gameState->renderer, eveSurface);
  gameState->walleTexture = SDL_CreateTextureFromSurface(gameState->renderer, walleSurface);
  gameState->bootTexture = SDL_CreateTextureFromSurface(gameState->renderer, bootSurface);

  // Free up memory from the Surface since you now have the Texture.
  SDL_FreeSurface(eveSurface);
  SDL_FreeSurface(walleSurface);
  SDL_FreeSurface(bootSurface);
}

// Process Events function used in the Event Loop.
int processEvents(SDL_Window *window, GameState *gameState) {
  // Use SDL Event instance to check for keypresses, etc. and set "done" pointer to return to the Event Loop.
  SDL_Event event;
  int done = 0;

  // Looping through all Events on every frame of the main Event Loop. Switch on the Event's "Type."
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_WINDOWEVENT_CLOSE: {
        if (window) {
          SDL_DestroyWindow(window);
          window = NULL;
          done = 1;
        }
      } break;
      case SDL_KEYDOWN: {
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE: {
            done = 1;
          } break;
          case SDLK_LEFT: {
            gameState->eve.x -=5;
          } break;
          case SDLK_RIGHT: {
            gameState->eve.x += 5;
          } break;
          case SDLK_UP: {
            gameState->eve.y -= 5;
          } break;
          case SDLK_DOWN: {
            gameState->eve.y += 5;
          } break;
        }
      } break;
      case SDL_QUIT: {
        done = 1;
      } break;
    }
  }

  // Check for held down keys on every event.
  const Uint8 *state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_LEFT]) {
    gameState->eve.x -= 5;
  }
  if (state[SDL_SCANCODE_RIGHT]) {
    gameState->eve.x += 5;
  }
  if (state[SDL_SCANCODE_UP]) {
    gameState->eve.y -= 5;
  }
  if (state[SDL_SCANCODE_DOWN]) {
    gameState->eve.y += 5;
  }

  // Return "Done" value.
  return done;
}

// Render function used in the Event Loop.
void render(GameState *gameState) {
  // Set the Renderer's color and "clear" the screen with that color.
  SDL_SetRenderDrawColor(gameState->renderer, 100, 125, 255, 255);
  SDL_RenderClear(gameState->renderer);

  // Set a Rect for the Boot image/surface and render.
  for (int i = 0; i < 10; i++) {
    SDL_Rect bootRect = { gameState->boots[i].x, gameState->boots[i].y, 35, 40 };
    SDL_RenderCopy(gameState->renderer, gameState->bootTexture, NULL, &bootRect);
  }

  // Set a Rect for the Wall-E image/surface and render.
  SDL_Rect walleRect = { gameState->wallE.x, gameState->wallE.y, 55, 60 };
  SDL_RenderCopy(gameState->renderer, gameState->walleTexture, NULL, &walleRect);

  // Set a Rect for the Eve image/surface and render.
  SDL_Rect eveRect = { gameState->eve.x, gameState->eve.y, 55, 60 };
  SDL_RenderCopy(gameState->renderer, gameState->eveTexture, NULL, &eveRect);

  // Present the draw to the screen.
  SDL_RenderPresent(gameState->renderer);
}

// Main program function.
int main(int argc, char const *argv[]) {
  // Initialize random number capability with current time as seed.
  srandom((int)time(NULL));

  // Initialize GameState structure.
  GameState gameState;

  // Create refs for window, renderer, and other assets needed for runtime.
  SDL_Window *window;
  SDL_Renderer *renderer;

  // Using the SDL Video resource.
  SDL_Init(SDL_INIT_VIDEO);

  // Create window and renderer.
  window = SDL_CreateWindow("It's Wall-E, Bitches", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  gameState.renderer = renderer;

  loadGame(&gameState);

  // Set a pointer for your Event Loop.
  int done = 0;

  // Begin Event Loop. Process your events and render to the screen on each frame.
  while (!done) {
    done = processEvents(window, &gameState);
    render(&gameState);
  }

  // After Event Loop exits, destroy all assets before quitting the program.
  SDL_DestroyTexture(gameState.eveTexture);
  SDL_DestroyTexture(gameState.walleTexture);
  SDL_DestroyTexture(gameState.bootTexture);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);

  SDL_Quit();

  return 0;
}
