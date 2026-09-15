#include "socket.h"
#pragma comment(lib, "ws2_32.lib")

int main() 
{
	Server server = Server(htons(65000));
	while (true)
		server.Read_fds();
}