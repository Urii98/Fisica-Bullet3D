#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"
#include "Bullet/include/LinearMath/btVector3.h"

class btRigidBody;
class Module;

enum class TypeObject {

	WALL_ROAD,
	BOOST_SPEED,
	VEHICLE,
	TURBINE,
	OUT_OF_BOUNDS,
	START,
	JOINT,
	NONE

};

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void AsSensor(bool is_sensor)const;

	btVector3 GetLinearVelocity();

	btRigidBody* PointerGet();

	// Added functions
	void SetAsSensor(bool is_sensor);

private:
	btRigidBody* body = nullptr;

public:
	bool mutable is_sensor = false;
	TypeObject type = TypeObject::NONE;
	p2List<Module*> collision_listeners;
	bool is_sensor = false;
};

#endif // __PhysBody3D_H__