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

#include <cstdio>
#include "../qpuriq/backend.hpp"

namespace qpuriq::backend {
	class BackendWindowSDL : public Window {
		public:
			~BackendWindowSDL(void) {};
	};
	
	class BackendSDL : public Backend {
		public:
			~BackendSDL(void) {};
	};
	
	bool is_sdl3_backend_available(void) {
		#if defined(QPURIQ_ENABLE_BACKEND_SDL3)
			return true;
		#else
			return false;
		#endif
	};
	
	std::unique_ptr<Backend> make_backend_sdl3(void) {
		std::printf("[DEBUG] Using SDL3 backend\n");
		return std::make_unique<BackendSDL>();
	};
};
