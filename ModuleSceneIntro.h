#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include <vector>
#include <memory>
#include <string>

#define MAX_SNAKE 2

struct PhysBody3D;
struct PhysMotor3D;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);
	void AddCube(vec3 position, vec3 size, Color RGB, int angle, bool rotateX, bool rotateY, bool rotateZ);
	void AddSphere(vec3 position, float radius, Color RGB, int angle, bool rotateX, bool rotateY, bool rotateZ);
	void AddCylinder(vec3 position, float radius, float height, Color RGB, int angle, bool rotateX, bool rotateY, bool rotateZ);
	
	void AddPlaneV(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, int angle, bool rotateX, bool rotateY, bool rotateZ, bool collider=true);
	void CreateSpiralRoad(float radius, float height, int sections, int size);
	void CreateRoad(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4, Color RGB);
	std::vector<glm::vec3> Map2dToMap3d(float x, float y, float width, float height, float posY=0.0f, float scale=2.0f);

	void CreateMarioKartMap();
	void CreateWall(float startX, float startZ, float endX, float endZ, float cubeSize, float posY = 0.0f, float scale = 2.0f);
	void GetWallCoordinates(float posX, float posZ, float width, float height, std::string side, float& startX, float& startZ, float& endX, float& endZ);
	void GraphMovement();

	void CreateSpiralRoadX(float radius, float height, int sections, int size, float heigthX, float heigthY, float heigthZ);

public:
	// No se que es aixo jaja no ho toco no vull espatllar res :D
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	PhysBody3D* sensor;


	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;

	std::vector<Cube> vectorCubes;
	std::vector<std::unique_ptr<Cube>> graphCubes;
	std::vector<PlaneV> vectorPlaneV;
	std::vector<Road> vectorRoad;

	float prueba;
	int scalarSize;
};
