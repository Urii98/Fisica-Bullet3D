#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Color.h"
#include <iostream>
#include "ModulePlayer.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	prueba = 0.0f;
	LOG("Loading Intro assets");
	bool ret = true;

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	//vec3 position = { 0,0,0 };
	//AddCube(vec3{ 0,0,0 }, vec3{ 10000,1,10000 }, Green, 0, 0, 0, 0);

	// ======================================================
	//						SINUS GRAPH
	// ======================================================
	//int numCubes = 100;
	//float step = 2.0f / numCubes;
	//vec3 scale(20, 20, 20);
	//scalarSize = 7.5f;
	//scale *= step;
	//vec3 position(0, 0, 0);

	//for (int i = 0; i < numCubes; i++)
	//{
	//	graphCubes.emplace_back(std::make_unique<Cube>(scale.x, scale.y, scale.z));
	//	
	//	float posX = ((i + 1.0f)*step - 1.0f) * scalarSize;
	//	float posY = posX * posX;

	//	graphCubes.at(i).get()->SetPos(posX, posY, 10);

	//	float rate = 8.0 / numCubes;
	//	float aux = i * rate;
	//	
	//	graphCubes.at(i).get()->color = GetColor(aux);

	//	
	//}
	

	// ======================================================
	//						SPIRAL ROAD
	// ======================================================
	
	
	//CreateSpiralRoad(25, 75, 50);
	
	// ======================================================
	//						Checkpoint and laps Code
	// ======================================================

	checkpointCube = { 35, 5, 35 };
	checkpointCube.color = { 255, 0, 50 };

	sensor = App->physics->AddBody(checkpointCube, 0.0f);
	sensor->SetAsSensor(true);
	sensor->SetPos(13, 200, 110);
	checkpointCube.SetPos(13, 200, 110);

	sensorCounter = 0;
	raceState = LAP1;
	numOfLaps = 0;
	restartTheGame = false;

	// -----------------------------


	btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

	btDefaultMotionState* myMotionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

	btRigidBody* body = new btRigidBody(rbInfo);


	// Coordenadas de los v�rtices de la carretera
	//glm::vec3 v1(0, 0, 0);
	//glm::vec3 v2(10, 0, 0);
	//glm::vec3 v3(10, 0, 10);
	//glm::vec3 v4(0, 0, 10);

	// Calculamos la inclinaci�n de la subida
	//float inclinacion = 0.3f;

	//Rampa de Izquierda a Derecha
	// Aplicamos la inclinaci�n a los v�rtices
	//v1.y = inclinacion * v1.x;
	//v2.y = inclinacion * v2.x;
	//v3.y = inclinacion * v3.x;
	//v4.y = inclinacion * v4.x;
	
	//Rampa de Norte a Sur
	//v4.y = inclinacion * v4.z;
	//v3.y = inclinacion * v3.z;
	//v2.y = inclinacion * v2.z;
	//v1.y = inclinacion * v1.z;

	//Rampa de Derecha a Izquierda
	// Aplicamos la inclinaci�n a los v�rtices
	//v1.y = inclinacion * 10;
	//v2.y = inclinacion * 0;
	//v3.y = inclinacion * 0;
	//v4.y = inclinacion * 10;

	//Rampa de Sur a Norte
	//v1.y = inclinacion * 10;
	//v2.y = inclinacion * 10;
	//v3.y = inclinacion * 0;
	//v4.y = inclinacion * 0;

	//CreateSpiralRamp(1, 2, 20, 2, -2, 0, 0);

	CreateSpiralRoad(125, 200, 50, 8);
	CreateMarioKartMap();
	CreateRamps();

	raceTimer = 0;
	endGameTimer = 0;
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
	if (raceTimer == 0)
	{
		raceTimer = SDL_GetTicks();
	}

	if (endGameTimer == 0)
	{
		endGameTimer = SDL_GetTicks();
	}


// ======================================================
//						SPIRAL ROAD RENDER	
// ======================================================
	for(auto& planeV: vectorPlaneV)
	{
		planeV.Render();
	}
	prueba += dt;
	
