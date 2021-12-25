#ifndef HTTPCONN_H
#define HTTPCONN_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <sys/uio.h>

class httpconn
{
public:
    static const int FILENAME_LEN = 200;
    static const int READ_BUFFER_SIZE = 2048;
    static const int WRITE_BUFFER_SIZE = 1024;
    enum METHOD
    {
        GET = 0,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT,
        PATH
    };
    enum CHECK_STATE
    {
        CHECK_STATE_REQUESTLINE = 0,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT
    };
    enum HTTP_CODE
    {
        NO_REQUEST,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURCE,
        FORBIDDEN_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION
    };
    enum LINE_STATUS
    {
        LINE_OK = 0,
        LINE_BAD,
        LINE_OPEN
    };

public:
    void init(int sockfd, const sockaddr_in &addr, char *root);
    
    void init();

    bool read_once();
    void process();
    LINE_STATUS parse_line();
    HTTP_CODE process_read();
    HTTP_CODE parse_request_line(char*);
    HTTP_CODE parse_headers(char*);
    HTTP_CODE parse_content(char*);
    HTTP_CODE do_request();
    char* get_line(){return m_read_buf + m_started_idx;}
    bool write();
    bool process_write(HTTP_CODE ret);
    bool add_response(const char* format, ...);
    bool add_status(int status, const char* title);
    bool add_headers(int content_length);
    bool add_content(const char *content);
    bool add_linger();
    bool add_blank_line();
    bool add_content_length(int content_length);
    void unmap();
    void close_conn(bool real_close = true);


public:
    char m_read_buf[READ_BUFFER_SIZE];
    int m_checked_idx;
    int m_read_idx;
    int m_started_idx;
    CHECK_STATE m_check_state;

public:
    char m_write_buf[WRITE_BUFFER_SIZE];
    int m_write_idx;
    struct iovec m_iv[2];

public:
    int m_sockfd;
    sockaddr_in m_address;
    int m_state;
    int improv;
    int time_flager;
    static int m_user_count;
    static int m_epollfd;
    METHOD m_method;
    char* m_url;
    char* m_version;
    int m_linger;
    char* m_host;
    char* m_string;
    int m_content_length;
    int cgi;
    char* doc_root;
    char m_real_file[FILENAME_LEN];
    struct stat m_file_stat;
    char* m_file_address;
    int m_iv_count;
    int bytes_to_send;
    int bytes_have_send;
    bool m_deleted;
};

#endif