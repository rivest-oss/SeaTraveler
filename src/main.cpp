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
#include "qpuriq/net/tcp-client.hpp"

int main(int argc, const char **argv) {
	std::shared_ptr<qpuriq::backend::Backend> backend;
	
	if(backend == nullptr && qpuriq::backend::is_sdl3_backend_available())
		backend = qpuriq::backend::make_backend_sdl3();
	
	if(backend == nullptr) {
		std::fprintf(stderr, "Couldn't find an available backend, exiting...\n");
		return 1;
	}

	qpuriq::backend::set_current_backend(backend);

	qpuriq::net::TCPClient tcp_client(qpuriq::net::TCPClient::tcp_client_parameters_t {
		.hostname = "1.1.1.1",
		.port = 80,
		.user = nullptr,
		.timeout_ms = 60000,

		.on_connect = [](qpuriq::net::TCPClient &client) -> void {
			std::printf("TCPCLIENT CONNECTED!\n");
		},
		.on_error = [](qpuriq::net::TCPClient &client, int err) -> void {
			std::printf("TCPCLIENT ERROR! %i\n", err);
		},
		.on_data = nullptr,
	});

	while(backend->update_events()) {};
	
	backend->quit();
	
	return 0;
};
