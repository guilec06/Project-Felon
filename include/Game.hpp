/*
** EPITECH PROJECT, 2025
** project01
** File description:
** Game
*/

#ifndef GAME_HPP_
    #define GAME_HPP_

#include "Project01.hpp"
#include <chrono>
#include <list>

#define DFAULT_TEXT_SIZE 20

struct EnvironmentStats {
    double mspt;         // Milliseconds per tick (time to calculate each tick)
    uint32_t fps;       // Frames per second
    double tps;        // Ticks per second (the numbers of ticks processed each second)
};

struct KeyInfo {
    bool isDown;
    bool isReleased;
    bool isPressed;
};

class CuteClock {
    private:
        std::list<std::pair<double, double>> timeStamps;
    public:
        CuteClock() {};
        ~CuteClock() {};

        bool isRunning() {return timeStamps.back().second == 0;}
        void Start()
        {
            if (isRunning())
                return;
            timeStamps.push_back(std::make_pair(GetTime(), 0.0));
        }

        void Stop()
        {
            if (!isRunning())
                return;
            timeStamps.back().second = GetTime();
        }

        void Restart()
        {
            for (size_t i = 0; i < timeStamps.size(); i++)
                timeStamps.pop_front();
        }

        double GetElapsed()
        {
            double total;
            for (auto it : timeStamps)
                total += ((it.second == 0 ? GetTime() : it.second) - it.first);
            return total;
        }
};

class Game {
    public:
        Game();
        ~Game();

        /**
         * @brief Initializes the game
         * 
         * This function will initialize the game, it will be called once at the beginning of the game.
         */
        void InitGame();

        /**
         * @brief Ticks the game
         * 
         * This function will update the game state, it will be called every ticks.
         */
        void TickGame();

        /**
         * @brief Renders the game
         * 
         * This function will render the game, it will be called every frame.
         */
        void RenderGame();

        /**
         * @brief Runs the game loop
         * 
         * This function will run the game loop, which will call TickGame and RenderGame
         * in a loop until the window is closed.
         */
        void RunGame();

        /**
         * @brief Returns the ECS instance
         * 
         * @return ECS::ECS& 
         */
        ECS::ECS &getECS();
    protected:
        // The entity component system
        ECS::ECS m_ecs;
        // Interval (in milliseconds) between each tick
        float m_tickrate;
        // Different statistics about the game
        EnvironmentStats m_stats;
        // The timer initialized when window is created
        CuteClock m_clock;
    private:
};

#endif /* !GAME_HPP_ */
