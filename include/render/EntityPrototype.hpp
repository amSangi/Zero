#pragma once

#include <memory>
#include "component/Animator.hpp"
#include "component/Material.hpp"
#include "component/ModelInstance.hpp"
#include "component/Volume.hpp"
#include "render/IMesh.hpp"

namespace zero::render
{

    class EntityPrototype
    {
    public:
        /**
         * @brief Constructor
         * @param the mesh of the EntityPrototype
         * @param material the material component prototype
         * @param volume the volume component prototype
         * @param model_instance the model instance component prototype
         */
        EntityPrototype(std::unique_ptr<IMesh> mesh,
                        Material material,
                        Volume volume,
                        ModelInstance model_instance);

        ~EntityPrototype() = default;

        /**
         * @brief Get the mesh
         * @return the mesh
         */
        [[nodiscard]] IMesh* GetMesh() const;

        /**
         * @brief Get the material prototype instance
         * @return the Material component
         */
        [[nodiscard]] const Material& GetMaterial() const;

        /**
         * @brief Get the volume prototype instance that encapsulates the entire mesh
         * @return the Volume component
         */
        [[nodiscard]] const Volume& GetVolume() const;

        /**
         * @brief Get the model instance prototype that describes the EntityPrototype
         * @return the ModelInstance component
         */
        [[nodiscard]] const ModelInstance& GetModelInstance() const;

    private:
        /**
         * @brief The mesh of the entity prototype
         */
        std::unique_ptr<IMesh> mesh_;

        /**
         * @brief Material prototype associated with the entity prototype
         */
        Material material_;

        /**
         * @brief Volume prototype encapsulating the mesh
         */
        Volume volume_;

        /**
         * @brief ModelInstance prototype that contains data that maps back to the entity prototype
         */
        ModelInstance model_instance_;

    }; // class EntityPrototype

} // namespace zero::render