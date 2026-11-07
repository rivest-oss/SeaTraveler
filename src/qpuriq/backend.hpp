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
#include "std.hpp"
#include "event.hpp"
#include "../Ox/include/nuclei.hpp"

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
			virtual bool is_valid(void) = 0;
			
			virtual std::shared_ptr<Window> create_window(int width, int height, string &title) = 0;

			enum : int {
				BackendNetStatusFailed = -1,
				BackendNetStatusWaiting = 0,
				BackendNetStatusSuccess = 1,
			};

			virtual void *net_resolve_hostname_alloc_and_resolve(string hostname) = 0;
			virtual int net_resolve_hostname_get_status(void *p) = 0;
			virtual void net_resolve_hostname_free(void *p) = 0;

			virtual void *net_tcp_client_create_and_connect(void *addr, int port) = 0;
			virtual int net_tcp_client_get_connection_status(void *p) = 0;
			virtual void net_tcp_client_free(void *p) = 0;

			virtual int net_tcp_client_read(void *sock, void *buf, int buflen) = 0;
			virtual int net_tcp_client_write(void *sock, const void *buf, int buflen) = 0;

			virtual void register_event(Event &event) = 0;
			virtual void unregister_event(Event event) = 0;
			
			virtual int update_events(void) = 0;

			virtual void flag_quit(bool fatal) = 0;
			virtual void quit(void) = 0;
	};
	
	bool is_sdl3_backend_available(void);

	// fails with (false).
	bool make_backend_sdl3(void);

	std::shared_ptr<Backend> get_current_backend(void);
	void set_current_backend(std::shared_ptr<Backend> backend);
};
