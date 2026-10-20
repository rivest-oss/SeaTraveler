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

#pragma once
#include <memory>
#include <string>

namespace qpuriq::backend {
	class Window {
		public:
			virtual ~Window(void) = default;
			
			virtual void make_current(void) = 0;
			virtual void show(void) = 0;
			virtual void hide(void) = 0;
			virtual void close(void) = 0;
	};
	
	class Backend {
		public:
			virtual ~Backend(void) = default;
			
			virtual std::unique_ptr<Window> create_window(int width, int height, std::string &title) = 0;
			virtual void quit(void) = 0;
	};
	
	bool is_sdl3_backend_available(void);
	std::unique_ptr<Backend> make_backend_sdl3(void);
};
