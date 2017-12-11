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
#include<vector>
using namespace std;

const int CHANNEL_READ_TIMEOUT = 3000;

class Channel
{
public:
	Channel(LIBSSH2_CHANNEL *channel);
	~Channel(void);
	std::vector<char> Read(const string &strend = "$", int timeout = CHANNEL_READ_TIMEOUT);
	bool   Write(const string &data);
private:
	LIBSSH2_POLLFD *m_fds = NULL;
	Channel(const Channel&);
	Channel operator=(const Channel&);
private:
	LIBSSH2_CHANNEL *m_channel;
};

