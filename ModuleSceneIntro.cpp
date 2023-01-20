#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "Color.h"
#include <iostream>
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

	//glm::vec3 v1(-2, -2, 0);
	//glm::vec3 v2(-10, 10, 0);
	//glm::vec3 v3(10, 10, 0);
	//glm::vec3 v4(10, -10, 0);

	//AddPlaneV(v1, v2, v3, v4, 0, 0, 0, 0);
	//primitives.emplace_back(std::make_unique<PlaneV>(v1, v2, v3, v4));

	int numCubes = 30;
	float step = 2.0f / numCubes;
	vec3 scale(20, 20, 20);
	scalarSize = 7.5f;
	scale *= step;
	vec3 position(0, 0, 0);

	for (int i = 0; i < numCubes; i++)
	{
		graphCubes.emplace_back(std::make_unique<Cube>(scale.x, scale.y, scale.z));
		
		float posX = ((i + 1.0f)*step - 1.0f) * scalarSize;
		float posY = posX * posX;

		graphCubes.at(i).get()->SetPos(posX, posY, 10);

		float rate = 8.0 / numCubes;
		float aux = i * rate;
		
		graphCubes.at(i).get()->color = GetColor(aux);

		
	}
	

	CreateSpiralRoad(125, 200, 15, 8);
	
	// ======================================================
	//						Test Code
	// ======================================================
	sensor = App->physics->AddBody(Cube(20, 10, 20), 0.0f);
	sensor->SetAsSensor(true);

	// -----------------------------


	btCollisionShape* colShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);

	btDefaultMotionState* myMotionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, myMotionState, colShape);

	btRigidBody* body = new btRigidBody(rbInfo);
	

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
	prueba += dt;
	
	for (auto& graph : graphCubes)
	{
		float d = abs(graph.get()->GetX());

		float posY = sin(M_PI * (graph.get()->GetX()/ scalarSize) + prueba*2)*20;
		
	/*	float posY = sin(M_PI*(4.0f * d - prueba)) * 20;
		posY /= (1.0f + 10.0f * d);*/
		
		graph.get()->SetY(posY);

		graph->Render();
	}

	//for (auto& cube : vectorCubes)
	//{
	//	cube.Render();
	//}

	//for (auto& primitive : primitives) {
	//	primitive->Render();
	//}

	
	//Render sensor


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
}

void ModuleSceneIntro::AddPlaneV(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, int angle, bool rotateX, bool rotateY, bool rotateZ, bool collider)
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
	if (collider)
	{
	App->physics->AddBody(planeVToAdd, 0);
	}
	vectorPlaneV.push_back(planeVToAdd);
}


void ModuleSceneIntro::CreateSpiralRoad(float radius, float height, int sections, int size)
{
	glm::vec3 prevV1, prevV2, prevV3, prevV4;
	glm::vec3 aV1, aV2, aV3, aV4;
	glm::vec3 center(0, 0, 0); // center of the spiral
	float angle = 0.0;
	float angleStep = 4 * PI / sections;
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

		//update the angle for the next iteration
		angle += angleStep;
		//increment the radius for the next iteration
		radius += angleStep;

		prevV1 = v1;
		prevV2 = v2;
		prevV3 = v3; 
		prevV4 = v4;
	}
}

void ModuleSceneIntro::GraphMovement()
{

}