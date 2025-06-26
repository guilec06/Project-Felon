/*
** EPITECH PROJECT, 2025
** project01
** File description:
** main
*/
#include <iostream>
#include <ECS/ECS.hpp>
#include <Game.hpp>
#include <Server.hpp>
#include <thread>

void connection_handle(Game &game, inet::Server &inet)
{

}

void start_server(Game &game)
{
    std::filesystem::create_directory("./logs");
}

int main()
{
    std::cerr << "Running server" << std::endl;

    Game game;
    start_server(game);

    return 0;
}
