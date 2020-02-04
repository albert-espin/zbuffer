//////////////////////////////////////////////////////////////////////////////
//
//  --- Angel.h ---
//
//   The main header file for all examples from Angel 6th Edition
//   Renamed to Common.h
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ANGEL_H__
#define __ANGEL_H__

//----------------------------------------------------------------------------
// 
// --- Include system headers ---
//

#include <cmath>
#include <iostream>
#include <stdlib.h>

//  Define M_PI in the case it's not defined in the math header file
#ifndef M_PI
#  define M_PI  3.14159265358979323846
#endif

//----------------------------------------------------------------------------
//
// --- Include OpenGL header files and helpers ---
//
//   The location of these files vary by operating system.  We've included
//     copies of open-soruce project headers in the "GL" directory local
//     this this "include" directory.
//

#define GL_GLEXT_PROTOTYPES 1

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#ifdef _WIN32
  #include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glext.h>
#endif


// Define a helpful macro for handling offsets into buffer objects
#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

//----------------------------------------------------------------------------
//
//  --- Include our class libraries and constants ---
//


namespace Common {


//  Defined constant for when numbers are too small to be used in the
//    denominator of a division operation.  This is only used if the
//    DEBUG macro is defined.
const GLfloat  DivideByZeroTolerance = GLfloat(1.0e-07);

//  Degrees-to-radians constant 
const GLfloat  DegreesToRadians = M_PI / 180.0;


}  // namespace Common

#include "vec.h"
#include "mat.h"

#define Print(x)  do { std::cerr << #x " = " << (x) << std::endl; } while(0)


//  Globally use our namespace in our example programs.
using namespace Common;

typedef struct {
      vec3 pmin;
      GLfloat a, h, p;
} Capsa3D;

typedef struct {
      vec2 pmin;
      float a, h;
} Capsa2D;



// Objecte vec4 que pot ser comparat amb == per determinar igualtat o diferència
class vec4_comparable{

public:

    vec4_comparable(vec4 vector);
    ~vec4_comparable();

    vec4 vector;

    // funció necessària per a usar vec4_comparable com a clau d'un hash map
    inline bool operator==(const vec4_comparable& other){
        return vector[0] == other.vector[0] && vector[1] == other.vector[1] && vector[2] == other.vector[2] && vector[3] == other.vector[3];
    }
};

// funció necessària per a usar vec4_comparable com a clau d'un hash map
inline bool operator< (const vec4_comparable& a, const vec4_comparable& b){

    // comparem component a component, d'esquerra a dreta, fins trobar una
    // desigualtat de components entre vectors que permeti dir quin és conceptualment
    // més petit (Nota: es podria pensar que fer servir la norma seria més senzill
    // per comparar quin vector és més petit, però si estan normalitzats sempre és 1!);
    // si els vectors són idèntics es retorna implícitament fals, ja que aquesta funció
    // és el "<" estricte, no el "<="

    if (a.vector[0] == b.vector[0]){

        if (a.vector[1] == b.vector[1]){

           if (a.vector[2] == b.vector[2]){

              return a.vector[3] < b.vector[3];

           } else return a.vector[2] < b.vector[2];

        } else return a.vector[1] < b.vector[1];

    } else return a.vector[0] < b.vector[0];
}






#endif // __ANGEL_H__
