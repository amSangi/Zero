#pragma once

#include "core/System.hpp"
#include "core/IEntityInstantiator.hpp"

namespace zero
{
    /**
     * @brief A game system is a specialized System that is inherited from to manage different game mechanics
     */
    class GameSystem : public System
    {
    public:
        GameSystem(EngineCore* engine_core, IEntityInstantiator* entity_instantiator)
        : System(engine_core)
        , entity_instantiator_(entity_instantiator)
        {
        }

    protected:

        /**
         * @brief Get the entity factory class
         *
         * The entity instantiator helps instantiate game entities with certain components
         *
         * @return the entity instantiator
         */
        [[nodiscard]] inline IEntityInstantiator* GetInstantiator() const { return entity_instantiator_; }

    private:
        IEntityInstantiator* entity_instantiator_;

    };

} // namespace zero