/*
** EPITECH PROJECT, 2025
** ECS
** File description:
** ECS
*/

#ifndef ECS_HPP_
    #define ECS_HPP_

#include <cstddef>
#include <vector>
#include <queue>
#include <map>
#include <string>
#include <exception>
#include <system_error>
#include <algorithm>
#include <unistd.h>

#include "Errors.hpp"
#include "Includes.hpp"
#include "Component.hpp"
#include "System.hpp"

namespace ECS {
    class ECS {
        public:
            /**
             * @brief Construct a new ECS object
             * 
             * @param max_entities OPTIONAL specify a maximum entity count, 0 = infinite
             * @param use_as_power OPTIONAL specify if the max_entities is to be interpreted as a power of 2 or as a litteral limit (false by default)
             */
            ECS(std::size_t max_entities = 0, bool use_as_power = false)
            {
                for (std::size_t i = 0; i < 32; i++) {
                    m_entities.push_back(Entity());
                }
            }

            ~ECS()
            {
                for (const auto &it : m_comp_pools)
                    delete it.second;
                for (const auto &it : m_systems)
                    delete it.sys;
            }

            /**
             * @brief Returns the amount of currently active entities
             * 
             * @return std::size_t Currently active entities
             */
            std::size_t currentEntityCount()
            {
                return m_active_entities;
            }

            /**
             * @brief Checks if the specified entity's ID match an active entity
             * 
             * @param e EntityID
             * @return true if it exists
             * @return false if it doesn't
             */
            bool entityIsActive(EntityID e)
            {
                if (m_entities.size() <= e)
                    return false;
                return m_entities[e].isActive;
            }

            /**
             * @brief Create a new entity and returns its ID
             * 
             * @return EntityID 
             */
            EntityID entityCreate()
            {
                EntityID newId;

                if (m_available_ids.size() == 0) {
                    newId = m_id_counter++;
                } else {
                    newId = m_available_ids.front();
                    m_available_ids.pop();
                }
                if (m_active_entities >= m_entities.size()) {
                    std::size_t current_capacity = m_entities.size();
                    for (std::size_t i = 0; i < current_capacity; i++) {
                        m_entities.push_back(Entity());
                    }
                }
                m_entities[newId].isActive = true;
                m_entities[newId].group = NONE;
                m_active_entities++;
                return newId;
            }

            /**
             * @brief Set the group of an entity
             * 
             * @param id The ID of the entity
             * @param group The new group the entity should belong to
             */
            void entitySetGroup(EntityID id, EntityGroup group)
            {
                if (m_entities.size() <= id)
                    return;
                m_entities[id].group = group;
            }
            
            /**
             * @brief Delete an entity corresponding to an ID
             * 
             * @param e EntityID
             */
            void entityDelete(EntityID e)
            {
                if (m_entities.size() <= e)
                    return;
                if (!entityIsActive(e))
                    return;
                m_entities[e].isActive = false;
                m_available_ids.push(e);
                m_active_entities--;
                for (auto &it : m_comp_pools)
                    it.second->disableEntity(e);
            }

            /**
             * @brief Returns a vector containing the IDs of entities belonging to group 'group'
             * 
             * @param group The group to target
             * @return std::vector<EntityID> The list of entities
             */
            std::vector<EntityID> getEntityGroup(EntityGroup group)
            {
                std::vector<EntityID> grp;

                for (std::size_t i = 0; i < m_entities.size(); i++) {
                    if (m_entities[i].isActive && m_entities[i].group == group)
                        grp.push_back(i);
                }
                return grp;
            }

