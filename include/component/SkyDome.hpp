#pragma once

#include "component/Component.hpp"

namespace zero
{

    struct SkyDome : public Component
    {
        /**
         * @brief Is the sky dome active?
         * A game may have multiple sky domes and switch between them.
         * The first active sky dome will be rendered. There is no particular order so a GameSystem must manage
         * which one sky dome to enable at any given moment.
         */
        bool is_active_;

        /**
         * @brief The vertex shader file
         */
        std::string vertex_shader_;

        /**
         * @brief The fragment shader file
         */
        std::string fragment_shader_;

    }; // struct SkyDome

} // namespace zero