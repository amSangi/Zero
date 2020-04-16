#include "render/Material.hpp"

using namespace zero::render;

Material::Material()
: name_()
, ambient_color_()
, diffuse_color_()
, emissive_color_()
, specular_color_()
, transparent_color_()
, specular_exponent_()
, opacity_()
, texture_map_()
, shaders_()
, wireframe_enabled_(false)
, visible_(true)
{}