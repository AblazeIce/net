//
// Created by 勾达不溜 on 25-1-6.
//
#include <iostream>
#include "Net.h"

namespace net {
    Net::Net(short port, std::size_t thread_num)\
        : port_(port), thread_num_(thread_num), io_context_(),
          acceptor_(io_context_, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port_)) {
    }

    void Net::init() {
    }

    void Net::run() {
        try {
            acceptor_.start();
            std::cout << "Server started on port " << port_ << "\n";

            for (std::size_t i = 0; i < thread_num_; ++i) {
                threads_.emplace_back([this]() {
                    try {
                        io_context_.run();
                    } catch (const std::exception &e) {
                        std::cerr << "Thread Exception: " << e.what() << "\n";
                    }
                });
            }
            for (auto &t: threads_) {
                t.join();
            }
        } catch (const std::exception &e) {
            std::cerr << "Exception: " << e.what() << "\n";
        }
    }
}
