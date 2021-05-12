#pragma once

#include <string>
#include "component/Component.hpp"
#include "core/ZeroBase.hpp"

namespace zero
{

    /**
     * @brief An instance of a 3D model.
     */
    struct ModelInstance : public Component
    {

        /**
         * @brief The name of the 3D model the entity is associated with
         */
        std::string model_name_;

        /**
         * @brief The name of the model node.
         *
         * A 3D model may contain multiple nodes with different node names. The nodes can be in a hierarchy.
         */
        std::string node_name_;

        /**
         * @brief The mesh index into the model node
         *
         * Each node may contain multiple meshes. If the index is greater than the total number of meshes in a node,
         * the last mesh will be used.
         */
        uint32 mesh_index_;

    }; // struct ModelInstance

} // namespace zero
