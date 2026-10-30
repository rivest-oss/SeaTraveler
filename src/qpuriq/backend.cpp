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

#include "backend.hpp"

namespace qpuriq::backend {
	std::shared_ptr<Backend> curr_backend;

	std::shared_ptr<Backend> get_current_backend(void) {
		return curr_backend;
	};

	void set_current_backend(std::shared_ptr<Backend> backend) {
		curr_backend = backend;
	};
};
