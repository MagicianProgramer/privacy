#ifndef _NamedPipeServer1_H_
#define _NamedPipeServer1_H_

#include <windows.h>

#define PIPE_TIMEOUT 1800000
#define BUFSIZE 4096

class CNamedPipeServer 
{
public:
	CNamedPipeServer();
	~CNamedPipeServer();
	
	int Start(char *_wszServerName, void *_pfnCallback);
	void Stop();
	void *m_pfnCallback;
	char m_szServerName[256]; 
	bool bStop;
	HANDLE hPipe;
};


////////////////////////////////////////////////////////////////////////////////
// Inline Functions															  //
////////////////////////////////////////////////////////////////////////////////


#endif //#ifndef _NamedPipeServer1_H_