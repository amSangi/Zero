#include "render/Material.hpp"

using namespace zero::render;

Material::Material()
: name_()
, texture_map_()
, shaders_()
, wireframe_enabled_(false)
, visible_(true)
{}