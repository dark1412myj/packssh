#pragma once

#pragma comment(lib, "ws2_32.lib")

#include "libssh2_config.h"
#include <libssh2.h>
#include <libssh2_sftp.h>
//#include <winsock2.h>
#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>
#include<string>
#include<iostream>
using namespace std;

class Sftp
{
	LIBSSH2_SFTP* m_sftp_session;
	LIBSSH2_SFTP_HANDLE* m_sftp_handle;
public:
	Sftp(LIBSSH2_SFTP* sftp_session) : m_sftp_session(sftp_session), m_sftp_handle(NULL){}
	bool Open(string sftppath,long mode = LIBSSH2_FXF_READ | LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT );

	bool Close();

	size_t Write(char* data,size_t len);

	size_t Read(char* data, size_t len);

	~Sftp()
	{
		if (m_sftp_handle)
		{
			libssh2_sftp_close(m_sftp_handle);
		}
		if (m_sftp_session)
		{
			libssh2_sftp_shutdown(m_sftp_session);
		}
	}
};

