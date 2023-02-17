#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library


#if defined (_TCP)
#include "TCP.h"
#define NET_TCP
#elif defined (_TCPS)
#define NET_TCPS
#elif defined (_HTTP)
#define NET_HTTP
#elif defined (_HTTPS)
#define NET_HTTPS
#elif defined (_UDP)
#define NET_UDP
#else
#include "TCP.h"
#define NET_TCP
#endif

#if !defined (_DEBUG)
#define printf ;
#endif

typedef struct _REPLY_HEADER {
	DWORD data_length;
	SHORT command_id;
	SHORT task_id;
	SHORT error_code;
	BYTE uuid[4];

} REPLY_HEADER, * PREPLY_HEADER;

typedef struct _INCOMING_HEADER {
	DWORD data_length;
	SHORT command_id;
	SHORT task_id;
	DWORD checksum;
	DWORD uuid;
	BYTE data[];
} INCOMING_HEADER, * PINCOMING_HEADER;

