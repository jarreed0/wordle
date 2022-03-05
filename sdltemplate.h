#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>

namespace sdltemplate {

SDL_Color bkg;
SDL_Color blue = {0,0,255,255};

const int FONT_SIZE = 32;
TTF_Font* font;

char *title = "SDL2 Template";
int width = 500, height = 500;
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Texture *screen;
SDL_Rect screensize;
int frameCount, timerFPS, lastFrame, fps, lastTime;
int setFPS = 60;
SDL_Point mouse;
const Uint8 *keystates;
Uint32 mousestate;
SDL_Event event;
bool running;

void init() {
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
  SDL_Init(SDL_INIT_EVERYTHING);
  window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                       width, height, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
  running = 1;
  screensize.x=screensize.y=0;
  screensize.w=width;screensize.h=height;
  screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
  TTF_Init();
  font = TTF_OpenFont("Peepo.ttf", FONT_SIZE);
}

void init(char *t, int w, int h) {
  title = t;
  width = w;
  height = h;
  init();
}

void input() {
  keystates = SDL_GetKeyboardState(NULL);
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT)
      running = false;
  }
  mousestate = SDL_GetMouseState(&mouse.x, &mouse.y);
}

SDL_Color createColor(int r, int g, int b, int a) {
  SDL_Color c;
  c.r = r;
  c.g = g;
  c.b = b;
  c.a = a;
  return c;
}

SDL_Rect tmp;
void drawRectOutline(int x, int y, int w, int h) {
  tmp.x = x;
  tmp.y = y;
  tmp.w = w;
  tmp.h = h;
  SDL_RenderDrawRect(renderer, &tmp);
}
void drawRectFilled(int x, int y, int w, int h) {
  tmp.x = x;
  tmp.y = y;
  tmp.w = w;
  tmp.h = h;
  SDL_RenderFillRect(renderer, &tmp);
}

void setDrawColor(SDL_Color c) {
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
}

void begin_render() {
  SDL_SetRenderTarget(renderer, screen);
  SDL_SetRenderDrawColor(renderer, bkg.r, bkg.g, bkg.b, bkg.a);
  SDL_RenderClear(renderer);
  frameCount++;
  timerFPS = SDL_GetTicks()-lastFrame;
  if(timerFPS<(1000/setFPS)) {
   SDL_Delay((1000/setFPS)-timerFPS);
  }
}

void end_render() {
  SDL_SetRenderTarget(renderer, NULL);
  SDL_RenderCopy(renderer, screen, &screensize, &screensize);
  SDL_RenderPresent(renderer);
}

void quit() {
  TTF_CloseFont(font);
  running = 0;
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void loop() {
  lastFrame = SDL_GetTicks();
  if (lastFrame >= (lastTime + 1000)) {
    lastTime = lastFrame;
    fps = frameCount;
    frameCount = 0;
  }
  input();
}

void drawpoint(int x, int y) { SDL_RenderDrawPoint(renderer, x, y); }
void drawpoint(int x, int y, int b) {
  int c = 255 * b;
  SDL_SetRenderDrawColor(renderer, c, c, c, 255);
  SDL_RenderDrawPoint(renderer, x, y);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

SDL_Color fcolor;
void write(std::string text, int x, int y) {
 SDL_Surface *surface;
 SDL_Texture *texture;
 SDL_Rect word;
 const char* t = text.c_str();
 surface = TTF_RenderText_Solid(font, t, fcolor);
 texture = SDL_CreateTextureFromSurface(renderer, surface);
 word.w=surface->w;
 word.h=surface->h;
 word.x=x;
 word.y=y;
 SDL_FreeSurface(surface);
 SDL_RenderCopy(renderer, texture, NULL, &word);
 SDL_DestroyTexture(texture);
}


} // namespace sdltemplate
