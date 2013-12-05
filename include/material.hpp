#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "color.hpp"

class Material {
    public: 
        Material () {};
        ~Material () {};
        Material(Color col, float diff, float refl);
        Material(const Material& mat);
        
        Color col;
        float diff;
        float refl;
};

#endif
