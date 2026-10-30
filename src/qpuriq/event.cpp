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

#include "event.hpp"

namespace qpuriq::backend {
	Event::Event(void) {
		valid = 0xa5e827b1;
	};

	Event::~Event(void) {
		destroy();
	};

	bool Event::is_valid(void) {
		return valid == 0xa5e827b1;
	};

	void Event::invalidate(void) {
		valid = 123;
	};

	void Event::destroy(void) {
		if(valid != 0xa5e827b1)
			return;
	};

	long Event::get_period_in_ms(void) {
		return period_ms;
	};

	void Event::set_period_in_ms(long ms) {
		period_ms = ms;
	};

	void Event::set_callback(Event::callback_t f) {
		callback = f;
	};

	void Event::execute(void) {
		callback(*this);
	};

	unsigned long Event::get_unique_id(void) {
		return unique_id;
	};

	void Event::set_unique_id(unsigned long id) {
		unique_id = id;
	};

	double Event::get_delta_time(void) {
		return delta_time;
	};

	void Event::set_delta_time(double dt) {
		delta_time = dt;
	};

	void *Event::get_user_p0(void) {
		return user_p0;
	};

	void *Event::get_user_p1(void) {
		return user_p1;
	};

	void Event::set_user_p0(void *v) {
		user_p0 = v;
	};

	void Event::set_user_p1(void *v) {
		user_p1 = v;
	};
};
