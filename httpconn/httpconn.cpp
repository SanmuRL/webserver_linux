#include "httpconn.h"

const char *ok_200_title = "OK";
const char *error_400_title = "Bad Request";
const char *error_400_form = "Your request has bad syntax or is inherently impossible to staisfy.\n";
const char *error_403_title = "Forbidden";
const char *error_403_form = "You do not have permission to get file form this server.\n";
const char *error_404_title = "Not Found";
const char *error_404_form = "The requested file was not found on this server.\n";
const char *error_500_title = "Internal Error";
const char *error_500_form = "There was an unusual problem serving the request file.\n";

//对文件描述符设置非阻塞
int setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

//将内核事件表注册读事件，ET模式，选择开启EPOLLONESHOT
void addfd(int epollfd, int fd)
{
    epoll_event event;
    event.data.fd = fd;

    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLONESHOT;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

//从内核时间表删除描述符
void removefd(int epollfd, int fd)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

//将事件重置为EPOLLONESHOT
void modfd(int epollfd, int fd, int ev)
{
    epoll_event event;
    event.data.fd = fd;

    event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;

    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}


void httpconn::init(int sockfd, const sockaddr_in &addr, char *root)
{
    m_sockfd = sockfd;
    m_address = addr;

    addfd(m_epollfd, m_sockfd);
    m_user_count++;

    doc_root = root;

    //strcpy(sql_user, user.c_str());
    //strcpy(sql_passwd, passwd.c_str());
    //strcpy(sql_name, sqlname.c_str());
    init();
}

void httpconn::init()
{
    bytes_to_send = 0;
    bytes_have_send = 0;
    m_check_state = CHECK_STATE_REQUESTLINE;
    m_linger = false;
    m_method = GET;
    m_url = 0;
    m_version = 0;
    m_content_length = 0;
    m_host = 0;
    m_started_idx = 0;
    m_checked_idx = 0;
    m_read_idx = 0;
    m_write_idx = 0;
    cgi = 0;
    m_state = 0;
    time_flager = 0;
    improv = 0;
    m_deleted = false;

    memset(m_read_buf, '\0', READ_BUFFER_SIZE);
    memset(m_write_buf, '\0', WRITE_BUFFER_SIZE);
    memset(m_real_file, '\0', FILENAME_LEN);
}

int httpconn::m_user_count = 0;
int httpconn::m_epollfd = -1;

void httpconn::close_conn(bool real_close)
{
    unmap();
    if (real_close && (m_sockfd != -1))
    {
        printf("close %d\n", m_sockfd);
        removefd(m_epollfd, m_sockfd);
        //close(m_sockfd);
        m_sockfd = -1;
        m_user_count--;
        m_deleted = true;
    }
}

bool httpconn::read_once()
{
    if(m_read_idx >= READ_BUFFER_SIZE){
        return false;
    }
    int bytes_read = 0;
    while(true)
    {
        bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0);
        if(bytes_read == -1){
            if(errno == EAGAIN || errno == EWOULDBLOCK){
                break;
            }
            return false;
        }
        if(bytes_read == 0){
            return false;
        }
        m_read_idx += bytes_read;
    }
    return true;
}

