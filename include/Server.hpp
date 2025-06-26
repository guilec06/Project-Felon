/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** Server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>

#include "Logger.hpp"

namespace inet {

    using socket_t = int;
    using ip_t = uint32_t;
    using port_t = uint16_t;
    typedef enum {
        SEND,
        RECEIVE,
        UNDEFINED
    } connection_mode_t;

    typedef struct {
        socket_t socket;
        port_t hard_port;
        bool running;
        connection_mode_t mode;
        sockaddr_in saddr;
    } connection_t;

    typedef struct {
        ip_t ip;
        port_t port;
    } remote_t;

    class Server {
        public:
            Server(std::string logtag = "SERVER", std::filesystem::path logpath = "./server.log");
            ~Server();

            std::shared_ptr<connection_t> connectExtern(ip_t address, port_t port);
            void disconnectExtern(ip_t address, port_t port);

            std::shared_ptr<connection_t> openConnection(port_t port);
            void closeConnection(port_t port);

            bool isSocket(socket_t sock);
            std::shared_ptr<connection_t> getConnection(port_t port);
            std::shared_ptr<connection_t> getConnection(ip_t ip, port_t port);

        protected:
            std::vector<std::shared_ptr<connection_t>> _connections;
        private:
    };

    // DO NOT TOUCH
    namespace thread_server {
        inline Server server_instance;
    }

    inline Server &getServerInstance()
    {
        return thread_server::server_instance;
    }
}

#endif /* !SERVER_HPP_ */
