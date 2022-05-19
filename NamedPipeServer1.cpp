#include "stdafx.h"
#include "NamedPipeServer1.h"
#include "common.h"

#include <strsafe.h>

typedef long (CALLBACK *pfnCallBack4Out)(const unsigned char *const pData, const long dsize);

CNamedPipeServer::CNamedPipeServer()
{
	m_pfnCallback = NULL;

	sprintf(m_szServerName, "Nonamed-Server");

	hPipe = INVALID_HANDLE_VALUE;

	bStop = false;
}

CNamedPipeServer::~CNamedPipeServer()
{
	Stop();
}

DWORD WINAPI ServerThread1(LPVOID lpvParam)
{
	PrintLogA("CNamedPipeServer1::Start Thread");

	char pchRequest[BUFSIZE];

	DWORD cbBytesRead = 0; 
	BOOL fSuccess = FALSE;
	
	CNamedPipeServer *This = (CNamedPipeServer*)lpvParam;
	
	// The thread's parameter is a handle to a pipe object instance. 
	
	BOOL   fConnected = FALSE; 
	This->hPipe = INVALID_HANDLE_VALUE; 

	char lpszPipename[256];
	sprintf(lpszPipename, "\\\\.\\pipe\\%s", This->m_szServerName);

	PrintLogA("Pipe Server : %s", This->m_szServerName);

	// The main loop creates an instance of the named pipe and 
	// then waits for a client to connect to it. When the client 
	// connects, a thread is created to handle communications 
	// with that client, and this loop is free to wait for the
	// next client connect request. It is an infinite loop.

	while (!This->bStop)
	{ 
		PrintLogA("\nPipe Server: Main thread awaiting client connection on %s\n", lpszPipename);
		This->hPipe = CreateNamedPipe( 
			lpszPipename,             // pipe name 
			PIPE_ACCESS_DUPLEX,       // read/write access 
			PIPE_TYPE_BYTE |       // message type pipe 
			PIPE_READMODE_BYTE |   // message-read mode 
			PIPE_WAIT,                // blocking mode 
			1, // max. instances  
			BUFSIZE,                  // output buffer size 
			BUFSIZE,                  // input buffer size 
			0,                        // client time-out 
			NULL);                    // default security attribute 

		if (This->hPipe == INVALID_HANDLE_VALUE) 
		{
			PrintLogA("CreateNamedPipe failed, GLE=%d.\n", GetLastError()); 
			return -1;
		}

		// Wait for the client to connect; if it succeeds, 
		// the function returns a nonzero value. If the function
		// returns zero, GetLastError returns ERROR_PIPE_CONNECTED. 

		fConnected = ConnectNamedPipe(This->hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED); 

		if (fConnected) 
		{ 
			PrintLogA("Client connected, creating a processing thread.\n"); 

			break;
		} 
		else 
			// The client could not connect, so close the pipe. 
			CloseHandle(This->hPipe); 
	} 

	// Loop until done reading
	while (!This->bStop) 
	{ 
		// Read client requests from the pipe. This simplistic code only allows messages
		// up to BUFSIZE characters in length.
		fSuccess = ReadFile( 
			This->hPipe,        // handle to pipe 
			pchRequest,    // buffer to receive data 
			BUFSIZE, // size of buffer 
			&cbBytesRead, // number of bytes read 
			NULL);        // not overlapped I/O 

		if (!fSuccess || cbBytesRead == 0)
		{   
			if (GetLastError() == ERROR_BROKEN_PIPE)
			{
				PrintLogA("InstanceThread: client disconnected.\n", GetLastError()); 
			}
			else
			{
				PrintLogA("InstanceThread ReadFile failed, GLE=%d.\n", GetLastError()); 
			}
			break;
		}

		PrintLogA("ServerThread Received - ", cbBytesRead);
		// Process the incoming message.
		if (This->m_pfnCallback)
		{
			pfnCallBack4Out pf = (pfnCallBack4Out)This->m_pfnCallback;
			pf((unsigned char*)pchRequest, cbBytesRead);
		}		
	}

	// Flush the pipe to allow the client to read the pipe's contents 
	// before disconnecting. Then disconnect the pipe, and close the 
	// handle to this pipe instance. 

	FlushFileBuffers(This->hPipe); 
	DisconnectNamedPipe(This->hPipe); 
	CloseHandle(This->hPipe); 
		
	PrintLogA("CNamedPipeServer1::End Thread");

	return 0;
}

int CNamedPipeServer::Start(char *_szServerName, void *_pfnCallback) 
{ 
	sprintf(m_szServerName, _szServerName);
	m_pfnCallback = _pfnCallback;
	bStop = false;
	CreateThread(NULL, NULL, ServerThread1, this, NULL, NULL);

	return 0; 
} 

void CNamedPipeServer::Stop()
{
	Sleep(5);
	bStop = true;
	Sleep(5);
}
