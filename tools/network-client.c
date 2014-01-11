#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_net.h>

int main(int argc, char **argv)
{
	char *ip = "188.230.173.194";
	int port = 1107;

	UDPsocket socket;
	IPaddress address;
	int quit = 0;

	SDL_Init(SDL_INIT_EVENTS);
	SDLNet_Init();

	SDL_CreateWindow("Zeno Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 200, 100, 0);

	if(!(socket = SDLNet_UDP_Open(0)))
	{
		fprintf(stderr, "SDLNet_UDP_Open: %s\n", SDLNet_GetError());
		exit(1);
	}

	if(SDLNet_ResolveHost(&address, ip, port) == -1)
	{
		fprintf(stderr, "SDLNet_ResolveHost(%s %d): %s\n", ip, port, SDLNet_GetError());
		exit(1);
	}

	SDLNet_UDP_Bind(socket, 0, &address);

	UDPpacket *p = SDLNet_AllocPacket(512);
	SDL_Event ev;

	while(!quit)
	{
		while(SDL_PollEvent(&ev) != 0)
		{
			if(ev.type == SDL_KEYDOWN)
			{
				switch(ev.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						quit = 1;
					break;

					case SDLK_f:
						sprintf((char *)p->data, "init");
					break;

					case SDLK_g:
						sprintf((char *)p->data, "600 200");
					break;

					default:
						sprintf((char *)p->data, "%d", ev.key.keysym.sym);
					break;
				}

				p->len = strlen((char *)p->data) + 1;
				SDLNet_UDP_Send(socket, 0, p);
			}
			else if(ev.type == SDL_QUIT)
			{
				quit = 1;
			}
		}
	}

	SDLNet_FreePacket(p);
	SDLNet_Quit();

	return 0;
}
