#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	CreateTurbine();

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	Plane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}
Cube* ModuleSceneIntro::cubeCreation(vec3 position, vec3 size, Color rgb, float angle, vec3 pivot) {
	Cube* cube = new Cube;
	cube->SetRotation(angle, pivot);
	cube->SetPos(position.x, position.y, position.z);
	cube->size = size;
	cube->wire = false;
	cube->color = rgb;

	return cube;
}

PhysBody3D* ModuleSceneIntro::CreateCubePhysbody(Cube* cube, Module* Callback, TypeObject type, bool is_sensor, float mass){

	PhysBody3D* cubeP;
	cubeP = App->physics->AddBody(*cube, mass);
	cubeP->type = type;
	cubeP->AsSensor(is_sensor);
	cubeP->collision_listeners.add(Callback);


	return cubeP;
}
void ModuleSceneIntro::CreateTurbine()
{
	//without
	fan1 = SpinMachine(40, 4, 20, 2, 2, 10, 90, Red,1000000);
	mover1 = TorqueMove(40, 4, 20, 90,Red, 1000000);
	App->physics->AddConstraintHinge(*mover1.Pcubeinfo, *fan1.Pcubeinfo, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);
	////with
	//fan2 = SpinMachine(160, 6, 55, 10, 1, 1, 90);
	//mover2 = TorqueMove(160, 6, 55, 90);
	//App->physics->AddConstraintHinge(*mover2.Pcubeinfo, *fan2.Pcubeinfo, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);
	////with
	//fan3 = SpinMachine(60, 6, 45, 10, 1, 1, 90);
	//mover3 = TorqueMove(60, 6, 45, 90);
	//App->physics->AddConstraintHinge(*mover3.Pcubeinfo, *fan3.Pcubeinfo, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);
	////with
	//fan4 = SpinMachine(-40, 6, 55, 10, 1, 1, 90);
	//mover4 = TorqueMove(-40, 6, 55, 90);
	//App->physics->AddConstraintHinge(*mover4.Pcubeinfo, *fan4.Pcubeinfo, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);
	////with
	//fan5 = SpinMachine(-140, 6, 45, 10, 1, 1, 90);
	//mover5 = TorqueMove(-140, 6, 45, 90);
	//App->physics->AddConstraintHinge(*mover5.Pcubeinfo, *fan5.Pcubeinfo, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);
	////without
	//fan6 = SpinMachine(320, 4, -110, 2, 2, 10, 90, 10);
	//mover6 = TorqueMove(320, 4, -110, 90, 10);
	//App->physics->AddConstraintHinge(*mover6.Pcubeinfo, *fan6.Pcubeinfo, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, false);
	////with
	//fan7 = SpinMachine(260, 6, -50, 10, 1, 1, 90);
	//mover7 = TorqueMove(260, 6, -50, 90);
	//App->physics->AddConstraintHinge(*mover7.Pcubeinfo, *fan7.Pcubeinfo, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);
	////without
	//fan8 = SpinMachine(160, 4, -50, 10, 2, 2, 90, 10);
	//mover8 = TorqueMove(160, 4, -50, 90, 10);
	//App->physics->AddConstraintHinge(*mover8.Pcubeinfo, *fan8.Pcubeinfo, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, false);

}

compact_info ModuleSceneIntro::SpinMachine(int posx, int posy, int posz, int sizex, int sizey, int sizez, float angle, Color color, float mass)
{
	Cube* cube = cubeCreation(vec3(posx, posy, posz), vec3(sizex, sizey, sizez), color, angle, vec3(0, 1, 0));
	cubeList.add(cube);

	PhysBody3D* attacher;
	attacher = CreateCubePhysbody(cube, this, TypeObject::NONE, false, mass);
	cubePhysList.add(attacher);
	attacher->PointerGet()->setLinearFactor(btVector3(0, 0, 0));

	compact_info aux;
	aux.cubeinfo = cube;
	aux.Pcubeinfo = attacher;

	return aux;
}

compact_info ModuleSceneIntro::TorqueMove(int posx, int posy, int posz, float angle, Color color, float mass) {

	Cube* cube = cubeCreation(vec3(posx, posy, posz), vec3(1, 1, 1), color, angle, vec3(0, 1, 0));
	cubeList.add(cube);

	PhysBody3D* attached;
	attached = CreateCubePhysbody(cube, this, TypeObject::JOINT, false, mass);
	cubePhysList.add(attached);
	attached->PointerGet()->setLinearFactor(btVector3(0, 0, 0));

	compact_info aux;
	aux.cubeinfo = cube;
	aux.Pcubeinfo = attached;



	return aux;
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
}

