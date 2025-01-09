//
// Created by 勾达不溜 on 25-1-6.
//

#ifndef SESSION_H
#define SESSION_H
#include <asio.hpp>

namespace net {

class Session:public std::enable_shared_from_this<Session> {
public:
    explicit Session(asio::ip::tcp::socket socket);

    void start();
    void close();
private:
    void do_read();
    void do_write(); //写入数据
    void handle_msg(std::size_t length);

    void start_heartbeat();
    void send_heartbeat();
    void reset_heartbeat();
    void handle_heartbeat_failure(const std::error_code& ec);


private:
    asio::streambuf read_buffer_;
    asio::streambuf write_buffer_;
    asio::steady_timer heartbeat_timer_;
    asio::ip::tcp::socket socket_;
private:
    int heartbeat_ = 5;
    int buffer_size_ = 1024;
};

} // net

#endif //SESSION_H
