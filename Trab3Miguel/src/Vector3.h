#ifndef __VECTOR_3_H__
#define __VECTOR_3_H__

#include <stdio.h>
#include <math.h>

class Vector3
{
public:
   float x, y, z;

   Vector3()
   {
      x = y = z = 0;
   }

   Vector3(float _x, float _y, float _z)
   {
       x = _x;
       y = _y;
       z = _z;
   }

   void set(float _x, float _y, float _z)
   {
       x = _x;
       y = _y;
       z = _z;
   }

   void normalize()
   {
       float norm = (float)sqrt(x*x + y*y + z*z);

       if(norm == 0.0)
       {
          printf("\n\nNormalize::Divisao por zero");
          x = 1;
          y = 1;
          z = 1;
          return;
       }
       x /= norm;
       y /= norm;
       z /= norm;
   }

   //Produto escalar (dot product: resulta em um float
   //Calcula ângulo entre vetores e iluminação
   float dot(const Vector3& v)
   {
       return x * v.x + y * v.y + z * v.z;
   }

   //Produto vetorial (cross product): resulta em um vetor perpendicular aos dois
   //Calcula normais de superfície
   Vector3 cross(const Vector3& v)
   {
       return Vector3(
           y * v.z - z * v.y,
           z * v.x - x * v.z,
           x * v.y - y * v.x
       );
   }

   Vector3 operator - (const Vector3& v)
   {
       Vector3 aux( x - v.x, y - v.y, z - v.z );
       return( aux );
   }

   Vector3 operator + (const Vector3& v)
   {
       Vector3 aux( x + v.x, y + v.y, z + v.z );
       return( aux );
   }

   Vector3 operator * (const Vector3& v)
   {
       Vector3 aux( x * v.x, y * v.y, z * v.z );
       return( aux );
   }

   Vector3 operator / (const Vector3& v)
   {
       Vector3 aux( x / v.x, y / v.y, z / v.z );
       return( aux );
   }

   //Multiplicação por escalar: útil para escala e direção
   Vector3 operator * (float s)
   {
       Vector3 aux( x * s, y * s, z * s );
       return( aux );
   }

   Vector3 operator / (float s)
   {
       Vector3 aux( x / s, y / s, z / s );
       return( aux );
   }

};

#endif
