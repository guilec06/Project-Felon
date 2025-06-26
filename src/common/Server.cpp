/*
** EPITECH PROJECT, 2025
** myftp
** File description:
** Server
*/

#include "Server.hpp"

namespace inet {
    Server::Server(std::string logtag, std::filesystem::path logpath)
    // : _logger(logtag, logpath)
    {
        // this->_logger.info("Server created.");
    }

    Server::~Server()
    {
        while (_connections.size() > 0) {
            std::shared_ptr<connection_t> con = _connections.front();
            if (con->running) {
                if (con->mode == RECEIVE)
                    this->closeConnection(ntohs(con->saddr.sin_port));
                if (con->mode == SEND)
                    this->disconnectExtern(con->saddr.sin_addr.s_addr, con->saddr.sin_port);
            }
        }
    }

    std::shared_ptr<connection_t> Server::connectExtern(ip_t address, port_t port)
    {
        struct sockaddr_in saddr;
        socket_t sock;

        // this->_logger.info(std::string() + "Establishing connection to " + inet_ntoa((in_addr){address}) + ":" + std::to_string(port));

        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock == -1) {
            // this->_logger.fatal(std::string() + "Couldn't create socket: " + strerror(errno));
            throw std::exception();
        }

        saddr.sin_addr.s_addr = address;
        saddr.sin_port = htons(port);
        saddr.sin_family = AF_INET;

        if (connect(sock, (const sockaddr *)&saddr, sizeof(saddr)) == -1) {
            // this->_logger.fatal(std::string() + "Couldn't establish connection: " + strerror(errno));
            throw std::exception();
        }

        std::shared_ptr<connection_t> con = std::make_shared<connection_t>();

        con->hard_port = port;
        con->mode = SEND;
        con->running = true;
        con->saddr = saddr;
        con->socket = sock;

        // this->_logger.info("Connection established.");
        _connections.push_back(con);
        return con;
    }

    void Server::disconnectExtern(ip_t address, port_t port)
    {
        // this->_logger.info(std::string() + "Disconnecting from " + inet_ntoa((in_addr){address}) + ":" + std::to_string(port));
        for (auto it = _connections.begin(); it != _connections.end();) {
            if ((*it)->saddr.sin_addr.s_addr == htonl(address) && (*it)->saddr.sin_port == htons(port) && (*it)->mode == SEND) {
                close((*it)->socket);
                it = _connections.erase(it);
                return;
            } else
                it++;
        }
        // this->_logger.warn("Connection doesn't exists");
    }

    std::shared_ptr<connection_t> Server::openConnection(port_t port)
    {
        struct sockaddr_in saddr;
        socket_t sock;

        // this->_logger.info("Attempting to create socket on port " + (port == 0 ? "ANY" : std::to_string(port)));
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (sock == -1) {
            // this->_logger.fatal("Couldn't create socket !");
            throw std::exception();
        }

        saddr.sin_addr.s_addr = INADDR_ANY;
        saddr.sin_port = htons(port);
        saddr.sin_family = AF_INET;

        if (bind(sock, (const sockaddr *)&saddr, sizeof(saddr)) != 0) {
            // this->_logger.fatal("Couldn't bind socket !");
            throw std::exception();
        }

        if (listen(sock, 64) != 0) {
            // this->_logger.fatal("Couldn't enable a listener on the socket !");
            throw std::exception();
        }

        std::shared_ptr<connection_t> con = std::make_shared<connection_t>();

        socklen_t len = sizeof(con->saddr);
        if (getsockname(sock, (sockaddr *)&(con->saddr), &len) == -1) {
            // this->_logger.fatal("Couldn't retrieve local server information");
            throw std::exception();
        }

        con->socket = sock;
        con->hard_port = port;
        con->running = true;
        con->mode = RECEIVE;

        // this->_logger.info(std::string() + "Socket " + std::to_string(sock) + " has been opened on port " + std::to_string(ntohs(con->saddr.sin_port)));
        _connections.push_back(con);
        return con;
    }

    void Server::closeConnection(port_t port)
    {
        // this->_logger.info(std::string() + "Closing port " + std::to_string(port) + "...");
        for (auto it = _connections.begin(); it != _connections.end();) {
            if (it.operator*()->mode == RECEIVE && it.operator*()->saddr.sin_port == htons(port)) {
                close((*it)->socket);
                it = _connections.erase(it);
                return;
            } else
                it++;
        }
        // this->_logger.warn("Connection doesn't exists");
    }

    bool Server::isSocket(socket_t sock)
    {
        for (auto &it : _connections) {
            if (it->socket == sock)
                return true;
        }
        return false;
    }

    std::shared_ptr<connection_t> Server::getConnection(port_t port)
    {
        for (auto it : _connections)
            if (it->mode == RECEIVE)
                if (it->saddr.sin_port == htons(port))
                    return it;
        return nullptr;
    }

    std::shared_ptr<connection_t> Server::getConnection(ip_t ip, port_t port)
    {
        for (auto it : _connections)
            if (it->mode == SEND)
                if (it->saddr.sin_addr.s_addr == htonl(ip) && it->saddr.sin_port == htons(port))
                    return it;
        return nullptr;
    }
}
