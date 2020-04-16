#pragma once

#include "core/Component.hpp"
#include "math/Angle.hpp"
#include "math/Vector3.hpp"

namespace zero::render {

    /**
     * @brief Directional light data
     */
    struct DirectionalLight {
        math::Vec3f color_;
        float intensity_;
        // TODO: Fully implement Directional Light data structure
    }; // struct DirectionalLight

    /**
     * @brief Point light data
     */
    struct PointLight {
        math::Vec3f color_;
        float intensity_;
        float radius_;
    }; // struct PointLight

    /**
     * @brief Spot light data
     */
    struct SpotLight {
        math::Vec3f color_;
        float intensity_;
        math::Degree inner_cone_angle_;
        math::Degree outer_cone_angle_;
    }; // struct SpotLight

    /**
     * @brief Light component used to light the environment of a scene.
     * This can be attached to any entity to emit light of a specific type.
     */
    struct Light : public Component {

        Light();
        explicit Light(const DirectionalLight& directional_light);
        explicit Light(const PointLight& point_light);
        explicit Light(const SpotLight& spot_light);

        /**
         * @brief The light type
         */
        enum class Type {
            DIRECTIONAL,     ///< Directional light. Distance Illuminate all objects from the same direction and have an unidentifiable position.
            POINT,           ///< Point light. Located at a point in space and sends out light in all directions.
            SPOT,            ///< Spot light. Constrained to an angle resulting in a cone-shaped region of illumination.
        }; // enum class Type

        /**
         * @brief The light data
         */
        union LightSource {
            explicit LightSource(const DirectionalLight& directional_light);
            explicit LightSource(const PointLight& point_light);
            explicit LightSource(const SpotLight& spot_light);
            DirectionalLight directional_light_;
            PointLight point_light_;
            SpotLight spot_light_;
        }; // union LightSource

        /**
         * @brief Get the light type
         * @return the light type
         */
        [[nodiscard]] Type GetType() const;

        /**
         * @brief Get the directional light data
         *
         * If the type is not DIRECTIONAL, a default constructed Directional light is returned
         *
         * @return the directional light data
         */
        [[nodiscard]] DirectionalLight GetDirectionalLight() const;

        /**
         * @brief Get the point light data
         *
         * If the type is not POINT, a default constructed Point light is returned
         *
         * @return the point light data
         */
        [[nodiscard]] PointLight GetPointLight() const;

        /**
         * @brief Get the spot light data
         *
         * If the type is not SPOT, a default constructed Spot light is returned
         *
         * @return the spot light data
         */
        [[nodiscard]] SpotLight GetSpotLight() const;

        /**
         * @brief Set the light type and data
         */
        ///@{
        void Set(const DirectionalLight& directional_light);
        void Set(const PointLight& point_light);
        void Set(const SpotLight& spot_light);
        ///@}

    private:
        Type type_;
        LightSource light_source_;

    }; // struct Light

} // namespace zero::render
