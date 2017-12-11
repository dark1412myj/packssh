#include "Channel.h"

Channel::Channel(LIBSSH2_CHANNEL*channel)
    :m_channel(channel)
{

}

Channel::~Channel(void)
{
    if (m_channel)
    {
        libssh2_channel_free(m_channel);
        m_channel = NULL;
    }
}

vector<char> Channel::Read(const string &strend, int timeout)
{
    vector<char> data;
    if (NULL == m_channel)
    {
        return data;
    }

    if (m_fds == NULL)
    {
        LIBSSH2_POLLFD *fds = new LIBSSH2_POLLFD;
        fds->type = LIBSSH2_POLLFD_CHANNEL;
        fds->fd.channel = m_channel;
        fds->events = LIBSSH2_POLLFD_POLLIN | LIBSSH2_POLLFD_POLLOUT;
        m_fds = fds;
    }
    if (timeout % 50)
    {
        timeout += timeout % 50;
    }
    while (timeout>0)
    {
        int rc = (libssh2_poll(m_fds, 1, 10));
        if (rc < 1)
        {
            timeout -= 50;
            //usleep(50 * 1000);
            Sleep(50);
            continue;
        }

        if (m_fds->revents & LIBSSH2_POLLFD_POLLIN)
        {
            char buffer[64 * 1024] = "";
            //char buffer[32] = "";
            size_t n = libssh2_channel_read(m_channel, buffer, sizeof(buffer));
            if (n == LIBSSH2_ERROR_EAGAIN)
            {
            }
            else if (n == 0)
            {
                return data;
            }
            else if (n < 0)
            {
                throw data;
            }
            else
            {
                data.insert(data.end(),buffer,buffer + n );
                if ("" == strend)
                {
                    return data;
                }
                int pos = strend.size() - 1;
                int pos2 = data.size() - 1;
                while (pos >= 0 && pos2 >= 0 && data[pos2] == strend[pos])
                    pos--, pos2--;
                if (pos < 0)
                    return data;
            }
        }
        timeout -= 50;
        Sleep(50);
    }
    return data;
}
bool Channel::Write(const string &data)
{
    if (NULL == m_channel)
    {
        return false;
    }
    string send_data = data + "\n";
    return libssh2_channel_write_ex(m_channel, 0, send_data.c_str(), send_data.size()) == data.size();
}
