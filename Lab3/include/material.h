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

  /* Added in Lab3 */
  virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,
                      const Vec3f &lightColor) const = 0;

  /* for OpenGL */
  virtual void glSetMaterial(void) const = 0;

protected:

  // REPRESENTATION
  Vec3f diffuseColor;
  
};

// ====================================================================
// ====================================================================

class PhongMaterial: public Material {

public:

  // Constructors and destructor
  PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent);
  ~PhongMaterial() {};

  // Accessors
  virtual Vec3f getSpecularColor() const { return specularColor; }

  Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;

  void glSetMaterial(void) const;

protected:

  Vec3f specularColor;    // k_s for Phong shading
  float exponent;         // specular exponent for Phong shading

};

#endif
