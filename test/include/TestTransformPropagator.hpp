#pragma once

#include "core/TransformSystem.hpp"
#include "TestRegistry.hpp"

class TestTransformPropagator : public TestRegistry
{
public:
    void SetUp() override { TestRegistry::SetUp(); }
}; // class TestTransformPropagator