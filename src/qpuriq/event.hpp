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

namespace qpuriq::backend {
	class Event {
		public:
			typedef void (*callback_t)(Event &event);

		private:
			unsigned long valid = 0;
			callback_t callback = nullptr;
			void *user_p0 = nullptr;
			void *user_p1 = nullptr;
			long period_ms = -1;
			unsigned long unique_id;
			unsigned long destroyable;
			double delta_time = 0.0;

		public:
			Event(void);
			~Event(void);

			bool is_valid(void);
			void invalidate(void);
			void destroy(void);

			// (not periodic, run once: -1, periodic, run as fast as possible: 0, else +1)
			long get_period_in_ms(void);
			// (not periodic, run once: -1, periodic, run as fast as possible: 0, else +1)
			void set_period_in_ms(long ms);

			void set_callback(callback_t f);
			void execute(void);

			unsigned long get_unique_id(void);
			void set_unique_id(unsigned long id);
			double get_delta_time(void);
			void set_delta_time(double dt);

			void *get_user_p0(void);
			void *get_user_p1(void);

			void set_user_p0(void *v);
			void set_user_p1(void *v);
	};
};
