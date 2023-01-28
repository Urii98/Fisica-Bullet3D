#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "PhysBody3D.h"
#include "Primitive.h"
#include <vector>
#include <memory>
#include <string>

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
	void AddCube(vec3 position, vec3 size, Color RGB, int angle, bool rotateX, bool rotateY, bool rotateZ);
	void AddSphere(vec3 position, float radius, Color RGB, int angle, bool rotateX, bool rotateY, bool rotateZ);
	void AddCylinder(vec3 position, float radius, float height, Color RGB, int angle, bool rotateX, bool rotateY, bool rotateZ);
	
	void AddPlaneV(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, int angle, bool rotateX, bool rotateY, bool rotateZ, bool collider=true);
	void CreateSpiralRoad(float radius, float height, int sections, int size);
	void CreateRoad(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, const glm::vec3& v4, Color RGB);
	std::vector<glm::vec3> Map2dToMap3d(float x, float y, float width, float height, float posY=196.0f, float scale=2.0f);

	void CreateMarioKartMap();
	void CreateRamps();
	void CreateWall(float startX, float startZ, float endX, float endZ, float cubeSize, float posY = 196.0f, float scale = 2.0f);
	void GetWallCoordinates(float posX, float posZ, float width, float height, std::string side, float& startX, float& startZ, float& endX, float& endZ);
	void GraphMovement();

	void CreateSpiralRamp(float radius, float height, int sections, int size, float heigthX, float heigthY, float heigthZ);

	void CreateTurbine();

	PhysBody3D* CreateCubePhysbody(Cube* cube, Module* Callback, TypeObject type = TypeObject::NONE, bool is_sensor = false, float mass = 0.00f);
	Cube* cubeCreation(vec3 position, vec3 size, Color rgb, float angle = 0, vec3 pivot = 0);

	compact_info SpinMachine(int posx, int posy, int posz, int sizex, int sizey, int sizez, float angle,Color color, float mass = 10000.0f);
	compact_info TorqueMove(int posx, int posy, int posz, float angle, Color color, float mass = 10000.0f);


public:
	// No se que es aixo jaja no ho toco no vull espatllar res :D
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

	enum RaceStateEnum
	{
		LAP1,
		LAP2,
		LAP3,
		WIN,
		LOSE
	};

	PhysBody3D* sensor;
	// Contador per saber a on ha d'estar el sensor
	int sensorCounter;
	RaceStateEnum raceState;
	int numOfLaps;
	Cube checkpointCube;

	PhysBody3D* ice_Sensor;
	Cube ice_Cube;

	PhysBody3D* mud_Sensor;
	Cube mud_Cube;

	PhysBody3D* pb_seient;
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

	std::vector<Cube*> spinTorqueMachine;
	std::vector<Cube> vectorCubes;
	std::vector<std::unique_ptr<Cube>> graphCubes;
	std::vector<PlaneV> vectorPlaneV;
	std::vector<Road> vectorRoad;

	int raceTimer;
	int endGameTimer;

	bool restartTheGame;

	float prueba;
	int scalarSize;


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
