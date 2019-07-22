#pragma once

#include "render/Propagator.hpp"
#include "TestRegistry.hpp"

class TestPropagator : public TestRegistry {
public:
    void SetUp() override { TestRegistry::SetUp(); }

protected:
    zero::render::Propagator propagator_;

}; // class TestPropagator