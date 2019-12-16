#pragma once

#include <memory>
#include "OrthographicViewVolume.hpp"
#include "PerspectiveViewVolume.hpp"

namespace zero::render {

    /**
     * @brief Builder used to create IViewVolume instances given a camera
     */
    class ViewVolumeBuilder {
    public:
        ViewVolumeBuilder() = delete;
        ~ViewVolumeBuilder() = delete;

        /**
         * @brief Create an IViewVolume instance
         * @param camera the camera the view volume is associated with
         * @return an instance of IViewVolume based on the projection type of the camera.
         */
        static std::unique_ptr<IViewVolume> create(const Camera& camera);

    }; // class ViewVolumeBuilder
} // namespace zero::render