            /**
             * @brief Returns a vector containing the IDs of entities that have ALL specified components
             * 
             * @tparam Components The component types to check for (variadic template)
             * @return std::vector<EntityID> The list of entities that have all specified components
             */
            template<ComponentType... Components>
            std::vector<EntityID> getEntitiesByComponentsAllOf()
            {
                if constexpr (sizeof...(Components) == 0) {
                    return {};
                }
                
                std::vector<std::vector<EntityID>> component_lists;
                
                ((component_lists.push_back(componentExists<Components>() ? 
                    static_cast<ComponentPool<Components>*>(m_comp_pools[std::type_index(typeid(Components))])->getActiveEntities() : 
                    std::vector<EntityID>{})), ...);

                if (component_lists.empty()) {
                    return {};
                }
                
                auto smallest_it = std::min_element(component_lists.begin(), component_lists.end(),
                    [](const std::vector<EntityID>& a, const std::vector<EntityID>& b) {
                        return a.size() < b.size();
                    });

                std::vector<EntityID> result = *smallest_it;

                for (const auto& comp_list : component_lists) {
                    if (&comp_list == &(*smallest_it)) continue;
                    
                    std::vector<EntityID> intersection;
                    std::set_intersection(result.begin(), result.end(),
                                        comp_list.begin(), comp_list.end(),
                                        std::back_inserter(intersection));
                    result = std::move(intersection);
                    
                    if (result.empty()) break;
                }

                return result;
            }

            /**
             * @brief Returns a vector containing the IDs of entities that have AT LEAST ONE of the specified components
             * 
             * @tparam Components The component types to check for (variadic template)
             * @return std::vector<EntityID> The list of entities that have at least one of the specified components
             */
            template<ComponentType... Components>
            std::vector<EntityID> getEntitiesByComponentsAnyOf()
            {
                if constexpr (sizeof...(Components) == 0) {
                    return {};
                }

                std::vector<EntityID> result;
                
                auto addEntities = [&result](const std::vector<EntityID>& entities) {
                    for (EntityID id : entities) {
                        if (std::find(result.begin(), result.end(), id) == result.end()) {
                            result.push_back(id);
                        }
                    }
                };
                
                ((componentExists<Components>() ? 
                    addEntities(static_cast<ComponentPool<Components>*>(m_comp_pools[std::type_index(typeid(Components))])->getActiveEntities()) : 
                    void()), ...);
                
                std::sort(result.begin(), result.end());
                
                return result;
            }

            /**
             * @brief Registers a new component in the environment, the component can then be used withing the environment
             * 
             * @tparam T The type of the component to register
             */
            template<ComponentType T>
            void registerComponent()
            {
                std::type_index comp_index = std::type_index(typeid(T));

                if (m_comp_pools.find(comp_index) != m_comp_pools.end())
                    return;
                m_comp_pools.insert_or_assign(comp_index, new ComponentPool<T>());
            }

            /**
             * @brief Checks if the component type is registered
             * 
             * @tparam T The Component type to check for
             * @return true If it is registered.
             * @return false if it is not.
             */
            template<ComponentType T>
            bool componentExists()
            {
                return m_comp_pools.find(std::type_index(typeid(T))) != m_comp_pools.end();
            }

            /**
             * @brief Checks if an enity has a component attached to it
             * 
             * @tparam T The component type to check for
             * @param e EntityID - The ID of the entity
             * @return true If the entity exists AND has the component attached to IT
             * @return false Either if the entity doesn't exists or if the component isn't attached to it
             */
            template<ComponentType T>
            bool entityHasComponent(EntityID e)
            {
                if (!componentExists<T>())
                    throw ERROR::UnregisteredComponent(std::string(typeid(T).name()));
                return static_cast<ComponentPool<T>*>(m_comp_pools[std::type_index(typeid(T))])->hasComponent(e);
            }

            /**
             * @brief Add a new component to the specified entity
             * 
             * @tparam T The component type to add to the entity
             * @param e EntityID - The entity to add the component to
             * @return T& Reference to the newly created Component
             * @throw ERROR::UnregisteredComponent => if the component isn't registered
             * @throw ERROR::ComponentAlreadyAttached => if the component is ALREADY attached to the entity
             */
            template<ComponentType T>
            T &entityAddComponent(EntityID e)
            {
                std::type_index comp_index = std::type_index(typeid(T));

                if (entityHasComponent<T>(e))
                    throw ERROR::ComponentAlreadyAttached(e,std::string(typeid(T).name()));
                return static_cast<ComponentPool<T>*>(m_comp_pools[comp_index])->addComponent(e);
            }

