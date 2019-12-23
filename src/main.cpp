#include "core/Engine.hpp"
#include "render/RenderSystem.hpp"

int main(int argc, char *argv[]) {
    std::shared_ptr<zero::Engine> engine = nullptr;
    zero::render::RenderSystemConfig render_system_config;
    zero::render::RenderSystem render_system{engine, render_system_config};
    return 0;
}