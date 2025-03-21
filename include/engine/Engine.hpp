#pragma once

#include <memory>
#include <vector>
#include "animation/AnimationSystem.hpp"
#include "engine/EngineConfig.hpp"
#include "engine/EntityInstantiator.hpp"
#include "core/EngineCore.hpp"
#include "core/NonCopyable.hpp"
#include "core/System.hpp"
#include "core/GameSystem.hpp"
#include "core/TimeDelta.hpp"
#include "core/ZeroBase.hpp"
#include "render/RenderSystem.hpp"

namespace zero
{

    /**
     * @brief The Game Engine
     */
    class Engine : public NonCopyable
    {
    public:

        /**
         * @brief Create an Engine using the given configuration
         * @param engine_config the Engine configuration data
         */
        explicit Engine(EngineConfig engine_config);

        /**
         * @brief Engine destructor
         */
        ~Engine() = default;

        /**
         * @brief Initialize the engine
         */
        void Initialize();

        /**
         * @brief Tick the engine
         */
        void Tick();

        /**
         * @brief Shutdown the engine
         */
        void ShutDown();

        /**
         * @brief Is the engine done ticking?
         * @return True if the engine is done ticking and should shut down. Otherwise false.
         */
        [[nodiscard]] bool IsDone() const;

        /**
         * @brief Get a raw reference to the engine core
         * @return the EngineCore
         */
        [[nodiscard]] EngineCore* GetEngineCore() const;

        /**
         * @brief Add a game system to the engine
         *
         * Note that the initial two parameters EngineCore and IEntityInstantiator are provided by the engine.
         * Therefore, `args` should be the constructor arguments after the EngineCore and IEntityInstantiator.
         *
         * @tparam GameSystemType a class that inherits from GameSystem
         * @tparam Args the constructor arguments
         * @param args the list of arguments to construct GameSystemType
         */
        template<class GameSystemType, typename... Args>
        inline void AddGameSystem(Args&&... args)
        {
            game_systems_.push_back(std::make_unique<GameSystemType>(engine_core_.get(),
                                                                     entity_instantiator_.get(),
                                                                     std::forward<Args>(args)...));
        }

    private:
        /**
         * @brief Update all events and post them onto the EventBus
         */
        void TickEvents();

    protected:

        /**
         * @brief The log title
         */
        static const char* kTitle;

        /**
         * @brief The engine configuration.
         */
        EngineConfig engine_config_;

        /**
         * @brief Container for all time related data (e.g. time since last frame, physics tick time, etc)
         */
        TimeDelta time_delta_;

        /**
         * @brief The engine core. Contains game data and objects that are used by many different Systems.
         */
        std::unique_ptr<EngineCore> engine_core_;

        /**
         * @brief The Animation system used to update entity animations
         */
        std::unique_ptr<animation::AnimationSystem> animation_system_;

        /**
         * @brief The Rendering system used to create a window/OpenGL context, render game entities, and instantiate 3D models.
         */
        std::unique_ptr<render::RenderSystem> render_system_;

        /**
         * @brief The systems that use the game data and objects to progress the game.
         */
        std::vector<std::unique_ptr<GameSystem>> game_systems_;

        /**
         * @brief The factory class that instantiates new entities with certain components
         */
        std::unique_ptr<EntityInstantiator> entity_instantiator_;

        /**
         * @brief Is the engine done ticking?
         */
        bool is_done_;

    }; // class Engine

} // namespace zero