// ======================================================
//						SINUS GRAPH UPDATE
// ======================================================
	for (auto& graph : graphCubes)
	{
		float d = abs(graph.get()->GetX());

		float posY = sin(M_PI * (graph.get()->GetX()/ scalarSize) + prueba*2.0f)*20.0f;
		
	/*	float posY = sin(M_PI*(4.0f * d - prueba)) * 20;
		posY /= (1.0f + 10.0f * d);*/
		
		graph.get()->SetY(posY);

		graph->Render();
	}

	for (auto& road : vectorRoad)
	{
		road.Render();
	}

	for (auto& cube : vectorCubes)
	{
		cube.Render();
	}

	fan1.Pcubeinfo->GetTransform(&fan1.cubeinfo->transform);
	fan1.cubeinfo->Render();


	p2List_item <Cube*>* cube_render = cubeList.getFirst();
    while (cube_render != nullptr) {

        cube_render->data->Render();
        cube_render = cube_render->next;
		


    }


	checkpointCube.Render();

	//for (auto& primitive : primitives) {
	//	primitive->Render();
	//}

	// TIME CONTROLS-----------------------
	// PER CANVIAR EL TEMPS, TOCAR EL 300 (en segons)

	if (SDL_GetTicks() - raceTimer >= 300 * 1000 && raceState != WIN)
	{
		raceState = LOSE;
	}

	// What happends when second timer ends
	// PER CANVIAR EL TEMPS de derrota, TOCAR EL 305 (en segons)
	// Ha d'haver-hi uns segons de diferència entre el temps de la cursa (ara es 300) i la derrota
	// ara es 305. Per tant, la derrota abans de fer el reinici de la partida son 5 segons
	if (raceState == LOSE && (SDL_GetTicks() - endGameTimer) >= 305 * 1000)
	{
		raceTimer = 0;
		endGameTimer = 0;

		raceState = LAP1;
		restartTheGame = true;

		numOfLaps = 0;
		sensorCounter = 0;
		sensor->SetPos(13, 200, 110);
		checkpointCube.SetPos(13, 200, 110);
	}

	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		raceState = WIN;
	}
	if (App->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
	{
		raceTimer = 0;
		endGameTimer = 0;

		raceState = LAP1;
		restartTheGame = true;

		numOfLaps = 0;
		sensorCounter = 0;
		sensor->SetPos(13, 200, 110);
		checkpointCube.SetPos(13, 200, 110);
	}

	// After win or lose and wait 5 seconds, restart the game
	// Check module player for more things that are restarting with the bool "restartTheGame"



	//DEBUG LAP VARIABLES:
	/*printf("VARIABLES\n");
	printf("SensorCounter: %d\n", sensorCounter);
	printf("Race State %d\n", raceState);
	printf("### Num of Laps %d\n", numOfLaps);*/
	printf("%d\n", (SDL_GetTicks() - App->scene_intro->raceTimer) / 1000);

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
	
	fan1 = SpinMachine(40, 202, 40, 2, 2, 10, 90, Red,1000000);
	mover1 = TorqueMove(40, 202, 40, 90,Red, 1000000);
	App->physics->AddConstraintHinge(*mover1.Pcubeinfo, *fan1.Pcubeinfo, vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, 0, 0), true, true);


}

compact_info ModuleSceneIntro::SpinMachine(int posx, int posy, int posz, int sizex, int sizey, int sizez, float angle, Color color, float mass)
{
	Cube* cube = cubeCreation(vec3(posx, posy, posz), vec3(sizex, sizey, sizez), color, angle, vec3(0, 1, 0));
	cubeList.add(cube);
	/*efe.push_back(cube);*/

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
	/*efe.push_back(cube);*/

	PhysBody3D* attached;
	attached = CreateCubePhysbody(cube, this, TypeObject::JOINT, false, mass);
	cubePhysList.add(attached);
	attached->PointerGet()->setLinearFactor(btVector3(0, 0, 0));

	compact_info aux;
	aux.cubeinfo = cube;
	aux.Pcubeinfo = attached;



	return aux;
}

void ModuleSceneIntro::AddCube(vec3 position, vec3 size, Color RGB, int angle, bool rotateX, bool rotateY, bool rotateZ)
{
	Cube cubeToAdd;

	cubeToAdd.color = RGB;

	cubeToAdd.SetPos(position.x, position.y, position.z);

	cubeToAdd.size = size;

	// angle, XYZ
	if (rotateX == true) {
		cubeToAdd.SetRotation(angle, { 1, 0, 0 });
	}
	if (rotateY == true) {
		cubeToAdd.SetRotation(angle, { 0, 1, 0 });
	}
	if (rotateZ == true) {
		cubeToAdd.SetRotation(angle, { 0, 0, 1 });
	}

	App->physics->AddBody(cubeToAdd, 0);
	vectorCubes.push_back(cubeToAdd);
	
	
}

