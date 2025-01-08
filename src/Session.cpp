//
// Created by 勾达不溜 on 25-1-6.
//

#include "Session.h"

#include <iostream>

namespace net
{
    Session::Session(asio::ip::tcp::socket socket)
        : heartbeat_timer_(socket.get_executor()), socket_(std::move(socket))
    {
        read_buffer_.prepare(buffer_size_);
        write_buffer_.prepare(buffer_size_);
    }

    void Session::start()
    {
        try
        {
            std::cout << " Session started: " + socket_.remote_endpoint().address().to_string() << std::endl;
            start_heartbeat();
            do_read();
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << "\n";
            close();
        }
    }

    void Session::close()
    {
        if (socket_.is_open())
        {
            try
            {
                std::cout << "Session closed: " + socket_.remote_endpoint().address().to_string() << std::endl;
                socket_.close();
            }
            catch (const std::exception& e)
            {
                std::cerr << "Exception: " << e.what() << "\n";
            }
        }
    }


    void Session::handle_read_msg(std::size_t length)
    {
        const std::string received_data(asio::buffers_begin(read_buffer_.data()),
                                        asio::buffers_end(read_buffer_.data()) - 2);
        std::cout << socket_.remote_endpoint().address().to_string() << " : " << received_data << std::endl;
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 12\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Hello,World!";
        handle_write_msg(response);
    }

    void Session::handle_write_msg(const std::string& message)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (message.size() > buffer_size_)
        {
            std::cerr << "Message too big: " << message.size() << std::endl;
            return;
        }
        std::ostream os(&write_buffer_);
        os << message;
        do_write(message.size());
    }//非线程安全

    void Session::do_read()
    {
        auto self(shared_from_this());
        socket_.async_read_some(read_buffer_,
                                [this,self](const std::error_code& ec, const std::size_t length)
                                {
                                    if (!ec)
                                    {
                                        reset_heartbeat();
                                        read_buffer_.commit(length);
                                        handle_read_msg(length);
                                        read_buffer_.consume(length);
                                        do_read();
                                    }
                                    else
                                    {
                                        std::cerr << "Error reading from socket: " << ec.message() << "\n";
                                        close();
                                    }
                                });
    }

    void Session::do_write(const std::size_t length)
    {
        auto self(shared_from_this());
        write_buffer_.commit(asio::buffer_size(length));
        async_write(socket_, write_buffer_,
                    [this,self](const std::error_code& ec, const std::size_t length)
                    {
                        std::lock_guard<std::mutex> lock(mutex_);
                        if (!ec)
                        {
                            //发送成功
                            write_buffer_.consume(length);
                        }
                        else
                        {
                            std::cerr << "Error writing to socket: " << ec.message() << "\n";
                            close();
                        }
                    });
    }

    void Session::start_heartbeat()
    {
        heartbeat_timer_.expires_from_now(std::chrono::seconds(heartbeat_));
        heartbeat_timer_.async_wait([this](const std::error_code& ec)
        {
            if (!ec)
            {
                send_heartbeat();
            }
        });
    }

    void Session::send_heartbeat()
    {
        auto self(shared_from_this());
        std::string heartbeat = "HEARTBEAT";
        asio::async_write(socket_, asio::buffer(heartbeat + "\n"),
                          [this,self](const std::error_code& ec, std::size_t /*length*/)
                          {
                              if (!ec)
                              {
                                  reset_heartbeat();
                              }
                              else
                              {
                                  handle_heartbeat_failure(ec);
                              }
                          });
    }

    void Session::reset_heartbeat()
    {
        heartbeat_timer_.cancel();
        start_heartbeat();
    }

    void Session::handle_heartbeat_failure(const std::error_code& ec)
    {
        std::cerr << "Heartbeat failed: " << ec.message() << "\n";
        close();
    }
} // net
