#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <getopt.h>
#include <io.h>
#include <fcntl.h>
#ifndef S_ISDIR
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#endif
#else /* !_WIN32 */
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif /* _WIN32 */
#include <signal.h>

#ifdef EVENT__HAVE_SYS_UN_H
#include <sys/un.h>
#endif
#ifdef EVENT__HAVE_AFUNIX_H
#include <afunix.h>
#endif
#include <event2/bufferevent_ssl.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/http.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/listener.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>
#include <openssl/x509v3.h>
#include <openssl/ssl.h>

#ifdef _WIN32
#include <event2/thread.h>
#endif /* _WIN32 */

#ifdef EVENT__HAVE_NETINET_IN_H
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#endif

#ifdef _WIN32
#ifndef stat
#define stat _stat
#endif
#ifndef fstat
#define fstat _fstat
#endif
#ifndef open
#define open _open
#endif
#ifndef close
#define close _close
#endif
#ifndef O_RDONLY
#define O_RDONLY _O_RDONLY
#endif
#endif /* _WIN32 */

#include <algorithm>
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
namespace http
{
    class message
    {
    public:
        message() = default;
        
        bool has_header(const std::string& key);
        std::string get_header(const std::string& key) const;
        void set_header(
            const std::string& key, const std::string& value) const;
        void remove_header(const std::string& key);
        
        void clear_headers();
        
        void for_each_header(
            std::function<void(std::pair<std::string,std::string>&)> function);

        bool has_body() const;

        void set_body(const uint8_t * const data, size_t size);

        void set_body(const std::vector<uint8_t>& buffer);

        void set_body(std::vector<uint8_t>&& buffer);

        std::vector<uint8_t> get_body() const;

        void copy_body_to(std::vector<uint8_t>& buffer);

        void move_body_to(std::vector<uint8_t>& buffer);

        void clear_body();

    protected:
        std::unordered_map<std::string,std::string> _headers;
        std::vector<uint8_t> _body;
    };

    class request : public message
    {
    public:
        request() = default;
        int get_command() const;
        void set_command(int command);
        std::string get_uri() const;
        void set_uri(const std::string& uri);
    private:
        int _command;
        std::string uri;
    };

    class response : public message
    {
    public:
        response() = default;
        int get_status() const;
        void set_status(int status);
        std::string get_message() const;
        void set_message(const std::string& message);
    private:
        int _status;
        std::string _message;
    };
    
    class server
    {

    public:
        server(const std::string& host, uint16_t port);
        server(const server& rhs) = delete;
        server(server&& rhs) = default;
        void start();
        void stop();
        void handle_request(const request& request, response& response);
    private:
        std::unique_ptr<event_base,std::function<void(event_base*)>> _base;
        std::unique_ptr<event,std::function<void(event*)>> _term;
        std::unique_ptr<evhttp,std::function<void(evhttp*)>> _http;
    };
}