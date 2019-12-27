#pragma once

#include <memory>
#include <vector>
#include "CoreEngine.hpp"
#include "EngineConfig.hpp"
#include "NonCopyable.hpp"
#include "System.hpp"
#include "TimeDelta.hpp"
#include "ZBase.hpp"

namespace zero {

    /**
     * @brief The Game Engine
     */
    class Engine : public NonCopyable {
    public:

        /**
         * @brief Create an Engine using the given configuration
         * @param config the Engine configuration
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
         * @brief Get a reference to the core engine
         * @return the CoreEngine
         */
        [[nodiscard]] CoreEngine* GetCoreEngine() const;

    protected:

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
        std::unique_ptr<CoreEngine> core_engine_;

        /**
         * @brief The systems that use the game data and objects to progress the game.
         */
        std::vector<std::unique_ptr<System>> game_systems_;

    }; // class Engine

} // namespace zero