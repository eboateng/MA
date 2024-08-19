/*
 * Copyright (C) 2012-2022 MotionSystems
 * 
 * This file is part of ForceSeatMI SDK.
 *
 * www.motionsystems.eu
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "UDPServer.h"
#include <stdio.h>
#include <string>

namespace
{
	static constexpr const unsigned int BUFFER_SIZE = 1024;
}

/*
 * Super simple UDP server implementation. It calls 'datagramCallback' when new datagram is received.
 */
void UDPServer::exec(unsigned int port, DatagramCallback datagramCallback)
{
	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("WSAStartup error: %d\n", WSAGetLastError());
		return;
	}

	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET)
	{
		printf("Socket error: %d\n" , WSAGetLastError());
		WSACleanup();
		return;
	}
	
	{
		struct sockaddr_in serverAddr;
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_addr.s_addr = INADDR_ANY;
		serverAddr.sin_port = htons(port);

		if (bind(s, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
		{
			printf("Bind error: %d\n", WSAGetLastError());
			closesocket(s);
			WSACleanup();
			return;
		}
	}

	char buffer[BUFFER_SIZE];
	memset(&buffer, 0, sizeof(buffer));
	struct sockaddr_in clientAddr;
	int slen = sizeof(clientAddr);

	printf("Server started on port: %u\n", port);
	printf("Press 'q' to exit\n");
	while (GetKeyState('Q') == 0)
	{
		auto bytes = recvfrom(s, buffer, BUFFER_SIZE, 0, nullptr, nullptr);

		if (bytes == SOCKET_ERROR)
		{
			printf("recvfrom() error: %d\n" , WSAGetLastError());
			break;
		}
		else if (bytes > 0 && datagramCallback != nullptr)
		{
			datagramCallback(buffer, bytes);
		}
	}
	printf("Server stopped\n");

	closesocket(s);
	WSACleanup();
}