            /**
             * @brief Gets the attached specified component to the specified entity
             * 
             * @tparam T The component type to get
             * @param e EntityID - The entity's ID
             * @return T& Reference to the associated component
             * @throw ERROR::UnregisteredComponent => if the component isn't registered
             * @throw ERROR::ComponentNotAttached => if the component is NOT attached to the entity
             */
            template<ComponentType T>
            T &entityGetComponent(EntityID e)
            {
                std::type_index comp_index = std::type_index(typeid(T));

                if (!entityHasComponent<T>(e))
                    throw ERROR::ComponentNotAttached(e, std::string(typeid(T).name()));
                return static_cast<ComponentPool<T>*>(m_comp_pools[comp_index])->getComponent(e);
            }

            /**
             * @brief Removes the attached component from the entity
             * 
             * @tparam T The component type
             * @param e The entity ID
             */
            template<ComponentType T>
            void entityRemoveComponent(EntityID e)
            {
                std::type_index comp_index = std::type_index(typeid(T));

                if (!entityHasComponent<T>(e))
                    throw ERROR::ComponentNotAttached(e, std::string(typeid(T).name()));
                static_cast<ComponentPool<T>*>(m_comp_pools[comp_index])->removeComponent(e);
            }

            /**
             * @brief Get the Pool object
             * 
             * @tparam T The component type of the Pool
             * @return ComponentPool<T>* The pointer to the pool
             * @throw ERROR::UnregisteredComponent => if the component isn't registered
             */
            template<ComponentType T>
            ComponentPool<T> &getPool()
            {
                if (!componentExists<T>())
                    throw ERROR::UnregisteredComponent(typeid(T).name());
                return *(static_cast<ComponentPool<T>*>(m_comp_pools[std::type_index(typeid(T))]));
            }

            /**
             * @brief Adds a new system to the ECS
             * 
             * @tparam T The system class
             * @param tickrate The ticks (calls to Update()) the system should skip after ticked
             * @return SystemID The new ID for the system
             */
            template<SystemClass T>
            SystemID addSystem(int tickrate = 0)
            {
                static SystemID id = 0;
                SystemData data;

                data.enabled = true;
                data.sys = new T();
                data.tickrate = tickrate;
                data.skipped_ticks = 0;
                m_systems.push_back(data);
                return id++;
            }

            /**
             * @brief Toggles the system on or off, defining if it should tick when Update() is called
             * 
             * @param id The id of the system to toggle
             */
            void toggleSystem(SystemID id)
            {
                if (id >= m_systems.size())
                    return;
                m_systems[id].enabled = !m_systems[id].enabled;
            }

            /**
             * @brief Checks if the specified system is enabled
             * 
             * @param sys The system's ID
             * @return true Is the system is enabled,
             * @return false if it is not
             */
            bool systemIsEnabled(SystemID sys)
            {
                if (sys >= m_systems.size())
                    return false;
                return m_systems[sys].enabled;
            }

            /**
             * @brief Updates every active systems
             * 
             */
            void Update(uint32_t msecs = 0)
            {
                for (SystemID i = 0; i < m_systems.size(); i++) {
                    auto &it = m_systems.at(i);
                    if (!it.enabled)
                        continue;
                    if (it.skipped_ticks >= it.tickrate) {
                        it.sys->Update(*this, i, msecs);
                        it.skipped_ticks = 0;
                    } else {
                        it.skipped_ticks++;
                    }
                }
            }

        private:
            EntityID m_id_counter = 0;
            std::queue<EntityID> m_available_ids;
            std::size_t m_active_entities = 0;
            std::vector<Entity> m_entities;
            std::map<std::type_index, IComponentPool*> m_comp_pools;
            std::vector<SystemData> m_systems;
    };
}

#endif /* !ECS_HPP_ */
