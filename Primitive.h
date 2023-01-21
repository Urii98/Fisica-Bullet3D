
#pragma once
#include "glmath.h"
#include "Color.h"
#define GL_GLEXT_PROTOTYPES 1
#define GL3_PROTOTYPES 1
#include <gl/GL.h>
#include <gl/GLU.h>
#include<glm/glm.hpp>
#include<vector>
#include "glut/glut.h"
#include"SDL/include/SDL_opengl_glext.h"


enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder,
	Primitive_Planea,
	Primitive_Road
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const vec3 &u);
	void			Scale(float x, float y, float z);
	float			GetX() const;
	float			GetY() const;
	float			GetZ() const;
	void			SetX(float x);
	void			SetY(float y);
	void			SetZ(float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	mat4x4 transform;
	bool axis,wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class Cube : public Primitive
{
public :
	Cube();
	Cube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	vec3 size;
};

// ============================================
class Sphere : public Primitive
{
public:
	Sphere();
	Sphere(float radius);
	void InnerRender() const;
public:
	float radius;
};

// ============================================
class Cylinder : public Primitive
{
public:
	Cylinder();
	Cylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class Line : public Primitive
{
public:
	Line();
	Line(float x, float y, float z);
	void InnerRender() const;
public:
	vec3 origin;
	vec3 destination;
};

// ============================================
class Plane : public Primitive
{
public:
	Plane();
	Plane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	vec3 normal;
	float constant;
};

class PlaneV : public Primitive
{
public:
	//v1: inferior derecha
	//v2: superior derecha
	//v3: superior izquierda
	//v4: inferior izquierda
	PlaneV(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4);
	void InnerRender() const;
	glm::vec3 GetNormal() const;
	std::vector<glm::vec3> GetVertices() const;
	float GetConstant() const;
	float GetRotation() const;
private:
	std::vector<glm::vec3> vertices;
	glm::vec3 normal;
	float constant;
};

class Road : public Primitive
{
public:
	Road();
	Road(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4);
	void InnerRender() const;
	std::vector<glm::vec3> GetVertices() const;
	//float GetRotation() const;
	//void CalculateRotation();

private:
	std::vector<glm::vec3> vertices;
	//float rotation;
}; 