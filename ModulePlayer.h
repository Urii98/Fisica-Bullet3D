#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 3000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	// Our functions
	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);


public:

	PhysVehicle3D* vehicle;
	bool turboActive;
	int turbo = 100;
	float turn;
	float acceleration;
	float brake;

	mat4x4 last_checkpoint_matrix;
	float dragForce;
	float massa;
	vec3 bouyancy;
    
	bool physics;
	vec3 gravity;
};