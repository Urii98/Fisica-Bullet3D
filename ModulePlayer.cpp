#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModulePhysics3D.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"
#include "ModuleSceneIntro.h"
#include <iostream>

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	physics = true;
	coeficiente = 1;
	VehicleInfo car;
	// Car properties ----------------------------------------
	// Seient
	car.seient_size.Set(2, 0.5f, 2);
	car.seient_offset.Set(0, 1.8f, -0.6f);

	// Alero
	car.alero_size.Set(4.0f, 0.25f, 1);
	car.alero_offset.Set(0, 2.2f, -2.75f);

	gravity= vec3(0.0f, -10.0f, 0.0f);

	// Car properties ----------------------------------------
	car.chassis_size.Set(3, 1.2f, 5);
	car.chassis_offset.Set(0, 1.5, 0);
	car.mass = 1500.0f;
	car.suspensionStiffness = 15.88f;
	car.suspensionCompression = 0.83f;
	car.suspensionDamping = 0.88f;
	car.maxSuspensionTravelCm = 1000.0f;
	car.frictionSlip = 50.5;
	car.maxSuspensionForce = 6000.0f;
	

	// Wheel properties ---------------------------------------
	float connection_height = 1.8f;
	float wheel_radius = 0.8f;
	float wheel_width = 0.9f;
	float suspensionRestLength = 1.2f;

	// Don't change anything below this line ------------------

	float half_width = car.chassis_size.x*0.8f;
	float half_length = car.chassis_size.z*0.5f;
	
	vec3 direction(0,-1,0);
	vec3 axis(-1,0,0);
	
	car.num_wheels = 4;
	car.wheels = new Wheel[4];

	// FRONT-LEFT ------------------------
	car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[0].direction = direction;
	car.wheels[0].axis = axis;
	car.wheels[0].suspensionRestLength = suspensionRestLength;
	car.wheels[0].radius = wheel_radius;
	car.wheels[0].width = wheel_width;
	car.wheels[0].front = true;
	car.wheels[0].drive = true;
	car.wheels[0].brake = false;
	car.wheels[0].steering = true;

	// FRONT-RIGHT ------------------------
	car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
	car.wheels[1].direction = direction;
	car.wheels[1].axis = axis;
	car.wheels[1].suspensionRestLength = suspensionRestLength;
	car.wheels[1].radius = wheel_radius;
	car.wheels[1].width = wheel_width;
	car.wheels[1].front = true;
	car.wheels[1].drive = true;
	car.wheels[1].brake = false;
	car.wheels[1].steering = true;

	// REAR-LEFT ------------------------
	car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[2].direction = direction;
	car.wheels[2].axis = axis;
	car.wheels[2].suspensionRestLength = suspensionRestLength;
	car.wheels[2].radius = wheel_radius;
	car.wheels[2].width = wheel_width;
	car.wheels[2].front = false;
	car.wheels[2].drive = false;
	car.wheels[2].brake = true;
	car.wheels[2].steering = false;

	// REAR-RIGHT ------------------------
	car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
	car.wheels[3].direction = direction;
	car.wheels[3].axis = axis;
	car.wheels[3].suspensionRestLength = suspensionRestLength;
	car.wheels[3].radius = wheel_radius;
	car.wheels[3].width = wheel_width;
	car.wheels[3].front = false;
	car.wheels[3].drive = false;
	car.wheels[3].brake = true;
	car.wheels[3].steering = false;


	vehicle = App->physics->AddVehicle(car);

	//vehicle pos at the beginning of the normal road
	vehicle->SetPos(14, 200, 142);

	//Vehicle pos at the beginnign of the spiral road
	//vehicle->SetPos(0, 2, 0);

	vehicle->collision_listeners.add(App->scene_intro);
	vehicle->collision_listeners.add(this);

	// Matrix where the vehicle spawns at the beginning of the game
	vehicle->GetTransform(&last_checkpoint_matrix);

	// These two lines below take the transformation matrix of the vehicle and rotates it
	// 180 degrees on the y-axis (0, 1, 0)
	last_checkpoint_matrix.rotate(180, { 0, 1, 0 });
	vehicle->SetTransform(&last_checkpoint_matrix);

	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;
	
	if (physics == true) {
		gravity = vec3(0.0f, -10.0f, 0.0f);

		bouyancy = vec3(0.0f, 5.0f, 0.0f);;
		App->physics->ForceDrag(vehicle, 10);

		dragForce = 0.5 * vehicle->GetKmh() * 1.2 * 5;
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && vehicle->GetKmh() > -30.0f)
	{
		acceleration = MAX_ACCELERATION;
	}

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if (turn < TURN_DEGREES)
			turn += TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if (turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && vehicle->GetKmh() < 30.0f)
	{
		acceleration = -MAX_ACCELERATION;
	}

	// Reset vehicle and TP to the last checkpoint
	if (App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		// ... TP, rotar i aturar el vehicle
		vehicle->vehicle->getRigidBody()->clearForces();
		vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0,0,0 });
		vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0,0,0 });

		vehicle->SetTransform(&last_checkpoint_matrix);
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		if (vehicle->GetKmh() > 0) {
			acceleration -= MAX_ACCELERATION * 5;
		}
		if (vehicle->GetKmh() < 0) {
			acceleration += MAX_ACCELERATION * 5;
		}
	}

	turboActive = false;
	if (turbo > 0 && (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)) {
		acceleration = MAX_ACCELERATION * 5;
		turbo--;
		turboActive = true;
	}
	else if (turbo < 100) {
		turbo++;
	}

	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT) {
		vehicle->info.mass += 1;
	}

	if (App->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT) {
		vehicle->info.mass -= 1;
	}


	if (App->input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT) {
		gravity += vec3(0.0f, -0.1f, 0.0f);

		App->physics->SetGravity(gravity);
	}

	if (App->input->GetKey(SDL_SCANCODE_4) == KEY_REPEAT) {
		
		gravity -= vec3 (0.0f, -0.1f, 0.0f);

		App->physics->SetGravity(gravity);
	}

	if (vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().y() < 0)
	{
		vehicle->vehicle->getRigidBody()->clearForces();
		vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0,0,0 });
		vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0,0,0 });

		vehicle->SetTransform(&last_checkpoint_matrix);
	}
	if (App->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) {

		physics = false;

		dragForce = 0;
		gravity = vec3(0.0f, -0.0f, 0.0f);
		bouyancy = vec3(0.0f, 0.0f, 0.0f);

	}

	

		if (App->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN) {

			physics = true;

		}

	
	App->physics->SetGravity(gravity);




	//if (App->input->GetKey(SDL_SCANCODE_DOWN) != KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_UP) != KEY_REPEAT) {
	//	brake = BRAKE_POWER / 100;

	//	if (vehicle->GetKmh() > 100) {
	//		brake = BRAKE_POWER / 20;
	//	}
	//}

	acceleration -= dragForce;

	vehicle->ApplyEngineForce(acceleration*coeficiente);
	vehicle->Turn(turn);
	vehicle->Brake(brake);
	
	vehicle->Render();


	//char title[80];
	//sprintf_s(title, "%.1f Km/h, Current Time: %d s, ### DO THREE LAPS IN LESS THAN 300s ###", vehicle->GetKmh(), 3);
	//App->window->SetTitle(title);
	if (App->scene_intro->raceState == App->scene_intro->RaceStateEnum::LOSE)
	{
		char title[200];
		sprintf_s(title, "   Current Time: %d s,   massa = %.1f       %.1f Km/h      Gravity= %.1f   dragForce= %.1f  Pyshics=%s ### YOU LOSE! Try Again :D ###",
			 (SDL_GetTicks() - App->scene_intro->raceTimer) / 1000, vehicle->info.mass, vehicle->GetKmh(), gravity.y, dragForce, physics ? "true":"false");
		App->window->SetTitle(title);
	}
	else if (App->scene_intro->raceState == App->scene_intro->RaceStateEnum::WIN)
	{
		char title[200];
		sprintf_s(title, "  Current Time: %d s,   massa = %.1f       %.1f Km/h      Gravity= %.1f   dragForce= %.1f  Pyshics=%s ### YOU WON, GOOD JOB ### Press K to restart", 
			 (SDL_GetTicks() - App->scene_intro->raceTimer) / 1000, vehicle->info.mass, vehicle->GetKmh(), gravity.y, dragForce,physics ? "true" : "false");
		App->window->SetTitle(title);
	}
	else
	{
		char title[200];
		sprintf_s(title, " Current Time: %d s,   massa = %.1f       %.1f Km/h      Gravity= %.1f   dragForce= %.1f  Pyshics=%s ### DO THREE LAPS IN LESS THAN 300s ###", 
			 (SDL_GetTicks() - App->scene_intro->raceTimer) / 1000, vehicle->info.mass, vehicle->GetKmh(), gravity.y, dragForce, physics ? "true" : "false");
		App->window->SetTitle(title);
	}

	if (App->scene_intro->restartTheGame)
	{
		vehicle->SetPos(14, 200, 142);
		vehicle->GetTransform(&last_checkpoint_matrix);
		last_checkpoint_matrix.rotate(180, { 0, 1, 0 });
		vehicle->SetTransform(&last_checkpoint_matrix);

		vehicle->vehicle->getRigidBody()->clearForces();
		vehicle->vehicle->getRigidBody()->setLinearVelocity({ 0,0,0 });
		vehicle->vehicle->getRigidBody()->setAngularVelocity({ 0,0,0 });

		App->scene_intro->restartTheGame = false;
	}
	 
	 
	gravity=vec3(0.0f, -10.f, 0.0f);
	coeficiente = 1;


	return UPDATE_CONTINUE;
}

void ModulePlayer::OnCollision(PhysBody3D* body1, PhysBody3D* body2)
{
	if (body2 == App->scene_intro->sensor)
	{
		switch (App->scene_intro->sensorCounter)
		{
			// linia de meta
		case 0:
			vehicle->GetTransform(&last_checkpoint_matrix);
			break;
			// sensor en posicio 1
		case 1:
			vehicle->GetTransform(&last_checkpoint_matrix);
			break;
			// sensor en posicio 2
		case 2:
			vehicle->GetTransform(&last_checkpoint_matrix);
			break;
			// sensor en posicio 3
		case 3:
			vehicle->GetTransform(&last_checkpoint_matrix);
			break;
		case 4:
			vehicle->GetTransform(&last_checkpoint_matrix);
			// sensor en posicio 3
			break;
		default:
			break;
		}
	}

	if (body2 == App->scene_intro->water_Sensor)
	{

		vehicle->Push(0, 1000, 0);
	}
	
}