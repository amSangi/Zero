#pragma once

#include "render/VolumePropagator.hpp"
#include "TestTransformPropagator.hpp"

class TestVolumePropagator : public TestTransformPropagator {
public:
    void SetUp() override { TestTransformPropagator::SetUp(); }
}; // class TestVolumePropagator