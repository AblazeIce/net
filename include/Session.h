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
    void do_write(std::size_t length); //写入数据
    void handle_read_msg(std::size_t length);
    void handle_write_msg(const std::string& message);

    void start_heartbeat();
    void send_heartbeat();
    void reset_heartbeat();
    void handle_heartbeat_failure(const std::error_code& ec);


private:
    asio::streambuf read_buffer_;
    asio::streambuf write_buffer_;
    asio::steady_timer heartbeat_timer_;
    asio::ip::tcp::socket socket_;

    std::mutex mutex_;//write_buffer_存在线程竞争，需要加锁
private:
    int heartbeat_ = 5;
    int buffer_size_ = 1024; //心跳间隔时间，单位：秒
};

} // net

#endif //SESSION_H
