/*
** EPITECH PROJECT, 2025
** project01
** File description:
** Game
*/

#include "../include/Game.hpp"

#define TARGET_FPS 120

Game::Game()
{
    m_tickrate = 1000 / TARGET_FPS;
    LoadComponents(m_ecs);
}

Game::~Game()
{
}

void Game::InitGame()
{
    InitWindow(500, 500, "Among us is SUS");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(TARGET_FPS);
}

void Game::TickGame()
{
    double beforeTick = GetTime();
    double afterTick;

    // usleep(50000); // artificial load
    m_ecs.Update(this->m_tickrate);
    afterTick = GetTime();
    m_stats.mspt = (afterTick - beforeTick) * 1000;
}

void Game::RenderGame()
{
}

void Game::RunGame()
{
    double prevTick;

    // Unsynced processing of logic and rendering creates the following issue:
    // since we are using Raylib to render and control inputs, if 2 frames are processed
    // between 2 ticks, an input sent during the frame that is not processed at the same time
    // of a logic tick, then the input will be voided
    // /!\ Fix immediately !!

    // Temporary fix: we will process the logic tick every frame, but this is not optimal
    while (!WindowShouldClose()) {
        double currentTick = GetTime();

        m_stats.tps = std::clamp<double>(1000.0 / m_stats.mspt, 0.0, 1000.0 / m_tickrate);
        // if (currentTick - prevTick >= m_tickrate / 1000.0) {
        //     TickGame();
        // }
        TickGame();
        BeginDrawing();
        ClearBackground(DARKGRAY);
        RenderGame();
        EndDrawing();
        prevTick = currentTick;
        m_stats.fps = (uint32_t)GetFPS();
    }
}

ECS::ECS &Game::getECS()
{
    return m_ecs;
}
