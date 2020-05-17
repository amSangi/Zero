#pragma once

#include "ZMath.hpp"

namespace zero::math
{

    /**
     * @brief Static class used to check if different shapes intersect
     */
    class Intersection
    {
    public:

        /**
         * @brief Check whether a box and a sphere intersect
         * @param box the box
         * @param sphere the sphere
         * @return True if the box and the sphere intersect. Otherwise false.
         */
        static bool BoxSphereIntersect(const Box& box, const Sphere& sphere);

    }; // class Intersection

} // namespace zero::math