void ModuleSceneIntro::AddSphere(vec3 position, float radius, Color RGB, int angle, bool rotateX, bool rotateY, bool rotateZ)
{
	Sphere sphereToAdd;

	sphereToAdd.color = RGB;

	sphereToAdd.SetPos(position.x, position.y, position.z);

	sphereToAdd.radius = radius;

	// angle, XYZ
	if (rotateX == true) {
		sphereToAdd.SetRotation(angle, { 1, 0, 0 });
	}
	if (rotateY == true) {
		sphereToAdd.SetRotation(angle, { 0, 1, 0 });
	}
	if (rotateZ == true) {
		sphereToAdd.SetRotation(angle, { 0, 0, 1 });
	}
	App->physics->AddBody(sphereToAdd, 0);
	

	//scenerySpheres.add(sphereToAdd);
}

void ModuleSceneIntro::AddCylinder(vec3 position, float radius, float height, Color RGB, int angle, bool rotateX, bool rotateY, bool rotateZ)
{
	Cylinder cylinderToAdd;

	cylinderToAdd.color = RGB;

	cylinderToAdd.SetPos(position.x, position.y, position.z);

	cylinderToAdd.radius = radius;

	cylinderToAdd.height = height;

	// angle, XYZ
	if (rotateX == true) {
		cylinderToAdd.SetRotation(angle, { 1, 0, 0 });
	}
	if (rotateY == true) {
		cylinderToAdd.SetRotation(angle, { 0, 1, 0 });
	}
	if (rotateZ == true) {
		cylinderToAdd.SetRotation(angle, { 0, 0, 1 });
	}
	App->physics->AddBody(cylinderToAdd, 0);
	

	//sceneryCylinders.add(cylinderToAdd);
}

void ModuleSceneIntro::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body2 == sensor)
	{
		switch (sensorCounter)
		{
			// linia de meta
		case 0:
			sensor->SetPos(13, 200, 110);
			checkpointCube.SetPos(13, 200, 110);
			sensorCounter++;
			break;
			// sensor en posicio 1
		case 1:
			sensor->SetPos(220, 200, 160);
			checkpointCube.SetPos(220, 200, 160);
			sensorCounter++;
			break;
			// sensor en posicio 2
		case 2:
			sensor->SetPos(385, 200, 50);
			checkpointCube.SetPos(385, 200, 50);
			sensorCounter++;
			break;
			// sensor en posicio 3
		case 3:
			sensor->SetPos(350, 200, 330);
			checkpointCube.SetPos(350, 200, 330);
			sensorCounter++;
			break;
		case 4:
			sensor->SetPos(60, 200, 335);
			checkpointCube.SetPos(60, 200, 335);
			sensorCounter++;
			break;
		default:
			break;
		}

		// si player passa l'ultim sensor, col.loca de nou el sensor a la meta i suma 1 lap
		if (sensorCounter > 4 && numOfLaps < 4)
		{
			sensorCounter = 0;
			numOfLaps++;
		}

		switch (numOfLaps)
		{
		case 1:
			raceState = LAP1;
			break;
		case 2:
			raceState = LAP2;
			break;
		case 3:
			raceState = LAP3;
			break;
		case 4:
			raceState = WIN;
		default:
			break;
		}
	}
}

void ModuleSceneIntro::AddPlaneV(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, int angle, bool rotateX, bool rotateY, bool rotateZ, bool collider)
{
	PlaneV planeVToAdd(v1, v2, v3, v4);

	planeVToAdd.color = Cyan;

	// angle, XYZ
	if (rotateX == true) {
		planeVToAdd.SetRotation(angle, { 1, 0, 0 });
	}
	if (rotateY == true) {
		planeVToAdd.SetRotation(angle, { 0, 1, 0 });
	}
	if (rotateZ == true) {
		planeVToAdd.SetRotation(angle, { 0, 0, 1 });
	}

	//Aqui deberias llamar a la funcion AddBody para agregar el collider al plano y agregarlo al mundo de fisica 
	if (collider)
	{
	App->physics->AddBody(planeVToAdd, 0);
	}
	vectorPlaneV.push_back(planeVToAdd);
}

