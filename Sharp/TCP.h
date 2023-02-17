#pragma once
#include "sharp.h"

DWORD recv_all(SOCKET sock, BYTE** buffer_ptr);
SOCKET get_socket(INT af, INT type, INT proto, CHAR* ip, USHORT port);
