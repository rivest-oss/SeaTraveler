/* SeaTraveler: An internet suite.
** Copyright (C) 2026  Rivest Osz
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <https://www.gnu.org/licenses/>.
**/

#include "Ox/include/nuclei.hpp"
#include <SDL3/SDL.h>

#if defined(PLATFORM_WEB)
	#include <emscripten/emscripten.h>
#endif

const int screen_width = 1280;
const int screen_height = 960;

namespace qpuriq {};

/*
static SDL_Window *win = nullptr;
static SDL_Renderer *renderer = nullptr;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
	if(SDL_Init(SDL_INIT_VIDEO) == false) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL", SDL_GetError(), NULL);
		return SDL_APP_FAILURE;
	}
	
	if(SDL_CreateWindowAndRenderer("SeaTraveler", screen_width, screen_height, 0, &win, &renderer) == false) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer", SDL_GetError(), NULL);
		return SDL_APP_FAILURE;
	}
	
	return SDL_APP_CONTINUE;
};

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *e) {
	switch(e->type) {
		case SDL_EVENT_QUIT:
			return SDL_APP_SUCCESS;
		
		case SDL_EVENT_KEY_DOWN: {
			if(e->key.key == SDLK_ESCAPE) {
				return SDL_APP_SUCCESS;
			}
		};
		
		default:
			break;
	};
	
	return SDL_APP_CONTINUE;
};

SDL_AppResult SDL_AppIterate(void *appstate) {
	SDL_SetRenderDrawColor(renderer, 216, 181, 137, 255);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	
	return SDL_APP_CONTINUE;
};

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
	//
};
*/

int main(int argc, const char **argv) {
	return 0;
};
