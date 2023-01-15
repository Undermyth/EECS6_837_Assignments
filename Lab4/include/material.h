#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <GL/gl.h>

#include "vectors.h"
#include "ray.h"
#include "hit.h"

#include "glCanvas.h"
#ifdef SPECULAR_FIX
extern int SPECULAR_FIX_WHICH_PASS;
#endif

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(const Vec3f &d_color) { diffuseColor = d_color; }
  Material() {}
  virtual ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }
  virtual Vec3f getSpecularColor() const { return specularColor; }
  virtual Vec3f getReflectiveColor() const { return reflectiveColor; }
  virtual Vec3f getTransparentColor() const { return transparentColor; }
  virtual float getIndexOfReflection() const { return indexOfRefraction; }

  /* Added in Lab3 */
  /* ray is the viewing ray */
  virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                      const Vec3f &lightColor) const = 0;

  /* for OpenGL */
  virtual void glSetMaterial(void) const = 0;

protected:

  // REPRESENTATION
  Vec3f diffuseColor;
  Vec3f specularColor;
  Vec3f reflectiveColor;  // reflect color for ray tracing
  Vec3f transparentColor; // refract color for ray tracing
  float indexOfRefraction;// refraction ratio
  
};

// ====================================================================
// ====================================================================

class PhongMaterial: public Material {

public:

  // Constructors and destructor
  PhongMaterial(const Vec3f &_diffuseColor, const Vec3f &_specularColor, float _exponent,
                            const Vec3f &_reflectiveColor, const Vec3f &transparentColor,
                            float _indexOfRefraction);
  ~PhongMaterial() {};

  Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;

  void glSetMaterial(void) const;

protected:

  float exponent;         // specular exponent for Phong shading

};

#endif
