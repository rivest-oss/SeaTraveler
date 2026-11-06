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
	bool ok = false;

	if(qpuriq::backend::is_sdl3_backend_available())
		ok = qpuriq::backend::make_backend_sdl3();

	if(ok == false) {
		std::fprintf(stderr, "Couldn't find an available backend, exiting...\n");
		return 1;
	}

	qpuriq::net::TCPClient tcp_client(qpuriq::net::TCPClient::tcp_client_parameters_t {
		.hostname = "http.badssl.com",
		.port = 80,
		.user = nullptr,
		.timeout_ms = 60000,

		.on_connect = [](qpuriq::net::TCPClient &client) -> void {
			std::printf("TCPCLIENT CONNECTED!\n");

			const char *s = ""
				"GET / HTTP/1.1\r\n"
				"Host: http.badssl.com\r\n"
				"User-Agent: libhttp/1.0\r\n"
				"Accept: */*\r\n"
				"\r\n"
				"";

			std::size_t buff_len = 80;

			const qpuriq::u8 *buff = (const qpuriq::u8 *)s;

			std::printf("TCPCLIENT WRITE: %i\n", client.write(buff, buff_len));
		},
		.on_error = [](qpuriq::net::TCPClient &client, int err) -> void {
			std::printf("TCPCLIENT ERROR! %i\n", err);
		},
		.on_data = [](qpuriq::net::TCPClient &client, std::vector<qpuriq::u8> buff, std::size_t buff_len) -> void {
			if(buff_len > 0) {
				buff.push_back('\0');
				std::printf("TCPCLIENT DATA (%i): %s\n", (int)buff_len, (char *)buff.data());
			}
		},
	});

	while(true) {
		int rc = qpuriq::backend::get_current_backend()->update_events();
		if(rc == 0)
			continue;

		qpuriq::backend::get_current_backend()->quit();
		std::exit(rc < 0 ? 1 : 0);
	};
};
