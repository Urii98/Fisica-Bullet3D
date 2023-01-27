#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "PhysBody3D.h"
#include "Primitive.h"

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

struct compact_info
{
	Cube* cubeinfo;
	PhysBody3D* Pcubeinfo;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

	void CreateTurbine();

	PhysBody3D* CreateCubePhysbody(Cube* cube, Module* Callback, TypeObject type = TypeObject::NONE, bool is_sensor = false, float mass = 0.00f);
	Cube* cubeCreation(vec3 position, vec3 size, Color rgb, float angle = 0, vec3 pivot = 0);

	compact_info SpinMachine(int posx, int posy, int posz, int sizex, int sizey, int sizez, float angle,Color color, float mass = 10000.0f);
	compact_info TorqueMove(int posx, int posy, int posz, float angle, Color color, float mass = 10000.0f);


public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	p2List<PhysBody3D*> cubePhysList;
	p2List<Cube*> cubeList;

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;



	compact_info fan1;
	compact_info mover1;

	compact_info fan2;
	compact_info mover2;

	compact_info fan3;
	compact_info mover3;

	compact_info fan4;
	compact_info mover4;

	compact_info fan5;
	compact_info mover5;

	compact_info fan6;
	compact_info mover6;

	compact_info fan7;
	compact_info mover7;

	compact_info fan8;
	compact_info mover8;
};