void ModuleSceneIntro::CreateRamps()
{
	glm::vec3 v1, v2, v3, v4;
	float inclinacion = 0.10f;

	v1 = glm::vec3(539 / 2, 196.0, 269 / 2);
	v2 = glm::vec3(539 / 2, 196.0, 275 / 2);
	v3 = glm::vec3(519 / 2, 196.0, 275 / 2);
	v4 = glm::vec3(519 / 2, 196.0, 269 / 2);
	v4.y += inclinacion * 10;
	v1.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(539 / 2, 196.0, 184 / 2);
	v2 = glm::vec3(539 / 2, 196.0, 189 / 2);
	v3 = glm::vec3(519 / 2, 196.0, 189 / 2);
	v4 = glm::vec3(519 / 2, 196.0, 184 / 2);
	v4.y += inclinacion * 10;
	v1.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(539 / 2, 196.0, 99 / 2);
	v2 = glm::vec3(539 / 2, 196.0, 105 / 2);
	v3 = glm::vec3(519 / 2, 196.0, 105 / 2);
	v4 = glm::vec3(519 / 2, 196.0, 99 / 2);
	v4.y += inclinacion * 10;
	v1.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);



	v1 = glm::vec3(796 / 2,196.0, 109 / 2);
	v2 = glm::vec3(796 / 2,196.0, 116 / 2);
	v3 = glm::vec3(745 / 2,196.0, 116 / 2);
	v4 = glm::vec3(745 / 2,196.0, 109 / 2);
	v2.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(780 / 2,196.0, 186 / 2);
	v2 = glm::vec3(780 / 2,196.0, 191 / 2);
	v3 = glm::vec3(755 / 2,196.0, 191 / 2);
	v4 = glm::vec3(755 / 2,196.0, 186 / 2);
	v2.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(766 / 2, 196.0, 275 / 2);
	v2 = glm::vec3(766 / 2, 196.0, 284 / 2);
	v3 = glm::vec3(746 / 2, 196.0, 284 / 2);
	v4 = glm::vec3(746 / 2, 196.0, 275 / 2);
	v2.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(815 / 2, 196.0, 339 / 2);
	v2 = glm::vec3(815 / 2, 196.0, 345 / 2);
	v3 = glm::vec3(741 / 2, 196.0, 345 / 2);
	v4 = glm::vec3(741 / 2, 196.0, 339 / 2);
	v2.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(764 / 2, 196.0, 407 / 2);
	v2 = glm::vec3(764 / 2, 196.0, 412 / 2);
	v3 = glm::vec3(718 / 2, 196.0, 412 / 2);
	v4 = glm::vec3(718 / 2, 196.0, 407 / 2);
	v2.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(819 / 2,196.0, 407 / 2);
	v2 = glm::vec3(819 / 2,196.0, 412 / 2);
	v3 = glm::vec3(775 / 2,196.0, 412 / 2);
	v4 = glm::vec3(775 / 2,196.0, 407 / 2);
	v2.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(776 / 2, 196.0, 469 / 2);
	v2 = glm::vec3(776 / 2, 196.0, 475 / 2);
	v3 = glm::vec3(734 / 2, 196.0, 475 / 2);
	v4 = glm::vec3(734 / 2, 196.0, 469 / 2);
	v2.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);



	v1 = glm::vec3(255 / 2, 196.0, 661 / 2);
	v2 = glm::vec3(255 / 2, 196.0, 648 / 2);
	v3 = glm::vec3(249 / 2, 196.0, 648 / 2);
	v4 = glm::vec3(249 / 2, 196.0, 661 / 2);
	v4.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(255 / 2,196.0, 689 / 2);
	v2 = glm::vec3(255 / 2,196.0, 678 / 2);
	v3 = glm::vec3(249 / 2,196.0, 678 / 2);
	v4 = glm::vec3(249 / 2,196.0, 689 / 2);
	v4.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(255 / 2,196.0, 689 / 2);
	v2 = glm::vec3(255 / 2,196.0, 678 / 2);
	v3 = glm::vec3(249 / 2,196.0, 678 / 2);
	v4 = glm::vec3(249 / 2,196.0, 689 / 2);
	v4.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(628 / 2, 196.0, 692 / 2);
	v2 = glm::vec3(628 / 2, 196.0, 674 / 2);
	v3 = glm::vec3(623 / 2, 196.0, 674 / 2);
	v4 = glm::vec3(623 / 2, 196.0, 692 / 2);
	v4.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(570 / 2, 196.0, 692 / 2);
	v2 = glm::vec3(570 / 2, 196.0, 674 / 2);
	v3 = glm::vec3(565 / 2, 196.0, 674 / 2);
	v4 = glm::vec3(565 / 2, 196.0, 692 / 2);
	v4.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(579 / 2, 196.0, 661 / 2);
	v2 = glm::vec3(579 / 2, 196.0, 642 / 2);
	v3 = glm::vec3(574 / 2, 196.0, 642 / 2);
	v4 = glm::vec3(574 / 2, 196.0, 661 / 2);
	v4.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(506 / 2, 196.0, 657 / 2);
	v2 = glm::vec3(506 / 2, 196.0, 642 / 2);
	v3 = glm::vec3(501 / 2, 196.0, 642 / 2);
	v4 = glm::vec3(501 / 2, 196.0, 657 / 2);
	v4.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(401 / 2, 196.0, 659 / 2);
	v2 = glm::vec3(401 / 2, 196.0, 640 / 2);
	v3 = glm::vec3(395 / 2, 196.0, 640 / 2);
	v4 = glm::vec3(395 / 2, 196.0, 659 / 2);
	v4.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(427 / 2, 196.0, 692 / 2);
	v2 = glm::vec3(427 / 2, 196.0, 679 / 2);
	v3 = glm::vec3(423 / 2, 196.0, 679 / 2);
	v4 = glm::vec3(423 / 2, 196.0, 692 / 2);
	v4.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(539 / 2,196.0, 692 / 2);
	v2 = glm::vec3(539 / 2,196.0, 682 / 2);
	v4 = glm::vec3(534 / 2,196.0, 692 / 2);
	v3 = glm::vec3(534 / 2,196.0, 682 / 2);
	v4.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);

	v1 = glm::vec3(475 / 2, 196.0, 682 / 2);
	v2 = glm::vec3(475 / 2, 196.0, 672 / 2);
	v4 = glm::vec3(470 / 2, 196.0, 682 / 2);
	v3 = glm::vec3(470 / 2, 196.0, 672 / 2);
	v4.y += inclinacion * 10;
	v3.y += inclinacion * 10;
	CreateRoad(v1, v2, v3, v4, White);


}


