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

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdio>
#include "../qpuriq/backend.hpp"

namespace qpuriq::backend {
	class BackendWindowSDL : public Window {
		private:
			SDL_Window *window = nullptr;
		
		public:
			BackendWindowSDL(int width, int height, std::string &title) {
				const char *c_title = title.c_str();
				
				window = SDL_CreateWindow(
					c_title,
					width,
					height,
					SDL_WINDOW_HIDDEN
				);
			};
			
			~BackendWindowSDL(void) {
				if(window != nullptr)
					SDL_DestroyWindow(window);
			};
			
			void make_current(void) {};
			
			void show(void) {
				if(window != nullptr)
					SDL_ShowWindow(window);
			};
			
			void hide(void) {
				if(window != nullptr)
					SDL_HideWindow(window);
			};
			
			void close(void) {
				hide();
				
				if(window != nullptr)
					SDL_DestroyWindow(window);
			};
	};
	
	class BackendSDL : public Backend {
		private:
			bool valid = false;
		
		public:
			BackendSDL(void) {
				if((valid = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) == false) {
					std::fprintf(stderr, "Couldn't initialize SDL.\n");
					return;
				}

				if(valid && (valid = NET_Init()) == false) {
					std::fprintf(stderr, "Couldn't initialize SDL_net.\n");
					return;
				}

				if(valid && (valid = TTF_Init()) == false) {
					std::fprintf(stderr, "Couldn't initialize SDL_ttf.\n");
					return;
				}
			};
			
			~BackendSDL(void) {
				quit();
			};
			
			std::unique_ptr<Window> create_window(int width, int height, std::string &title) {
				return std::make_unique<BackendWindowSDL>(width, height, title);
			};
			
			void quit(void) {
				TTF_Quit();
				NET_Quit();
				SDL_Quit();
			};
	};
	
	bool is_sdl3_backend_available(void) {
		#if defined(QPURIQ_ENABLE_BACKEND_SDL3)
			return true;
		#else
			return false;
		#endif
	};
	
	std::unique_ptr<Backend> make_backend_sdl3(void) {
		return std::make_unique<BackendSDL>();
	};
};
