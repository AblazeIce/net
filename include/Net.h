//
// Created by 勾达不溜 on 25-1-6.
//

#ifndef NET_H
#define NET_H
#include <asio.hpp>
#include "Acceptor.h"

namespace net {

class Net {
public:
    Net(short port,std::size_t thread_num);


    void init();

    void run();

private:
    void start_accept();
private:
    short port_;
    std::size_t thread_num_;
private:
    std::vector<std::thread> threads_;
    asio::io_context io_context_;
    Acceptor acceptor_;
};

} // net

#endif //NET_H