void ModuleSceneIntro::CreateSpiralRamp(float radius, float height, int sections, int size, float heigthX, float heigthY, float heigthZ)
{
	glm::vec3 prevV1, prevV2, prevV3, prevV4;
	glm::vec3 aV1, aV2, aV3, aV4;
	glm::vec3 center(0, 0, 0); // center of the spiral
	float angle = 0.0;
	float angleStep = 2 * PI / sections;
	float heightStep = height / sections;

	int sizePlane = size;
	float initialHeightX = 0;
	float initialHeightY = 0;
	float initialHeightZ = 0;


	for (int i = 0; i < sections; i++) {
		// calculate the position of each vertex
		float y = center.y + radius * cos(angle) + initialHeightY;
		float z = center.z + radius * sin(angle) + initialHeightZ;
		float x = center.x + i * heightStep + initialHeightX;
		glm::vec3 v1 = glm::vec3(x - size, y, z - size);
		glm::vec3 v2 = glm::vec3(x + size, y, z - size);
		glm::vec3 v3 = glm::vec3(x + size, y, z + size);
		glm::vec3 v4 = glm::vec3(x - size, y, z + size);

		//create the plan
		bool rotateX = true; //example rotate value
		bool rotateY = false; //example rotate value
		bool rotateZ = false; //example rotate value

		//add it to the scene
		//AddPlaneV(v1, v2, v3, v4, angle, rotateX, rotateY, rotateZ); 

		if (i > 0) {
			//connect to the previous section

			//v1 y v2
			aV1 = prevV1;
			aV2 = prevV2;
			aV3 = v2;
			aV4 = v1;
			AddPlaneV(aV1, aV2, aV3, aV4, 0, 0, 0, 0);

			//v2 y v3
			aV1 = prevV2;
			aV2 = prevV3;
			aV3 = v3;
			aV4 = v2;
			AddPlaneV(aV1, aV2, aV3, aV4, 0, 0, 0, 0);

			//v3 y v4
			aV1 = prevV3;
			aV2 = prevV4;
			aV3 = v4;
			aV4 = v3;
			AddPlaneV(aV1, aV2, aV3, aV4, 0, 0, 0, 0);

			//v4 y v1
			aV1 = prevV4;
			aV2 = prevV1;
			aV3 = v1;
			aV4 = v4;
			AddPlaneV(aV1, aV2, aV3, aV4, 0, 0, 0, 0);
		}

		/* Ultimo vertice arriba del todo
		V1 X:125.001 Y: 196 Z: -42.1483
		V2 X:141.001 Y: 196 Z: -42.1483
		V3 X:141.001 Y: 196 Z: -26.1483
		V4 X:125.001 Y: 196 Z: -26.1483
		*/

		//update the angle for the next iteration
		angle += angleStep;
		//increment the radius for the next iteration
		radius += angleStep;

		initialHeightX += heigthX;
		initialHeightY += heigthY;
		initialHeightZ += heigthZ;

		prevV1 = v1;
		prevV2 = v2;
		prevV3 = v3;
		prevV4 = v4;
	}
}

