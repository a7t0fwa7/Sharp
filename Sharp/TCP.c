#include "TCP.h"

SOCKET get_socket(INT af, INT type, INT proto, CHAR* ip, USHORT port) {
	WSADATA wsa = { 0 };
	SOCKET listen_socket = INVALID_SOCKET;
	SOCKET connected_socket = INVALID_SOCKET;
	DWORD ip_decimal = 0;
	struct sockaddr_in server = { 0 };
	struct sockaddr_in client = { 0 };
	INT i_ret = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		printf("[!] WSAStartup(...) failed. Error Code: %d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	listen_socket = socket(af, type, proto);
	//listen_socket = WSASocket(af, type, proto, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listen_socket == INVALID_SOCKET) {
		printf("[!] socket(...) failed. Error Code: %d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	ip_decimal = inet_addr(ip);
	if (ip_decimal == INADDR_NONE) {
		printf("[!] inet_addr(...) failed. Error Code: %d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	//Prepare the sockaddr_in structure
	server.sin_family = af;
	server.sin_addr.s_addr = ip_decimal;
	server.sin_port = htons(port);

	i_ret = bind(listen_socket, (struct sockaddr*)&server, sizeof(server));
	if (i_ret == SOCKET_ERROR) {
		printf("[!] bind(...) failed. Error Code: %d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	i_ret = listen(listen_socket, 2);
	if (i_ret == SOCKET_ERROR) {
		printf("[!] listen(...) failed. Error Code: %d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	// TODO: We can set this to check the IP that connects to us
	connected_socket = accept(listen_socket, (struct sockaddr*)&client, NULL);
	if (connected_socket == INVALID_SOCKET) {
		printf("[!] accept(...) failed. Error Code: %d\n", WSAGetLastError());
		return INVALID_SOCKET;
	}

	return connected_socket;
}

DWORD recv_all(SOCKET sock, BYTE** buffer_ptr) {
	INT length = 0;
	INT received = 0;
	INT remaining = 0;
	BYTE length_bytes[sizeof(INCOMING_HEADER)] = { 0 };
	BYTE* pos = NULL;
	BYTE* buffer = NULL;

	// Read the first 16 bytes to collect to header
	received = recv(sock, length_bytes, sizeof(INCOMING_HEADER), 0);
	if (received < sizeof(INCOMING_HEADER)) {
		printf("[!] recv(...) failed. Error code: %d\n", GetLastError());
		return -1;
	}

	length = *(INT*)length_bytes;
	remaining = length - sizeof(INCOMING_HEADER);
	if (length < sizeof(INCOMING_HEADER) || remaining <= 0) {
		return -1;
	}

	buffer = (BYTE*)calloc(length, 1);
	if (buffer == NULL) {
		return -1;
	}

	memcpy(buffer, length_bytes, sizeof(INCOMING_HEADER));

	pos = buffer + sizeof(INCOMING_HEADER);

	// Loop to read the remaining data and store it in the buffer
	while (remaining > 0) {
		received = recv(sock, pos, remaining, 0);
		if (received <= 0) {
			return -1;
		}
		remaining -= received;
		pos += received;
	}

	*buffer_ptr = buffer;
	return length;
}
