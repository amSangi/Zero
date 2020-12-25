#pragma once

#include "component/Component.hpp"
#include "math/Angle.hpp"
#include "math/Vector3.hpp"

namespace zero
{

    /**
     * @brief Contains the constant, linear, and quadratic factors that are used to reduce the intensity of light rays
     * over larger distances.
     *
     * Formula: 1.0 / (constant + (linear * distance) + (quadratic * distance * distance))
     */
    struct Attenuation
    {
        Attenuation();

        /**
         * @brief Constant attenuation factor
         *
         * This is usually kept at 1.0 to ensure denominator never gets smaller than 1
         */
        float constant_;

        /**
         * @brief Linear attenuation factor
         *
         * Linearly reduce light intensity
         */
        float linear_;

        /**
         * @brief Quadratic attenuation factor
         *
         * Results in light quickly losing its brightness over distance until it eventually loses its brightness
         * at a slower pace.
         */
        float quadratic_;

    }; // struct Attenuation

    /**
     * @brief Directional light component
     */
    struct DirectionalLight : public Component
    {
        DirectionalLight();

        /**
         * @brief The color of the light with each coordinate in the range [0, 1]
         */
        math::Vec3f color_;

        /**
         * @brief The direction of the spot light
         *
         * Defaults to <0, -1, 0>
         */
        math::Vec3f direction_;

        /**
         * @brief The ambient intensity
         *
         * Controls the direct illumination on surfaces
         */
        float ambient_intensity_;

        /**
         * @brief The diffuse intensity.
         *
         * Controls the indirect illumination on surfaces
         */
        float diffuse_intensity_;

        /**
         * @brief Does this light source cast shadows?
         */
        bool casts_shadows_;

    }; // struct DirectionalLight

    /**
     * @brief Point light component
     */
    struct PointLight : public Component
    {
        PointLight();

        /**
         * @brief The color of the light with each coordinate in the range [0, 1]
         */
        math::Vec3f color_;

        /**
         * @brief The ambient intensity
         *
         * Controls the direct illumination on surfaces
         */
        float ambient_intensity_;

        /**
         * @brief The diffuse intensity.
         *
         * Controls the indirect illumination on surfaces
         */
        float diffuse_intensity_;

        /**
         * @brief The attenuation of the spot light
         */
        Attenuation attenuation_;

        /**
         * @brief Does this light source cast shadows?
         */
        bool casts_shadows_;

    }; // struct PointLight

    /**
     * @brief Spot light component
     */
    struct SpotLight : public Component
    {
        SpotLight();

        /**
         * @brief The color of the light with each coordinate in the range [0, 1]
         */
        math::Vec3f color_;

        /**
         * @brief The direction of the spot light
         *
         * Defaults to <0, -1, 0>
         */
        math::Vec3f direction_;

        /**
         * @brief The ambient intensity
         *
         * Controls the direct illumination on surfaces
         */
        float ambient_intensity_;

        /**
         * @brief The diffuse intensity.
         *
         * Controls the indirect illumination on surfaces
         */
        float diffuse_intensity_;

        /**
         * @brief The angle from the direction vector
         *
         * This angle defines the center of the cone where the light is brightest
         */
        math::Degree inner_cone_angle_;

        /**
         * @brief The larger angle from the direction vector
         *
         * This angle defines the smooth falloff radius from the inner cone angle.
         * The larger the distance between the inner and outer angles, the smoother and larger the falloff.
         */
        math::Degree outer_cone_angle_;

        /**
         * @brief The attenuation of the spot light
         */
        Attenuation attenuation_;

        /**
         * @brief Does this light source cast shadows?
         */
        bool casts_shadows_;

    }; // struct SpotLight

    /**
     * @brief Light used to instantiate a light entity
     */
    struct Light
    {
        /**
         * @brief The Light type
         */
        enum Type
        {
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
        union LightSource
        {
            explicit LightSource(const DirectionalLight& directional_light);
            explicit LightSource(const PointLight& point_light);
            explicit LightSource(const SpotLight& spot_light);
            DirectionalLight directional_light_;
            PointLight point_light_;
            SpotLight spot_light_;
        };

        Type type_;
        LightSource source_;
    }; // struct Light

} // namespace zero
