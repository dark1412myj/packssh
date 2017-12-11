#include "Sftp.h"

bool Sftp::Open(string sftppath,long mode = LIBSSH2_FXF_READ | LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT )
{
    m_sftp_handle =
        libssh2_sftp_open(m_sftp_session, sftppath.c_str(), mode, 0);
    if (!m_sftp_handle)
    {
        return false;
    }
    return true;
}


bool Sftp::Close()
{
    if (m_sftp_handle)
    {
        libssh2_sftp_close(m_sftp_handle);
        m_sftp_handle = NULL;
        return true;
    }
    return false;
}


size_t Sftp::Write(char* data,size_t len)
{
    return libssh2_sftp_write(m_sftp_handle, data, len);
}

size_t Sftp::Read(char* data, size_t len)
{
    return libssh2_sftp_read(m_sftp_handle, data, len );
}



