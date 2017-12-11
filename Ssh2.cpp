#include "Ssh2.h"
#include <Ws2tcpip.h>

string Ssh2::s_password;

void Ssh2::S_KbdCallback(const char *name, int name_len,
                         const char *instruction, int instruction_len,
                         int num_prompts,
                         const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
                         LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses,
                         void **abstract)
{
    (void)name;
    (void)name_len;
    (void)instruction;
    (void)instruction_len;
    if (num_prompts == 1)
    {
        responses[0].text =  new char[s_password.size()+2];
        memcpy(responses[0].text, s_password.c_str(), s_password.size() + 1);
        responses[0].length = (int)s_password.size();
    }
    (void)prompts;
    (void)abstract;
}

Ssh2::Ssh2(const string &srvIp, int srvPort)
    :m_srvIp(srvIp), m_srvPort(srvPort)
{
    m_sock = -1;
    m_session = NULL;
    libssh2_init(0);
}

Ssh2::~Ssh2(void)
{
    Disconnect();
    libssh2_exit();
}

bool Ssh2::Connect(const string &userName, const string &userPwd)
{
    m_sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(22);
    sin.sin_addr.s_addr = inet_addr(m_srvIp.c_str());
    if (connect(m_sock, (sockaddr*)(&sin), sizeof(sockaddr_in)) != 0)
    {
        Disconnect();
        return false;
    }

    m_session = libssh2_session_init();
    if (libssh2_session_handshake(m_session, m_sock))
    {
        Disconnect();
        return false;
    }

    int auth_pw = 0;
    string fingerprint = libssh2_hostkey_hash(m_session, LIBSSH2_HOSTKEY_HASH_SHA1);
    string userauthlist = libssh2_userauth_list(m_session, userName.c_str(), (int)userName.size());
    if (strstr(userauthlist.c_str(), "password") != NULL)
    {
        auth_pw |= 1;
    }
    if (strstr(userauthlist.c_str(), "keyboard-interactive") != NULL)
    {
        auth_pw |= 2;
    }
    if (strstr(userauthlist.c_str(), "publickey") != NULL)
    {
        auth_pw |= 4;
    }

    if (auth_pw & 1)
    {
        if (libssh2_userauth_password(m_session, userName.c_str(), userPwd.c_str()))
        {
            Disconnect();
            return false;
        }
    }
    else if (auth_pw & 2)
    {
        s_password = userPwd;
        if (libssh2_userauth_keyboard_interactive(m_session, userName.c_str(), &S_KbdCallback))
        {
            Disconnect();
            return false;
        }
    }
    else
    {
        Disconnect();
        return false;
    }


    return true;
}

bool Ssh2::Disconnect(void)
{
    if (m_session)
    {
        libssh2_session_disconnect(m_session, "Bye bye, Thank you");
        libssh2_session_free(m_session);
        m_session = NULL;
    }
    if (m_sock != -1)
    {
#ifdef WIN32
        closesocket(m_sock);
#else
        close(m_sock);
#endif
        m_sock = -1;
    }
    return true;
}

Channel* Ssh2::CreateChannel(const string &ptyType)
{
    if (NULL == m_session)
    {
        return NULL;
    }

    LIBSSH2_CHANNEL *channel = NULL;
    if (!(channel = libssh2_channel_open_session(m_session)))
    {
        return NULL;
    }

    if (libssh2_channel_request_pty(channel, ptyType.c_str()))
    {
        libssh2_channel_free(channel);
        return NULL;
    }

    if (libssh2_channel_shell(channel))
    {
        libssh2_channel_free(channel);
        return NULL;
    }
    Channel *ret = new Channel(channel);
    ret->Read();
    return ret;
}
