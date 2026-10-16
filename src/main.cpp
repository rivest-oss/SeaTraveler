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
#include "qpuriq/backend.hpp"

int main(int argc, const char **argv) {
	std::unique_ptr<qpuriq::backend::Backend> backend;
	
	if(backend == nullptr && qpuriq::backend::is_sdl3_backend_available())
		backend = qpuriq::backend::make_backend_sdl3();
	
	if(backend == nullptr) {
		std::fprintf(stderr, "Couldn't find an available backend, exiting...\n");
		return 1;
	}
	
	return 0;
};
