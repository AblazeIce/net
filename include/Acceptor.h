//
// Created by 勾达不溜 on 25-1-6.
//

#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include <asio.hpp>
namespace net {

class Acceptor {
public:
    Acceptor(asio::io_context& io_context, const asio::ip::tcp::endpoint& endpoint);

    void start();

private:
    asio::ip::tcp::acceptor acceptor_;
};

} // net

#endif //ACCEPTOR_H
