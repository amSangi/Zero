#pragma once

#include "component/Component.hpp"
#include "component/Shapes.hpp"

namespace zero
{
    /**
     * @brief An instance of a primitive.
     */
    struct PrimitiveInstance : public Component
    {

        /**
         * @brief The primitive type
         */
        enum class Type
        {
            BOX,
            CONE,
            CYLINDER,
            PLANE,
            SPHERE,
            TORUS,
        }; // enum class Type

        /**
         * @brief Default constructs a unit box
         */
        PrimitiveInstance();

        /**
         * @brief Construct a primitive of a given type
         */
        ///@{
        explicit PrimitiveInstance(const Box& box);
        explicit PrimitiveInstance(const Sphere& sphere);
        explicit PrimitiveInstance(const Plane& plane);
        explicit PrimitiveInstance(const Cone& cone);
        explicit PrimitiveInstance(const Cylinder& cylinder);
        explicit PrimitiveInstance(const Torus& torus);
        ///@}

        /**
         * @brief Get the primitive type
         * @return the primitive type
         */
        [[nodiscard]] Type GetType() const;

        /**
         * @brief Change the primitive
         */
        ///@{
        void Set(const Box& box);
        void Set(const Sphere& sphere);
        void Set(const Plane& plane);
        void Set(const Cone& cone);
        void Set(const Cylinder& cylinder);
        void Set(const Torus& torus);
        ///@}

        /**
         * @brief Get the primitive data
         *
         * If the primitive instance type and the accessor do not match, a default constructed value is returned.
         */
        ///@{
        [[nodiscard]] Box GetBox() const;
        [[nodiscard]] Sphere GetSphere() const;
        [[nodiscard]] Plane GetPlane() const;
        [[nodiscard]] Cone GetCone() const;
        [[nodiscard]] Cylinder GetCylinder() const;
        [[nodiscard]] Torus GetTorus() const;
        ///@}

    private:
        /**
         * @brief The primitive data
         */
        union Primitive
        {
            /**
             * @brief Construct a primitive of a given type
             */
            ///@{
            explicit Primitive(const Box& box);
            explicit Primitive(const Sphere& sphere);
            explicit Primitive(const Plane& plane);
            explicit Primitive(const Cone& cone);
            explicit Primitive(const Cylinder& cylinder);
            explicit Primitive(const Torus& torus);
            ///@}

            Box box_;
            Sphere sphere_;
            Plane plane_;
            Cone cone_;
            Cylinder cylinder_;
            Torus torus_;
        }; // union Primitive

        Type type_;
        Primitive primitive_;

    }; // struct PrimitiveInstance

} // namespace zero
