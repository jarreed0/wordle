#include "sdltemplate.h"
#include <ctype.h> //isalpha
#include <vector>
#include <fstream>
#include <vector>

using namespace sdltemplate;

std::string alphabet;


const int rows = 6;
const int cols = 5;
const int WIDTH = 435;
const int HEIGHT = 520;
std::string grid[rows*cols];
int check[rows*cols];

int loc = 0;

bool won = 0;

//std::string answer[cols] = {"h","e","l","l","o"};
std::string answer[cols] = {"y","o","d","l","e"};

SDL_Color lightgrey = createColor(194, 197, 198, 255);
SDL_Color grey = createColor(119, 124, 126, 255);
SDL_Color green = createColor(96, 166, 101, 255);
SDL_Color yellow = createColor(205, 179, 93, 255);
SDL_Color white = createColor(255, 255, 255, 255);
SDL_Color black = createColor(0, 0, 0, 255);

void render() {
	for(int w=0; w<cols; w++) {
		for(int h=0; h<rows; h++) {
			setDrawColor(lightgrey);
			fcolor = black;
			drawRectOutline(w * 85 + 10, h * 85 + 10, 75, 75);
			if(w + (h*cols) == loc) drawRectFilled(w * 85 + 10, h * 85 + 10, 75, 75);
			if(check[w + (h*cols)] != 0) {
				fcolor = white;
				if(check[w + (h*cols)] == 1) setDrawColor(green);
				if(check[w + (h*cols)] == 2) setDrawColor(yellow);
				if(check[w + (h*cols)] == 3) setDrawColor(grey);
				drawRectFilled(w * 85 + 10, h * 85 + 10, 75, 75);
			}
			if(grid[w + (h*cols)] != "") {
				write(grid[w + (h*cols)], w * 85 + 42, h * 85 + 18);
			}
		}
	}
}

void update() {
}

char lastKey;
bool enterDown;
bool bkspDown;

std::vector<std::string> dict;

bool isAWord(std::string w) {
	for(int i = 0; i < dict.size(); i++) {
		if(dict[i] == w) return 1;
	}
	return 0;
}

void type() {
	if(isalpha(*event.text.text) && *event.text.text != lastKey && loc < rows*cols) {
		grid[loc] = event.text.text;
		if(loc == 0 || (loc+2) % cols != 1) loc++;
	}
	lastKey = *event.text.text;
	if(keystates[SDL_SCANCODE_RETURN] && !enterDown) {
		//std::cout << "ENTER" << std::endl;
		enterDown = 1;
		if(loc != 0 && (loc+2) % cols == 1 && grid[loc] != "") {
			std::string word;
			for(int i=loc-4; i<loc+1; i++) word.append(grid[i]);
			if(isAWord(word)) {
				int correct = 0;
				for(int i=loc-4; i<loc+1; i++) {
					if(grid[i] == answer[i % cols]) {
						check[i] = 1;
						correct++;
					} else {
						check[i] = 3;
						for(int a=0; a<cols; a++) {
							if(answer[a] == grid[i]) check[i] = 2;
						}
					}
				}
				if(correct == cols) won=1;
				if(loc != rows*cols - 1) loc++;
			}
		}
	} else if(!keystates[SDL_SCANCODE_RETURN]) {
		enterDown = 0;
	}
	if(keystates[SDL_SCANCODE_BACKSPACE] && !bkspDown && loc > 0) {
		//std::cout << "DELETE" << std::endl;
		bkspDown = 1;
		if(check[loc-1] == 0) {
			if(grid[loc] != "") {
				grid[loc] = "";
			} else {
				grid[loc] = "";
			}
			loc--;
		}
	} else if (!keystates[SDL_SCANCODE_BACKSPACE]) {
		bkspDown = 0;
	}
}

int main() {
	init("Wordle", WIDTH, HEIGHT);
	alphabet = "abcdefghijklmnopqrstuvwxyz";
	bkg = createColor(255, 255, 255, 255);

	std::ifstream file("dict.txt");
	while(true) {
		std::string w;
		file >> w;
		if(file.eof()) break;
		dict.push_back(w);
	}
	file.close();

	std::srand(time(NULL));
	std::string randAns = dict[std::rand() % dict.size()];
	/*for(int i=0; i<cols; i++) {
		std::string s(1,randAns[i]);
		answer[i] = s;
	}*/

	while(running) {
		loop();
		begin_render();
		render();
		end_render();
		update();
		if(won==false) type();
		if(keystates[SDL_SCANCODE_ESCAPE]) running = false;
	}
	std::cout << randAns << std::endl;
	quit();
	return 0;
}

