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
using namespace std;
#include "Channel.h"
#include "Sftp.h"
class Ssh2
{
public:
	Ssh2(const string &srvIp, int srvPort = 22);
	~Ssh2(void);

	bool Connect(const string &userName, const string &userPwd);
	bool Disconnect(void);

	Channel* CreateChannel(const string &ptyType = "vanilla");
	Sftp* CreateSftp()
	{
		LIBSSH2_SFTP* sftp_session = NULL;
		sftp_session = libssh2_sftp_init(m_session);
		if (!sftp_session)
		{
			return NULL;
		}
		Sftp* ret = new Sftp(sftp_session);
		return ret;
	}
public:
	static void S_KbdCallback(const char*, int, const char*, int, int, const LIBSSH2_USERAUTH_KBDINT_PROMPT*, LIBSSH2_USERAUTH_KBDINT_RESPONSE*, void **a);
	static string s_password;

private:
	string m_srvIp;
	int    m_srvPort;
	string m_userName;
	string m_password;
	int    m_sock;
	LIBSSH2_SESSION *m_session;
};