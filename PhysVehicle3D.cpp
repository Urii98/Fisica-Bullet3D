#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	Cylinder seient(1, 1.2f);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&seient.transform);
	btQuaternion qseient = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset3(info.seient_offset.x, info.seient_offset.y, info.seient_offset.z);
	offset3 = offset3.rotate(qseient.getAxis(), qseient.getAngle());
	seient.color = Blue;
	seient.transform.M[12] += offset3.getX();
	seient.transform.M[13] += offset3.getY();
	seient.transform.M[14] += offset3.getZ();

	Cube alero(info.alero_size.x, info.alero_size.y, info.alero_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&alero.transform);
	btQuaternion qalero = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset10(info.alero_offset.x, info.alero_offset.y, info.alero_offset.z);
	offset10 = offset10.rotate(qalero.getAxis(), qalero.getAngle());
	alero.color = Blue;
	alero.transform.M[12] += offset10.getX();
	alero.transform.M[13] += offset10.getY();
	alero.transform.M[14] += offset10.getZ();

	seient.Render();
	alero.Render();


	chassis.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}