httpconn::LINE_STATUS httpconn::parse_line()
{
    for(; m_checked_idx < m_read_idx; ++m_checked_idx)
    {
        if(m_read_buf[m_checked_idx] == '\r')
        {
            if(m_checked_idx + 1 == m_read_idx)
            {
                return LINE_OPEN;
            }
            else if(m_read_buf[m_checked_idx + 1] == '\n')
            {
                m_read_buf[m_checked_idx++] = '\0';
                m_read_buf[m_checked_idx++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
        else if(m_read_buf[m_checked_idx] == '\n')
        {
            if(m_checked_idx > 1 && m_read_buf[m_checked_idx - 1] == '\r')
            {
                m_read_buf[m_checked_idx - 1] = '\0';
                m_read_buf[m_checked_idx++] = '\0';
                return LINE_OK;
            }
            return LINE_BAD;
        }
    }
    return LINE_OPEN;
}

httpconn::HTTP_CODE httpconn::parse_request_line(char* text)
{
    m_url = strpbrk(text, " \t");
    *m_url++ = '\0';
    char* method = text;
    if(strcasecmp(method, "GET") == 0)
    {
        m_method = GET;
    }
    else if(strcasecmp(method, "POST") == 0)
    {
        m_method = POST;
    }
    else return BAD_REQUEST;
    m_url += strspn(m_url, " \t");
    m_version = strpbrk(m_url, " \t");
    if(!m_version) return BAD_REQUEST;
    *m_version++ = '\0';
    m_version += strspn(m_version, " \t");
    if(strcasecmp(m_version, "HTTP/1.1") != 0) return BAD_REQUEST;
    if(strncasecmp(m_url, "http://", 7) == 0)
    {
        m_url += 7;
    }
    else if(strncasecmp(m_url, "https://", 8) == 0)
    {
        m_url += 8;
    }
    m_url = strchr(m_url, '/');
    if(!m_url || m_url[0] != '/') return BAD_REQUEST;
    if(strlen(m_url) == 1)
    {
        strcat(m_url, "judge.html");
    }
    m_check_state = CHECK_STATE_HEADER;
    return NO_REQUEST;
}

httpconn::HTTP_CODE httpconn::parse_headers(char* text)
{
    if(text[0] == '\0')
    {
        if(m_content_length == 0) return GET_REQUEST;
        m_check_state = CHECK_STATE_CONTENT;
        return NO_REQUEST;
    }
    else if (strncasecmp(text, "Connection:", 11) == 0)
    {
        text += 11;
        text += strspn(text, " \t");
        if (strcasecmp(text, "keep-alive") == 0)
        {
            m_linger = true;
        }
    }
    else if (strncasecmp(text, "Content-length:", 15) == 0)
    {
        text += 15;
        text += strspn(text, " \t");
        m_content_length = atol(text);
    }
    else if (strncasecmp(text, "Host:", 5) == 0)
    {
        text += 5;
        text += strspn(text, " \t");
        m_host = text;
    }
    else
    {
        std::cout << "unknown header" << std::endl;
    }
    return NO_REQUEST;
}

httpconn::HTTP_CODE httpconn::parse_content(char* text)
{
    if (m_read_idx >= (m_content_length + m_checked_idx))
    {
        text[m_content_length] = '\0';
        //POST请求中最后为输入的用户名和密码
        m_string = text;
        return GET_REQUEST;
    }
    return NO_REQUEST;
}

httpconn::HTTP_CODE httpconn::do_request()
{
    strcpy(m_real_file, doc_root);
    int len = strlen(doc_root);
    const char* p = strrchr(m_url, '/');
    if(*(p + 1) == '1')
    {
        char* m_url_real = (char*)(malloc(sizeof(char) * 200));
        strcpy(m_url_real, "/log.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));
        free(m_url_real);
    }
    else if(*(p + 1) == '2')
    {
        char* m_url_real = (char*)(malloc(sizeof(char) * 200));
        strcpy(m_url_real, "/picture.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));
        free(m_url_real);
    }
    else if(*(p + 1) == '3')
    {
        char* m_url_real = (char*)(malloc(sizeof(char) * 200));
        strcpy(m_url_real, "/video.html");
        strncpy(m_real_file + len, m_url_real, strlen(m_url_real));
        free(m_url_real);
    }
    else
    {
        strncpy(m_real_file + len, m_url, strlen(m_url));
    }

    //std::cout << m_real_file << std::endl;
    
    if(stat(m_real_file, &m_file_stat) < 0){
        //std::cout << "no resource" << std::endl;
        return NO_RESOURCE;
    }
    
    if (!(m_file_stat.st_mode & S_IROTH))
        return FORBIDDEN_REQUEST;

    if (S_ISDIR(m_file_stat.st_mode))
        return BAD_REQUEST;
    
    int fd = open(m_real_file, O_RDONLY);
    m_file_address = (char*)mmap(0, m_file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    return FILE_REQUEST;
}

httpconn::HTTP_CODE httpconn::process_read()
{
    LINE_STATUS line_status = LINE_OK;
    HTTP_CODE ret = NO_REQUEST;
    char* text = 0;

    while((m_check_state == CHECK_STATE_CONTENT && line_status == LINE_OK) || (line_status = parse_line()) == LINE_OK)
    {
        text = get_line();
        m_started_idx = m_checked_idx;
        switch(m_check_state)
        {
            case CHECK_STATE_REQUESTLINE:
            {
                ret = parse_request_line(text);
                if(ret == BAD_REQUEST)
                {
                    return BAD_REQUEST;
                }
                break;
            }
            case CHECK_STATE_HEADER:
            {
                ret = parse_headers(text);
                if(ret == BAD_REQUEST)
                {
                    return BAD_REQUEST;
                }
                else if(ret == GET_REQUEST)
                {
                    return do_request();
                }
                break;
            }
            case CHECK_STATE_CONTENT:
            {
                ret = parse_content(text);
                if(ret == GET_REQUEST)
                {
                    return do_request();
                }
                line_status = LINE_OPEN;
                break;
            }
            default:
                return INTERNAL_ERROR;
        }
    }
    return NO_REQUEST;
}

bool httpconn::add_response(const char* format, ...)
{
    if(m_write_idx > WRITE_BUFFER_SIZE)
    {
        return false;
    }
    va_list ap;
    va_start(ap, format);
    int len = vsnprintf(m_write_buf + m_write_idx, WRITE_BUFFER_SIZE - m_write_idx - 1, format, ap);
    if(len >= (WRITE_BUFFER_SIZE - m_write_idx - 1))
    {
        va_end(ap);
        return false;
    }
    m_write_idx += len;
    va_end(ap);
    return true;
}

bool httpconn::add_status(int status, const char* title)
{
    return add_response("%s %d %s\r\n", "HTTP/1.1", status, title);
}
bool httpconn::add_headers(int content_length)
{
    return add_content_length(content_length) && add_linger() && add_blank_line();
}
bool httpconn::add_content(const char *content)
{
    return add_response("%s", content);
}
bool httpconn::add_linger()
{
    return add_response("Connection:%s\r\n", (m_linger == true) ? "keep-alive" : "close");
}
bool httpconn::add_blank_line()
{
    return add_response("%s", "\r\n");
}
bool httpconn::add_content_length(int content_length)
{
    return add_response("Content length:%d\r\n", content_length);
}

bool httpconn::process_write(HTTP_CODE ret)
{
    switch(ret)
    {
        case INTERNAL_ERROR:
        {
            add_status(500, error_500_title);
            add_headers(strlen(error_500_form));
            if(!(add_content(error_500_form)))
            {
                return false;
            }
            break;
        }
        case BAD_REQUEST:
        {
            add_status(404, error_404_title);
            add_headers(strlen(error_404_form));
            if(!(add_content(error_404_form)))
            {
                return false;
            }
            break;
        }
        case FORBIDDEN_REQUEST:
        {
            add_status(403, error_403_title);
            add_headers(strlen(error_403_form));
            if(!(add_content(error_403_form)))
            {
                return false;
            }
            break;
        }
        case FILE_REQUEST:
        {
            add_status(200, ok_200_title);
            if(m_file_stat.st_size != 0)
            {
                add_headers(m_file_stat.st_size);
                m_iv[0].iov_base = m_write_buf;
                m_iv[0].iov_len = m_write_idx;
                m_iv[1].iov_base = m_file_address;
                m_iv[1].iov_len = m_file_stat.st_size;
                m_iv_count = 2;
                bytes_to_send = m_write_idx + m_file_stat.st_size;
                return true;
            }
            else
            {
                const char *ok_string = "<html><body></body></html>";
                add_headers(m_file_stat.st_size);
                if (!add_content(ok_string))
                {
                    return false;
                }
            }
        }
    }
    m_iv[0].iov_base = m_write_buf;
    m_iv[0].iov_len = m_write_idx;
    m_iv_count = 1;
    bytes_to_send = m_write_idx;
    return true;
}

void httpconn::unmap()
{
    if(m_file_address)
    {
        munmap(m_file_address, m_file_stat.st_size);
        m_file_address = 0;
    }
}

bool httpconn::write()
{
    int temp = 0;
    if(bytes_to_send == 0)
    {
        modfd(m_epollfd, m_sockfd, EPOLLIN);
        init();
        return true;
    }
    while(1)
    {
        temp = writev(m_sockfd, m_iv, m_iv_count);
        if(temp < 0)
        {
            if(errno == EAGAIN)
            {
                modfd(m_epollfd, m_sockfd, EPOLLOUT);
                return true;
            }
            unmap();
            return false;
        }
        
        bytes_have_send += temp;
        bytes_to_send -= temp;

        if(bytes_have_send >= m_iv[0].iov_len)
        {
            m_iv[0].iov_len = 0;
            m_iv[1].iov_base = m_file_address + (bytes_have_send - m_write_idx);
            m_iv[1].iov_len = bytes_to_send;
        }
        else
        {
            m_iv[0].iov_len -= bytes_have_send;
            m_iv[0].iov_base = m_write_buf + bytes_have_send;
        }
        
        if(bytes_to_send <= 0)
        {
            //std::cout << "All send" << std::endl;
            unmap();
            modfd(m_epollfd, m_sockfd, EPOLLIN);
            if(m_linger)
            {
                init();
                return true;
            }
            else
            {
                return false;
            }
        }
    }
}

void httpconn::process()
{
    HTTP_CODE read_ret = process_read();
    if(read_ret == NO_REQUEST)
    {
        modfd(m_epollfd, m_sockfd, EPOLLIN);
        return;
    }
    bool write_ret = process_write(read_ret);
    if(!write_ret)
    {
        //std::cout << "!!!!!!!!!!!!!" << std::endl;
        close_conn(); 
    }
    modfd(m_epollfd, m_sockfd, EPOLLOUT);
}