#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Color.h"
#include "ModulePlayer.h"

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

	//vec3 position = { 0,0,0 };
	//AddCube(vec3{ 0,0,0 }, vec3{ 10000,1,10000 }, Green, 0, 0, 0, 0);

	/*glm::vec3 v1(-2, -2, 0);
	glm::vec3 v2(-10, 10, 0);
	glm::vec3 v3(10, 10, 0);
	glm::vec3 v4(10, -10, 0);

	AddPlaneV(v1, v2, v3, v4, 0, 0, 0, 0);*/

	//primitives.emplace_back(std::make_unique<PlaneV>(v1, v2, v3, v4));

	//CreateSpiralRoad(25, 75, 50);
	
	// ======================================================
	//						Test Code
	// ======================================================
	sensor = App->physics->AddBody(Cube(20, 10, 20), 0.0f);
	sensor->SetAsSensor(true);

	sensorCounter = 0;
	raceState = LAP1;
	numOfLaps = 0;

	// -----------------------------


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
	
	for(auto& planeV: vectorPlaneV)
	{
		planeV.Render();
	}

	//for (auto& primitive : primitives) {
	//	primitive->Render();
	//}

	//DEBUG LAP VARIABLES:
	printf("VARIABLES\n");
	printf("SensorCounter: %d\n", sensorCounter);
	printf("Race State %d\n", raceState);
	printf("### Num of Laps %d\n", numOfLaps);


	return UPDATE_CONTINUE;
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
			sensor->SetPos(30, 0, 0);
			sensorCounter++;
			break;
			// sensor en posicio 1
		case 1:
			sensor->SetPos(0, 0, 30);
			sensorCounter++;
			break;
			// sensor en posicio 2
		case 2:
			sensor->SetPos(-30, 0, 0);
			sensorCounter++;
			break;
			// sensor en posicio 3
		case 3:
			sensor->SetPos(0, 0, -30);
			sensorCounter++;
			break;
		default:
			break;
		}

		// si player passa l'ultim sensor, col·loca de nou el sensor a la meta i suma 1 lap
		if (sensorCounter > 3)
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

void ModuleSceneIntro::AddPlaneV(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, int angle, bool rotateX, bool rotateY, bool rotateZ)
{
	PlaneV planeVToAdd(v1, v2, v3, v4);

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
	App->physics->AddBody(planeVToAdd, 0);
	vectorPlaneV.push_back(planeVToAdd);
}


//void ModuleSceneIntro::CreateSpiralRoad(float radius, float height, int sections)
//{
//	glm::vec3 center(0, 0, 0); // center of the spiral
//	float angle = 0.0;
//	float angleStep = 2 * PI / sections;
//	float heightStep = height / sections;
//
//	for (int i = 0; i < sections; i++) {
//		// calculate the position of each vertex
//		float x = center.x + radius * cos(angle);
//		float z = center.z + radius * sin(angle);
//		float y = center.y + i * heightStep;
//		glm::vec3 v1 = glm::vec3(x - 1, y, z - 1);
//		glm::vec3 v2 = glm::vec3(x + 1, y, z - 1);
//		glm::vec3 v3 = glm::vec3(x + 1, y, z + 1);
//		glm::vec3 v4 = glm::vec3(x - 1, y, z + 1);
//
//		//create the plan
//		bool rotateX = false; //example rotate value
//		bool rotateY = true; //example rotate value
//		bool rotateZ = false; //example rotate value
//		//add it to the scene
//		AddPlaneV(v1, v2, v3, v4, angle, rotateX, rotateY, rotateZ);
//
//
//
//
//		//update the angle for the next iteration
//		angle += angleStep;
//		//increment the radius for the next iteration
//		radius += angleStep;
//	}
//}

void ModuleSceneIntro::CreateSpiralRoad(float radius, float height, int sections)
{
	float prev_x = 0, prev_y = 0, prev_z = 0, next_x = 0, next_y = 0, next_z = 0;
	glm::vec3 center(0, 0, 0); // center of the spiral
	float angle = 0.0;
	float angleStep = 2 * PI / sections;
	float heightStep = height / sections;

	for (int i = 0; i < sections; i++) {
		// calculate the position of each vertex
		float x = center.x + radius * cos(angle);
		float z = center.z + radius * sin(angle);
		float y = center.y + i * heightStep;
		glm::vec3 v1 = glm::vec3(x - 1, y, z - 1);
		glm::vec3 v2 = glm::vec3(x + 1, y, z - 1);
		glm::vec3 v3 = glm::vec3(x + 1, y, z + 1);
		glm::vec3 v4 = glm::vec3(x - 1, y, z + 1);

		//create the plan
		bool rotateX = false; //example rotate value
		bool rotateY = true; //example rotate value
		bool rotateZ = false; //example rotate value
		//add it to the scene
		AddPlaneV(v1, v2, v3, v4, angle, rotateX, rotateY, rotateZ);

		if (i > 0) {
			//connect to the previous section
			glm::vec3 v1 = glm::vec3(x - 1, y, z - 1);
			glm::vec3 v2 = glm::vec3(prev_x - 1, prev_y, prev_z + 1);
			glm::vec3 v3 = glm::vec3(prev_x + 1, prev_y, prev_z + 1);
			glm::vec3 v4 = glm::vec3(x + 1, y, z - 1);
			AddPlaneV(v1,v2,v3,v4,0,0,0,0);
		}
		if (i < sections - 1) {
			//connect to the next section
			glm::vec3 v1 = glm::vec3(x + 1, y, z + 1);
			glm::vec3 v2 = glm::vec3(next_x + 1, next_y, next_z - 1);
			glm::vec3 v3 = glm::vec3(next_x - 1, next_y, next_z - 1);
			glm::vec3 v4 = glm::vec3(x - 1, y, z + 1);
			AddPlaneV(v1, v2, v3, v4, 0, 0, 0, 0);
		}


		//update the angle for the next iteration
		angle += angleStep;
		//increment the radius for the next iteration
		radius += angleStep;

		prev_x = x;
		prev_y = y;
		prev_z = z;
		next_x = center.x + (radius + angleStep) * cos(angle + angleStep);
		next_y = y + heightStep;
		next_z = center.z + (radius + angleStep) * sin(angle + angleStep);
	}
}
