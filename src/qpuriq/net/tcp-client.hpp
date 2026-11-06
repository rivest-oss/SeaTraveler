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
#include "../std.hpp"

namespace qpuriq::net {
	class TCPClient {
		public:
			typedef void (* connect_callback_t)(TCPClient &client);
			typedef void (* error_callback_t)(TCPClient &client, int error);
			typedef void (* data_callback_t)(TCPClient &client, std::vector<u8> data, size_t length);

			typedef struct tcp_client_parameters_t {
				string hostname;
				int port = -1;
				void *user = nullptr;
				int timeout_ms = 60e3;

				connect_callback_t on_connect = nullptr;
				error_callback_t on_error = nullptr;
				data_callback_t on_data = nullptr;
			} tcp_client_parameters_t;
		
		private:
			void *user = nullptr;
			unsigned long timeout_ms = 60e3;
			connect_callback_t callback_connect = nullptr;
			error_callback_t callback_error = nullptr;
			data_callback_t callback_data = nullptr;

			void *resolved_hostname = nullptr;
			void *sock = nullptr;

		public:
			TCPClient(void);
			TCPClient(tcp_client_parameters_t setup);

			void *get_user(void);
			void *internal_get_socket(void);
			void internal_set_socket(void *p);

			unsigned long get_timeout_ms(void);
			connect_callback_t get_callback_connect(void);
			error_callback_t get_callback_error(void);
			data_callback_t get_callback_data(void);

			void set_user(void *p);
			void set_timeout_ms(unsigned long t);
			void set_callback_connect(connect_callback_t f);
			void set_callback_error(error_callback_t f);
			void set_callback_data(data_callback_t f);

			void connect(string hostname, int port);
			int write(const u8 *buff, std::size_t buff_len);
	};
};
