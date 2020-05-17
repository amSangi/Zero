#pragma once

#include "math/Vector3.hpp"

namespace zero::render
{

    /**
     * @brief 3D Box data
     */
    struct Box
    {
        Box();
        bool operator==(const Box& other) const;
        bool operator!=(const Box& other) const;
        /**
         * @brief The width of the box (+x)
         */
        uint32 width_;
        /**
         * @brief The height of the box (+y)
         */
        uint32 height_;
        /**
         * @brief The depth of the box (+z)
         */
        uint32 depth_;
    }; // struct Box

    /**
     * @brief 2D Plane data
     */
    struct Plane
    {
        Plane();
        bool operator==(const Plane& other) const;
        bool operator!=(const Plane& other) const;
        /**
         * @brief The width of the plane (+u)
         */
        uint32 width_;
        /**
         * @brief The height of the plane (+v)
         */
        uint32 height_;
        /**
         * @brief The u axis of the plane
         *
         * The default is math::Vec3f::Right() (i.e. +x)
         */
        math::Vec3f u_axis_;
        /**
         * @brief The v axis of the plane
         *
         * The default is math::Vec3f::Forward(). (i.e. +z)
         */
        math::Vec3f v_axis_;
    }; // struct Plane

    /**
     * @brief Cone data
     */
    struct Cone
    {
        Cone();
        bool operator==(const Cone& other) const;
        bool operator!=(const Cone& other) const;
        /**
         * @brief The radius of the base of the cone
         */
        float radius_;
        /**
         * @brief The height of the cone
         */
        uint32 height_;
        /**
         * @brief Radial segments of the cone
         */
        uint32 radial_segments_;
        /**
         * @brief Height segments of the cone
         */
        uint32 height_segments_;
        /**
         * @brief Is the cone's base open?
         */
        bool is_open_ended_;
    }; // struct Cone

    /**
     * @brief Cylinder data
     */
    struct Cylinder
    {
        Cylinder();
        bool operator==(const Cylinder& other) const;
        bool operator!=(const Cylinder& other) const;
        /**
         * @brief The top radius of the cylinder
         */
        float top_radius_;
        /**
         * @brief The bottom radius of the cylinder
         */
        float bottom_radius_;
        /**
         * @brief The height of the cylinder
         */
        uint32 height_;
        /**
         * @brief Radial segments of the cylinder
         */
        uint32 radial_segments_;
        /**
         * @brief Height segments of the cylinder
         */
        uint32 height_segments_;
        /**
         * @brief Does the cylinder have an open top and bottom?
         */
        bool is_open_ended_;
    }; // struct Cylinder

    /**
     * @brief 3D UV-Sphere data
     */
    struct Sphere
    {
        Sphere();
        bool operator==(const Sphere& other) const;
        bool operator!=(const Sphere& other) const;
        /**
         * @brief Latitudinal segments of the sphere
         */
        uint32 latitude_count_;
        /**
         * @brief Longitudinal segments of the sphere
         */
        uint32 longitude_count_;
    }; // struct Sphere

    /**
     * @brief Torus data
     */
    struct Torus
    {
        Torus();
        bool operator==(const Torus& other) const;
        bool operator!=(const Torus& other) const;
        /**
         * @brief The central radius of the torus
         */
        float radius_;
        /**
         * @brief The radius of the outer tube
         */
        float tube_radius_;
        /**
         * @brief Radial segments of the torus
         */
        uint32 radial_segments_;
        /**
         * @brief Tubular segments of the torus
         */
        uint32 tubular_segments_;
    }; // struct Torus

} // namespace zero::render
