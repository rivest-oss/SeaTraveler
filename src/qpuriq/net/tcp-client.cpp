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
	typedef struct tcp_client_obj_t {
		TCPClient *client = nullptr;
		void *addr = nullptr;
		void *sock = nullptr;

		void destroy_address(void) {
			if(addr != nullptr)
				backend::get_current_backend()->net_resolve_hostname_free(addr);
			
			addr = nullptr;
		};

		void destroy_connection(void) {
			if(sock != nullptr)
				backend::get_current_backend()->net_tcp_client_free(sock);

			sock = nullptr;
		};

		void destroy_all(void) {
			destroy_address();
			destroy_connection();
			client = nullptr;
		};

		static void update_event_cb(backend::Event &event);
	} tcp_client_obj_t;

	tcp_client_obj_t *alloc_tcp_client_obj(void) {
		Ox::Error err;
		tcp_client_obj_t *client = Ox::inhale<tcp_client_obj_t>(err);

		if(client == nullptr)
			return nullptr;

		client->addr = client->sock = nullptr;

		return client;
	};

	void free_tcp_client_obj(tcp_client_obj_t *client) {
		if(client == nullptr)
			return;

		client->destroy_all();
		Ox::exhale(client);
	};

	void tcp_client_obj_t::update_event_cb(backend::Event &event) {
		tcp_client_obj_t *tcp_client_obj = (tcp_client_obj_t *)event.get_user_p0();
		if(tcp_client_obj == nullptr)
			return;

		if(tcp_client_obj->client == nullptr) {
			tcp_client_obj->destroy_all();
			event.invalidate();
			return;
		}

		TCPClient *client = tcp_client_obj->client;
		void *addr = tcp_client_obj->addr;
		void *sock = tcp_client_obj->sock;

		TCPClient::error_callback_t err_cb = client->get_callback_error();

		if(addr == nullptr && sock == nullptr) {
			string hostname = event.get_user_s0();
			addr = backend::get_current_backend()->net_resolve_hostname_alloc_and_resolve(hostname);

			if(addr == nullptr) {
				event.invalidate();

				if(err_cb)
					err_cb(*client, __LINE__);

				free_tcp_client_obj(tcp_client_obj);

				return;
			}

			tcp_client_obj->addr = addr;

			return;
		}

		if(addr != nullptr && sock == nullptr) {
			int rc = backend::get_current_backend()->net_resolve_hostname_get_status(addr);

			if(rc == backend::Backend::BackendNetStatusWaiting)
				return;

			if(rc == backend::Backend::BackendNetStatusFailed) {
				event.invalidate();

				if(err_cb)
					err_cb(*client, __LINE__);

				free_tcp_client_obj(tcp_client_obj);

				return;
			}

			if(rc == backend::Backend::BackendNetStatusSuccess) {
				int port = event.get_user_i0();
				sock = backend::get_current_backend()->net_tcp_client_create_and_connect(addr, port);

				if(sock == nullptr) {
					event.invalidate();

					if(err_cb)
						err_cb(*client, __LINE__);

					free_tcp_client_obj(tcp_client_obj);

					return;
				}

				tcp_client_obj->sock = sock;
			}

			return;
		}

		if(addr != nullptr && sock != nullptr) {
			int rc = backend::get_current_backend()->net_tcp_client_get_connection_status(sock);

			if(rc == backend::Backend::BackendNetStatusWaiting)
				return;

			if(rc == backend::Backend::BackendNetStatusFailed) {
				event.invalidate();

				if(err_cb)
					err_cb(*client, __LINE__);

				free_tcp_client_obj(tcp_client_obj);

				return;
			}

			if(rc == backend::Backend::BackendNetStatusSuccess) {
				tcp_client_obj->destroy_address();

				TCPClient::connect_callback_t conn_cb = client->get_callback_connect();
				if(conn_cb)
					conn_cb(*client);
			}

			return;
		}

		if(addr == nullptr && sock != nullptr) {
			TCPClient::data_callback_t data_cb = client->get_callback_data();

			u8 buff[256];
			int rc = backend::get_current_backend()->net_tcp_client_read(sock, (void *)buff, 256);

			if(rc < 0) {
				event.invalidate();

				if(err_cb)
					err_cb(*client, __LINE__);

				free_tcp_client_obj(tcp_client_obj);

				return;
			}

			if(backend::get_current_backend()->net_tcp_client_get_connection_status(sock) != backend::Backend::BackendNetStatusSuccess) {
				event.invalidate();

				TCPClient::close_callback_t close_cb = client->get_callback_close();
				if(close_cb)
					close_cb(*client);

				free_tcp_client_obj(tcp_client_obj);

				return;
			}

			if(data_cb)
				data_cb(*client, buff, rc);

			return;
		}
	};

	TCPClient::TCPClient(void) {
		sock = alloc_tcp_client_obj();
	};

	TCPClient::TCPClient(tcp_client_parameters_t setup) {
		sock = alloc_tcp_client_obj();
		
		set_user(setup.user);
		set_timeout_ms(setup.timeout_ms);
		set_callback_connect(setup.on_connect);
		set_callback_error(setup.on_error);
		set_callback_data(setup.on_data);
		set_callback_close(setup.on_close);

		connect(setup.hostname, setup.port);
	};

	void TCPClient::connect(string hostname, int port) {
		tcp_client_obj_t *client_obj = (tcp_client_obj_t *)sock;
		if(client_obj != nullptr) {
			client_obj->client = this;

			backend::Event event;
			event.set_period_in_ms(1);
			event.set_callback(tcp_client_obj_t::update_event_cb);

			event.set_user_i0(port);
			event.set_user_s0(hostname);
			event.set_user_p0(sock);

			backend::get_current_backend()->register_event(event);
		}
	};

	int TCPClient::write(const u8 *buff, std::size_t buff_len) {
		tcp_client_obj_t *client_obj = (tcp_client_obj_t *)sock;
		void *sock = client_obj->sock;

		if(client_obj == nullptr || sock == nullptr)
			return -1;

		return backend::get_current_backend()->net_tcp_client_write(sock, buff, buff_len);
	};

	void TCPClient::close(void) {
		tcp_client_obj_t *client_obj = (tcp_client_obj_t *)sock;
		if(client_obj != nullptr)
			client_obj->destroy_all();
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

	TCPClient::close_callback_t TCPClient::get_callback_close(void) {
		return callback_close;
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

	void TCPClient::set_callback_close(close_callback_t f) {
		callback_close = f;
	};
};
