#include "http.h"

// Libevent callbacks
static void request_callback(evhttp_request *req, void *arg)
{
    http::server * const server = reinterpret_cast<http::server*>(arg);
    http::request request;
    http::response response;
    evbuffer * buf;
    evkeyvalq *headers;
    // parse request
    request.set_command(evhttp_request_get_command(req));
    request.set_uri(evhttp_request_get_uri(req));
    for(
        evkeyval * header = headers->tqh_first; 
        header != nullptr; 
        header = header->next.tqe_next)
    {
        request.set_header(header->key,header->value);
    }
    buf = evhttp_request_get_input_buffer(req);
    size_t request_body_length = evbuffer_get_length(buf);
    if(request_body_length > 0)
    {
        std::vector<uint8_t> request_body(request_body_length);
        evbuffer_remove(
            buf,
            request_body.data(),
            request_body.size());
        request.set_body(std::move(request_body));
    }
    // get response from request
    server->handle_request(request,response);
    // dump response
    response.for_each_header([&](std::pair<std::string,std::string>& header)
    {
        evhttp_add_header(
            evhttp_request_get_output_headers(req),
            header.first.c_str(),
            header.second.c_str());
    });
    if(response.has_body())
    {
        buf = evhttp_request_get_output_buffer(req);
        std::vector<uint8_t> response_body;
        response.move_body_to(response_body);
        evbuffer_add(
            buf,
            response_body.data(),
            response_body.size());
    }
    else
    {
        buf = nullptr;
    }
    evhttp_send_reply(
        req,
        response.get_status(),
        response.get_message().c_str(),
        buf);
}

// Libevent callbacks
static void signal_callback(int sig, short events, void *arg)
{
    http::server * const server = reinterpret_cast<http::server*>(arg);
    server->stop();
}

http::server::server(const std::string& host, uint16_t port)
{
    _base = std::unique_ptr<event_base,std::function<void(event_base*)>>(
        event_base_new(),
        [](event_base* data)
        {
            event_base_free(data);
        });

    _term = std::unique_ptr<event,std::function<void(event*)>>(
        evsignal_new(_base.get(),SIGINT,signal_callback,this),
        [](event* data)
        {
            event_free(data);
        });
    
    event_add(_term.get(),nullptr);

    _http = std::unique_ptr<evhttp,std::function<void(evhttp*)>>(
        evhttp_new(_base.get()),[](evhttp* data)
        {
            evhttp_free(data);
        });
    
    evhttp_set_gencb(_http.get(),request_callback,this);

    evhttp_bind_socket(_http.get(),host.c_str(),port);
}

void http::server::start()
{
    event_base_dispatch(_base.get());
}

void http::server::stop()
{
    event_base_loopbreak(_base.get());
}