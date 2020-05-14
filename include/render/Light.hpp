#pragma once

#include "core/Component.hpp"
#include "math/Angle.hpp"
#include "math/Vector3.hpp"

namespace zero::render {

    /**
     * @brief Directional light component
     */
    struct DirectionalLight : public Component {
        DirectionalLight();
        math::Vec3f color_;
        math::Vec3f direction_;
        float intensity_;
    }; // struct DirectionalLight

    /**
     * @brief Point light component
     */
    struct PointLight : public Component {
        PointLight();
        math::Vec3f color_;
        float attenuation_constant_;
        float attenuation_linear_;
        float attenuation_quadratic_;
    }; // struct PointLight

    /**
     * @brief Spot light component
     */
    struct SpotLight : public Component {
        SpotLight();
        math::Vec3f color_;
        math::Degree inner_cone_angle_;
        math::Degree outer_cone_angle_;
    }; // struct SpotLight

    /**
     * @brief Light used to instantiate a light entity
     */
    struct Light {
        /**
         * @brief The Light type
         */
        enum Type {
            DIRECTIONAL,
            POINT,
            SPOT,
        };

        /**
         * @brief Default constructs a point light
         */
        Light();

        explicit Light(const DirectionalLight& directional_light);
        explicit Light(const PointLight& point_light);
        explicit Light(const SpotLight& spot_light);

        /**
         * @brief Get the light type
         * @return the light type
         */
        [[nodiscard]] Type GetType() const;

        /**
         * @brief Get the light component.
         *
         * If the instance is not of the correct type, a default constructed component is returned.
         *
         * @return the light component data
         */
        ///@{
        [[nodiscard]] DirectionalLight GetDirectionalLight() const;
        [[nodiscard]] PointLight GetPointLight() const;
        [[nodiscard]] SpotLight GetSpotLight() const;
        ///@}

        /**
         * @brief Set the light type
         */
        ///@{
        void Set(const DirectionalLight& directional_light);
        void Set(const PointLight& point_light);
        void Set(const SpotLight& spot_light);
        ///@}

    private:
        /**
         * @brief The underlying light union container of the different Light components.
         */
        union LightSource {
            explicit LightSource(const DirectionalLight& directional_light);
            explicit LightSource(const PointLight& point_light);
            explicit LightSource(const SpotLight& spot_light);
            DirectionalLight directional_light_;
            PointLight point_light_;
            SpotLight spot_light_;
        };

        Type type_;
        LightSource source_;
    };

} // namespace zero::render