void ModuleSceneIntro::CreateMarioKartMap()
{
	float startX, startZ, endX, endZ;
	Map2dToMap3d(2, 57, 160, 66);
	Map2dToMap3d(2, 122, 53, 359);
	Map2dToMap3d(2, 481, 38, 130);
	GetWallCoordinates(2, 481, 38, 130, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	GetWallCoordinates(2 + 53, 122, 113 - (53 + 2), 132, "top", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	GetWallCoordinates(2 + 38, 481, (53 + 3) - (2 + 38), 130, "top", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(2, 610, 55, 41);
	GetWallCoordinates(2, 610, 55, 41, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(2 + 38, 610, (2 + 55) - (2 + 38), 41, "top", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);


	Map2dToMap3d(2, 650, 230, 40);
	GetWallCoordinates(2, 650, 230, 40, "bottom", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(2 + 55, 650, (2 + 230) - (2 + 55), 40, "top", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	CreateWall(2, 57, 2, 690, 1);


	GetWallCoordinates(2, 57, 160, 66, "top", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(2, 57, 160, 66, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(2, 122, 53, 359, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(113, 122, 49, 272);
	GetWallCoordinates(113, 122, 49, 272, "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(113, 122, 49, 68, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(113, 223, 49, 171, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(113, 393, 57, 45);
	GetWallCoordinates(113, 393, 57, 45, "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(113, 393, 57, 45, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(113 + 51, 393, (113 + 57) - (113 + 51), 45, "top", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(113, 435, 186, 54);
	GetWallCoordinates(113, 435, 186, 54, "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(113, 435, 186, 54, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(113, 435, 186, 54, "bottom", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(113 + 57, 435, 229 - (115 + 54), 54, "top", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(229, 357, 98, 47);
	GetWallCoordinates(229, 357, 98, 48, "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(229, 358, 98, 47, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(229, 357, (278 - 228), 47, "top", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(229 + 72, 357, (229 + 98) - (229 + 70), 47, "bottom", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(229 + 98, 358, (278 + 109) - (229 + 98), 47, "top", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(229, 404, 72, 32);
	GetWallCoordinates(229, 404, 72, 32, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(229, 404, 72, 32, "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(278, 297, 109, 63);
	GetWallCoordinates(278, 297, 109, 63, "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(278, 297, 109, 63, "top", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(387, 297, 178, 48);
	GetWallCoordinates(387, 297, 181, 48, "bottom", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(385, 297, (489 - 384), 48, "top", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(387, 297 + 48, 178, (297 + 63) - (297 + 48), "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(489, 2, 307, 48);
	GetWallCoordinates(489, 2, 307, 48, "top", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(489, 2, 307, 48, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(489, 2, 307, 48, "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(489, 49, 33, 295);
	GetWallCoordinates(489, 49, 33, (296 - 47), "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(519, 49, 24, 26);
	Map2dToMap3d(519, 99, 20, 61);
	Map2dToMap3d(519, 184, 20, 59);
	Map2dToMap3d(519, 269, 20, 75);

	Map2dToMap3d(538, 49, 29, 295);
	GetWallCoordinates(538, 49, 29, 296, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(387, 297, 180, 48);

	Map2dToMap3d(745, 50, 51, 66);
	GetWallCoordinates(745, 50, 51, 66, "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(745, 50, 51, 66, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(721, 136, 93, 51);
	GetWallCoordinates(721, 136, 93, 51 + 91, "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(721, 136, 93, 51 + 156, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(721, 186, 34, 91);
	Map2dToMap3d(746, 233, 46, 44);
	Map2dToMap3d(780, 185, 34, 156);
	Map2dToMap3d(741, 291, 44, 50);
	GetWallCoordinates(741, 291, 44, 50, "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(718, 357, 46, 50);
	GetWallCoordinates(718, 357, 46, 50, "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(718, 357, 46, 50, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(775, 356, 43, 51);
	GetWallCoordinates(775, 356, 43, 51, "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(775, 356, 43, 51, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(734, 422, 84, 47);
	GetWallCoordinates(734, 422, 84, 47, "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(734, 422, 84, 47, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(776, 468, 42, 60);
	GetWallCoordinates(776, 468, 42, 60, "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(734, 488, 59, 216);
	GetWallCoordinates(734, 488, 59, (623 - 488), "left", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(734, 488 + ((468 + 60) - 488), 59, 216 - ((468 + 60) - 488), "right", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);
	GetWallCoordinates(734, 488, 59, 216, "bottom", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);

	Map2dToMap3d(655, 623, 79, 81);
	GetWallCoordinates(657, 623, 79, 81, "bottom", startX, startZ, endX, endZ);
	CreateWall(startX, startZ, endX, endZ, 1);


	Map2dToMap3d(134, 650, 97, 39);
	Map2dToMap3d(230, 661, 25, 17);
	Map2dToMap3d(249, 648, 68, 41);
	Map2dToMap3d(313, 617, 344, 24);
	Map2dToMap3d(313, 640, 38, 72);
	Map2dToMap3d(346, 639, 15, 41);
	Map2dToMap3d(346, 659, 78, 21);
	Map2dToMap3d(346, 691, 312, 21);
	Map2dToMap3d(395, 640, 28, 39);
	Map2dToMap3d(423, 655, 111, 16);
	Map2dToMap3d(423, 670, 12, 24);
	Map2dToMap3d(434, 681, 53, 11);
	Map2dToMap3d(470, 669, 64, 13);
	Map2dToMap3d(501, 635, 33, 26);
	Map2dToMap3d(534, 659, 16, 37);
	Map2dToMap3d(547, 659, 52, 18);
	Map2dToMap3d(565, 674, 36, 21);
	Map2dToMap3d(574, 640, 81, 19);
	Map2dToMap3d(598, 659, 59, 16);
	Map2dToMap3d(623, 674, 33, 18);

}

void ModuleSceneIntro::CreateSpiralRoad(float radius, float height, int sections, int size)
{
	glm::vec3 prevV1, prevV2, prevV3, prevV4;
	glm::vec3 aV1, aV2, aV3, aV4;
	glm::vec3 center(0, 0, 0); // center of the spiral
	float angle = 90.0; //0
	float angleStep = 3.7 * PI / sections; //4
	float heightStep = height / sections;

	int sizePlane = size;

	for (int i = 0; i < sections; i++) {
		// calculate the position of each vertex
		float x = center.x + radius * cos(angle);
		float z = center.z + radius * sin(angle);
		float y = center.y + i * heightStep;
		glm::vec3 v1 = glm::vec3(x - size, y, z - size);
		glm::vec3 v2 = glm::vec3(x + size, y, z - size);
		glm::vec3 v3 = glm::vec3(x + size, y, z + size);
		glm::vec3 v4 = glm::vec3(x - size, y, z + size);

		//create the plan
		bool rotateX = false; //example rotate value
		bool rotateY = true; //example rotate value
		bool rotateZ = false; //example rotate value

		//add it to the scene
		//AddPlaneV(v1, v2, v3, v4, angle, rotateX, rotateY, rotateZ); 

		if (i > 0) {
			//connect to the previous section
			
			//v1 y v2
			aV1 = prevV1;
			aV2 = prevV2;
			aV3 = v2;
			aV4 = v1;
			AddPlaneV(aV1, aV2, aV3, aV4, 0, 0, 0, 0);

			//v2 y v3
			aV1 = prevV2;
			aV2 = prevV3;
			aV3 = v3;
			aV4 = v2;
			AddPlaneV(aV1, aV2, aV3, aV4, 0, 0, 0, 0);

			//v3 y v4
			aV1 = prevV3;
			aV2 = prevV4;
			aV3 = v4;
			aV4 = v3;
			AddPlaneV(aV1, aV2, aV3, aV4, 0, 0, 0, 0);

			//v4 y v1
			aV1 = prevV4;
			aV2 = prevV1;
			aV3 = v1;
			aV4 = v4;
			AddPlaneV(aV1, aV2, aV3, aV4, 0, 0, 0, 0);
		}

		/* Ultimo vertice arriba del todo
		V1 X:125.001 Y: 196 Z: -42.1483
		V2 X:141.001 Y: 196 Z: -42.1483
		V3 X:141.001 Y: 196 Z: -26.1483
		V4 X:125.001 Y: 196 Z: -26.1483
		*/

		//update the angle for the next iteration
		angle += angleStep;
		//increment the radius for the next iteration
		radius += angleStep;

		prevV1 = v1;
		prevV2 = v2;
		prevV3 = v3; 
		prevV4 = v4;
	}

	//aV1 = glm::vec3(141.001, 196, -42.1483);
	//aV2 = glm::vec3(141.001, 196, -42.1483);
	//aV3 = glm::vec3(2, 160, 57);
	//aV4 = glm::vec3(59, 160, 57);
	//AddPlaneV(aV1, aV2, aV3, aV4, 0, 0, 0, 0);
	
	//aV1, aV2, aV3, aV4;
	//Map2dToMap3d(2, 57, 160, 66);


}

void ModuleSceneIntro::GraphMovement()
{

}

void ModuleSceneIntro::CreateRoad(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4, Color RGB)
{
	Road road(v1, v2, v3, v4);
	road.color = RGB;
	vectorRoad.push_back(road);
	App->physics->AddBody(road, 0.0f);
}

std::vector<glm::vec3> ModuleSceneIntro::Map2dToMap3d(float x, float y, float width, float height, float posY, float scale)
{
	std::vector<glm::vec3> vertices;
	vertices.push_back(glm::vec3(x/scale + width/scale, posY, y/scale + height/scale));
	vertices.push_back(glm::vec3(x/scale + width/scale, posY, y/scale));
	vertices.push_back(glm::vec3(x/scale, posY, y/scale));
	vertices.push_back(glm::vec3(x/scale, posY, y/scale + height/scale));
	
	CreateRoad(vertices[0], vertices[1], vertices[2], vertices[3], Brown);

	return vertices;
}

void ModuleSceneIntro::CreateWall(float startX, float startZ, float endX, float endZ, float cubeSize, float posY, float scale)
{

	// Calcular la cantidad de cubos necesarios
	float distance = sqrtf(powf(endX/scale - startX/ scale, 2) + powf(endZ/ scale - startZ/ scale, 2));
	int numCubes = distance / cubeSize;

	// Calcular el vector de direcci�n del muro
	vec3 direction = normalize(vec3(endX/ scale - startX/ scale, 0, endZ/ scale - startZ/ scale));

	// Crear cada cubo y agregarlo al vector de cubos
	for (int i = 0; i < numCubes; i++)
	{
		vec3 pos = vec3(startX/scale, posY, startZ/scale) + direction * (i * cubeSize);
		//std::cout << "Cubo numero: " << i << "posicionX: " << startX << "posicionZ: " << startZ << std::endl;
		AddCube(pos, vec3(cubeSize, 2, cubeSize), Green, 90, 0, 1, 0);
	}
}

void ModuleSceneIntro::GetWallCoordinates(float posX, float posZ, float width, float height, std::string side, float& startX, float& startZ, float& endX, float& endZ)
{
	if (side == "left")
	{
		startX = posX;
		startZ = posZ;
		endX = posX;
		endZ = posZ + height;
	}
	else if (side == "right")
	{
		startX = posX + width;
		startZ = posZ;
		endX = posX + width;
		endZ = posZ + height;
	}
	else if (side == "top")
	{
		startX = posX;
		startZ = posZ;
		endX = posX + width;
		endZ = posZ;
	}
	else if (side == "bottom")
	{
		startX = posX;
		startZ = posZ + height;
		endX = posX + width;
		endZ = posZ + height;
	}
}