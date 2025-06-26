/*
** project01
*/

#ifndef PROJECT01_HPP_
    #define PROJECT01_HPP_
    
#include <raylib.h>
#include <string>

inline std::string binary_path;
inline std::string GET_ASSET_PATH(const std::string &name) {return (binary_path + "assets/" + name);};

#include "ECS/ECS.hpp"
#include "ECS/System.hpp"
#include "Utils.hpp"
#include "Game.hpp"
#include "Systems/Ships.hpp"
#include "Systems/Mouse.hpp"

/**
 * @brief Loads basic components into the ECS class
 * 
 * @param ecs 
 */
void LoadComponents(ECS::ECS &ecs);

#endif /* !PROJECT01_HPP_ */
