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
#include "tcp-client.hpp"
#include "../backend.hpp"
#include "../event.hpp"

namespace qpuriq::net {
	void tcp_client_update_cb(backend::Event &event) {
		void *sock = event.get_user_p1();
		TCPClient *tcp_client = (TCPClient *)event.get_user_p0();

		std::printf("SOCKET D: %p\n", sock);

		TCPClient::data_callback_t data_cb = tcp_client->get_callback_data();
		TCPClient::error_callback_t err_cb = tcp_client->get_callback_error();
		
		u8 buf[256];
		const int buflen = 256;
		int read_rc = backend::get_current_backend()->net_tcp_client_read(sock, buf, buflen);

		std::printf("[read RC] %i\n", read_rc);

		if(read_rc == 0 && backend::get_current_backend()->net_tcp_client_get_connection_status(sock) == backend::Backend::BackendNetStatusFailed) {
			event.invalidate();
			backend::get_current_backend()->net_tcp_client_free(event.get_user_p1());

			std::printf("TCPCLIENT CIAO\n");

			return;
		}

		if(read_rc < 0) {
			event.invalidate();
			backend::get_current_backend()->net_tcp_client_free(event.get_user_p1());
			if(err_cb)
				err_cb(*tcp_client, __LINE__);

			return;
		}

		std::printf("[conn RC] %i\n", backend::get_current_backend()->net_tcp_client_get_connection_status(sock));

		std::vector<u8> buff_v;
		for(int i = 0; i < read_rc; i++)
			buff_v.push_back(buf[i]);
		
		if(data_cb)
			data_cb(*tcp_client, buff_v, read_rc);
	};

	void tcp_client_connect_cb(backend::Event &event) {
		TCPClient *tcp_client = (TCPClient *)event.get_user_p3();
		TCPClient::error_callback_t err_cb = tcp_client->get_callback_error();

		// first part: address resolution
		if(event.get_user_p1() == nullptr) {
			int rc = backend::get_current_backend()->net_resolve_hostname_get_status(event.get_user_p0());

			if(rc == backend::Backend::BackendNetStatusWaiting)
				return;

			if(rc == backend::Backend::BackendNetStatusSuccess) {
				event.set_user_i0(1);

				void *sock = backend::get_current_backend()->net_tcp_client_create_and_connect(event.get_user_p0(), event.get_user_i1());
				if(sock == nullptr) {
					event.invalidate();
					backend::get_current_backend()->net_resolve_hostname_free(event.get_user_p0());
					if(err_cb)
						err_cb(*tcp_client, __LINE__);
				}

				event.set_user_p1(sock);
				std::printf("SOCK A¹: %p\n", sock);
				std::printf("SOCK A²: %p\n", event.get_user_p1());
				return;
			}

			// else fail
			event.invalidate();
			backend::get_current_backend()->net_resolve_hostname_free(event.get_user_p0());

			if(err_cb)
				err_cb(*tcp_client, __LINE__);

			return;
		}

		// second part: connection
		int rc = backend::get_current_backend()->net_tcp_client_get_connection_status(event.get_user_p1());

		if(rc == backend::Backend::BackendNetStatusWaiting)
			return;

		if(rc == backend::Backend::BackendNetStatusSuccess) {
			void *sock = event.get_user_p1();

			backend::get_current_backend()->net_resolve_hostname_free(event.get_user_p0());
			event.invalidate();
			TCPClient::connect_callback_t connected_cb = tcp_client->get_callback_connect();

			if(connected_cb)
				connected_cb(*tcp_client);

			backend::Event e;
			e.set_period_in_ms(1);
			e.set_user_p0((void *)tcp_client);
			e.set_user_p1(sock);
			e.set_callback(tcp_client_update_cb);
			std::printf("SOCKET C: %p\n", sock);

			backend::get_current_backend()->register_event(e);
			
			return;
		}

		// else fail
		event.invalidate();
		backend::get_current_backend()->net_resolve_hostname_free(event.get_user_p0());
		backend::get_current_backend()->net_tcp_client_free(event.get_user_p1());

		if(err_cb)
			err_cb(*tcp_client, 456);
		
		return;
	};

	TCPClient::TCPClient(void) {};

	TCPClient::TCPClient(tcp_client_parameters_t setup) {
		set_user(setup.user);
		set_timeout_ms(setup.timeout_ms);
		set_callback_connect(setup.on_connect);
		set_callback_error(setup.on_error);
		set_callback_data(setup.on_data);

		connect(setup.hostname, setup.port);
	};

	void TCPClient::set_user(void *p) {
		user = p;
	};

	void TCPClient::set_timeout_ms(unsigned long t) {
		timeout_ms = t;
	};

	void TCPClient::set_callback_connect(TCPClient::connect_callback_t f) {
		callback_connect = f;
	};

	void TCPClient::set_callback_error(TCPClient::error_callback_t f) {
		callback_error = f;
	};

	void TCPClient::set_callback_data(TCPClient::data_callback_t f) {
		callback_data = f;
	};

	void TCPClient::connect(string hostname, int port) {
		backend::Event event;
		event.set_period_in_ms(5);
		event.set_user_i1(port);
		event.set_user_p0(backend::get_current_backend()->net_resolve_hostname_alloc_and_resolve(hostname));
		event.set_user_p1(nullptr);
		event.set_user_p3((void *)this);
		event.set_callback(tcp_client_connect_cb);

		backend::get_current_backend()->register_event(event);
	};

	void *TCPClient::get_user(void) {
		return user;
	};

	unsigned long TCPClient::get_timeout_ms(void) {
		return timeout_ms;
	};

	TCPClient::connect_callback_t TCPClient::get_callback_connect(void) {
		return callback_connect;
	};
	
	TCPClient::error_callback_t TCPClient::get_callback_error(void) {
		return callback_error;
	};

	TCPClient::data_callback_t TCPClient::get_callback_data(void) {
		return callback_data;
	};
};
