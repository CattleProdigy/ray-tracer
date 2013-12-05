#include "material.hpp"
#include "color.hpp"

Material::Material(Color col, float diff, float refl) {
    this->col = col;
    this->diff = diff;
    this->refl = refl;
}

Material::Material(const Material& mat) {
    this->col = mat.col;
    this->diff = mat.diff;
    this->refl = mat.refl;
}
