//
// Created by 勾达不溜 on 25-1-6.
//

#include "Acceptor.h"

#include <iostream>

#include "Session.h"

namespace net {
    Acceptor::Acceptor(asio::io_context &io_context, const asio::ip::tcp::endpoint &endpoint):
        acceptor_(io_context, endpoint){}

    void Acceptor::start() {
        acceptor_.async_accept([this](const asio::error_code &ec, asio::ip::tcp::socket socket) {
            if (ec) {
                std::cerr << "Acceptor error: " << ec.message() << std::endl;
                return;
            }
            std::make_shared<Session>(std::move(socket))->start();
            start();
        });
    }
} // net
