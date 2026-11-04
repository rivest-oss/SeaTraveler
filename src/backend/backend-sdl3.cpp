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

#include <SDL3/SDL.h>
#include <SDL3_net/SDL_net.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdio>
#include "../qpuriq/backend.hpp"

namespace qpuriq::backend {
	class BackendWindowSDL : public Window {
		private:
			SDL_Window *window = nullptr;
		
		public:
			BackendWindowSDL(int width, int height, std::string &title) {
				const char *c_title = title.c_str();
				
				window = SDL_CreateWindow(
					c_title,
					width,
					height,
					SDL_WINDOW_HIDDEN
				);
			};
			
			~BackendWindowSDL(void) {
				if(window != nullptr)
					SDL_DestroyWindow(window);
			};
			
			void make_current(void) {};
			
			void show(void) {
				if(window != nullptr)
					SDL_ShowWindow(window);
			};
			
			void hide(void) {
				if(window != nullptr)
					SDL_HideWindow(window);
			};
			
			void close(void) {
				hide();
				
				if(window != nullptr)
					SDL_DestroyWindow(window);
			};
	};
	
	class BackendSDL : public Backend {
		private:
			bool valid = false;
			SDL_Time time;

			// used for "generating" unique events IDs.
			unsigned long event_id = 0l;
			std::unordered_map<unsigned long, Event> events;
		
		public:
			BackendSDL(void) {
				event_id = 0l;
				events.clear();
				valid = true;

				if(valid && (valid == SDL_SetAppMetadata("SeaTraveler", "0.1.0a1", "org.chimoteam.seatraveler")) == false) {
					std::fprintf(stderr, "Couldn't set app metadata.\n");
					return;
				}

				if(valid && (valid == SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, "Rivest Osz")) == false) {
					std::fprintf(stderr, "Couldn't set app metadata.\n");
					return;
				}

				if(valid && (valid == SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, "Copyright (C) 2026 Rivest Osz")) == false) {
					std::fprintf(stderr, "Couldn't set app metadata.\n");
					return;
				}

				if(valid && (valid == SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, "https://chimoteam.eu.org/")) == false) {
					std::fprintf(stderr, "Couldn't set app metadata.\n");
					return;
				}

				if(valid && (valid == SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, "application")) == false) {
					std::fprintf(stderr, "Couldn't set app metadata.\n");
					return;
				}

				if(valid && (valid = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS)) == false) {
					std::fprintf(stderr, "Couldn't initialize SDL.\n");
					return;
				}

				if(valid && (valid = NET_Init()) == false) {
					std::fprintf(stderr, "Couldn't initialize SDL_net.\n");
					return;
				}

				if(valid && (valid = TTF_Init()) == false) {
					std::fprintf(stderr, "Couldn't initialize SDL_ttf.\n");
					return;
				}

				if(valid && (valid = SDL_GetCurrentTime(&time)) == false) {
					std::fprintf(stderr, "Couldn't get current time.\n");
					return;
				}
			};
			
			~BackendSDL(void) {
				quit();
			};

			void *net_resolve_hostname_alloc_and_resolve(string hostname) {
				const char *nstr_hostname = hostname.c_str();
				NET_Address *addr = NET_ResolveHostname(nstr_hostname);
				return (void *)addr;
			};

			int net_resolve_hostname_get_status(void *p) {
				NET_Address *addr = (NET_Address *)p;
				if(addr == nullptr)
					return BackendNetStatusFailed;

				NET_Status status = NET_GetAddressStatus(addr);
				switch(status) {
					case NET_WAITING:
						return BackendNetStatusWaiting;
					case NET_SUCCESS:
						return BackendNetStatusSuccess;
					case NET_FAILURE:
					default:
						return BackendNetStatusFailed;
				};
			};

			void net_resolve_hostname_free(void *p) {
				NET_Address *addr = (NET_Address *)p;
				if(addr == nullptr)
					return;

				NET_UnrefAddress(addr);
			};

			void *net_tcp_client_create_and_connect(void *addr_voidptr, int port) {
				NET_Address *addr = (NET_Address *)addr_voidptr;
				if(addr == nullptr)
					return nullptr;
				if(port < 0 || port > 0xffff)
					return nullptr;

				NET_StreamSocket *socket = NET_CreateClient(addr, port, 0);
				return (void *)socket;
			};

			int net_tcp_client_get_connection_status(void *p) {
				NET_StreamSocket *socket = (NET_StreamSocket *)p;
				if(socket == nullptr)
					return BackendNetStatusFailed;
				
				NET_Status status = NET_GetConnectionStatus(socket);
				switch(status) {
					case NET_WAITING:
						return BackendNetStatusWaiting;
					case NET_SUCCESS:
						return BackendNetStatusSuccess;
					case NET_FAILURE:
					default:
						return BackendNetStatusFailed;
				};
			};

			void net_tcp_client_free(void *p) {
				(void)p;
			};
			
			std::shared_ptr<Window> create_window(int width, int height, std::string &title) {
				return std::make_shared<BackendWindowSDL>(width, height, title);
			};

			void register_event(Event &event) {
				while(events.find(event_id) != events.cend())
					event_id++;

				event.set_unique_id(event_id);
				events.insert_or_assign(event_id, event);
			};

			void unregister_event(Event event) {
				unsigned long id = event.get_unique_id();

				if(events.find(id) == events.cend())
					return;
				
				events.erase(id);
			};

			bool update_events(void) {
				SDL_Time old_time = time;
				(void)SDL_GetCurrentTime(&time);

				double dt = (time - old_time) / 1e9;

				for(std::pair<unsigned long, Event> pair : events) {
					Event &event = events.at(pair.first);
					
					if(event.is_valid() == false) {
						unregister_event(event);
						continue;
					}

					if(event.get_period_in_ms() < 0l) {
						event.execute();
						unregister_event(event);
						continue;
					}

					if(event.get_period_in_ms() == 0l) {
						event.execute();
						continue;
					}

					double t = event.get_delta_time() + dt;

					if((long)(t * 1000.0) >= event.get_period_in_ms()) {
						t -= (double)event.get_period_in_ms() / 1000.0;
						event.execute();
					}

					event.set_delta_time(t);
				};

				return true;
			};
			
			void quit(void) {
				TTF_Quit();
				NET_Quit();
				SDL_Quit();
			};
	};
	
	bool is_sdl3_backend_available(void) {
		#if defined(QPURIQ_ENABLE_BACKEND_SDL3)
			return true;
		#else
			return false;
		#endif
	};
	
	std::shared_ptr<Backend> make_backend_sdl3(void) {
		return std::make_unique<BackendSDL>();
	};
};
