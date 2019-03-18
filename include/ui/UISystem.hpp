#pragma once

#include <memory>
#include "core/System.hpp"
#include "SFML/Window.hpp"

namespace zero {
namespace ui {

    /**
     * @brief The UI System manages the UI, Graphics Contexts, and Windowing of the game.
     */
    class UISystem : public System {
    public:

        /**
         * @brief UISystem Constructor
         * @param engine the game engine this ui system belongs to
         */
        explicit UISystem(std::shared_ptr<Engine> engine)
        : System(std::move(engine)) {}

        /**
         * @brief Initialize the window
         */
        void Initialize() override;

        /**
         * @brief Update the UISystem and post ui/input events
         * @param dt time between the current and last frame
         */
        void Update(float dt) override;

        /**
         * @brief Shutdown the UISystem
         */
        void ShutDown() override;

        /**
         * @brief Get the main game window
         * @note This is an alias to GetWindow
         * @return the window
         */
        inline const sf::Window& GetUIWindow() const { return window_; }

    protected:

        /**
         * @brief The game window
         */
        sf::Window window_;

    }; // class UISystem

} // namespace ui
} // namespace zero