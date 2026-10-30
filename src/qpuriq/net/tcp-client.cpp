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

#include "tcp-client.hpp"

namespace qpuriq::net {
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
		// [TODO]
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
