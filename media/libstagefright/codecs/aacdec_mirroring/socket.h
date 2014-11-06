/****************************************************************
* �ļ�����: socket.h                       
* ��    ��: socket��������                         
* ��    ��: ���            2011/11/16
 ***************************************************************/

#ifndef __SOCKET_H__
#define __SOCKET_H__

namespace android {

	class Socket  
	{
	public:
		Socket();
		virtual ~Socket();
		int Listen(int iPort);
		int Accept(int iSrvFd);
		int Close(int iSockFd);
		int Connect(const char *strIp, const int iPort);
		int RecvN(int iSockFd, unsigned char *strBuf, int iLen, unsigned int uiTimeOut);
		int SendN(int iSockFd, unsigned char *strBuf, int iLen, unsigned int uiTimeOut);
		int Recv(int iSockFd, unsigned char *strBuf, int iLen, unsigned int uiTimeOut);
	};
	
}

#endif