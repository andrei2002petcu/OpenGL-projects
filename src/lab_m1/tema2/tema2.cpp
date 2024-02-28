#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;
using namespace m1;

tema2::tema2()
{
}


tema2::~tema2()
{
}

// function to generate random numbers in given range
int randomInts(int min, int max)
{
    return min + rand() % (max - min + 1);
}

MovementState GetNextMovementState(const MovementState currentState)
{
    int randomChange = randomInts(0, 1);
    MovementState nextState = currentState;

    switch (currentState)
    {
    case MovementState::GoingForward:
    case MovementState::GoingBackward:
        nextState = (randomChange == 1)
            ? MovementState::InPlaceRotationLeft
            : MovementState::InPlaceRotationRight;
        break;

    case MovementState::InPlaceRotationLeft:
    case MovementState::InPlaceRotationRight:
        nextState = (randomChange == 1)
            ? MovementState::GoingForward
            : MovementState::GoingBackward;
        break;

    default:
        break;
    }

    return nextState;
}


std::string GetMovementStateName(const MovementState state)
{
    static const std::map<MovementState, std::string> kStateNames
    {
        { MovementState::GoingForward, "GoingForward" },
        { MovementState::GoingBackward, "GoingBackward" },
        { MovementState::InPlaceRotationLeft, "InPlaceRotationLeft" },
        { MovementState::InPlaceRotationRight, "InPlaceRotationRight" },
		{ MovementState::Stop, "Stop" },
        { MovementState::Shoot, "Shoot"},
    };

    std::string s = "";

    if (kStateNames.find(state) != kStateNames.end())
    {
        s = kStateNames.at(state);
    }

    return s;
}


// function to generate a building
building generateBuilding() {
	
    int scaleX = randomInts(10, 40);
    int scaleY = randomInts(20, 50);
    int scaleZ = randomInts(10, 40);

    int posX = randomInts(-100, 100);
    int posY = scaleY / 2;
    int posZ = randomInts(-100, 100);

    if (abs(posX) < 20 && abs(posZ) < 20) {
        posX += 45;
        posZ += 45;
    }
   

    glm::vec3 position = glm::vec3(posX, posY, posZ);
    glm::vec3 scale = glm::vec3(scaleX, scaleY, scaleZ);
    
    return building(position, scale);
}

// function to generate an enemy
enemy generateEnemy(bool to_fall) {
	int posX = randomInts(-100, 100);
	int posY = 0;
    if (to_fall) {
		posY = 50;
	}
	int posZ = randomInts(-100, 100);

    if (abs(posX) < 10 && abs(posZ) < 10) {
		posX += 20;
		posZ += 20;
	}
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(posX, posY, posZ));

	return enemy(modelMatrix, to_fall);
}

// function to check is tank is colliding with a building
glm::vec3 isCollidingBuildingTank(vector <building> buildings, glm::mat4 tank_modelMatrix, float deltaTimeSeconds, float tankSpeed) {
	glm::vec3 tank_position = glm::vec3(tank_modelMatrix[3]);
    
    for (int i = 0; i < buildings.size(); i++) {
		glm::vec3 building_position = buildings[i].position;
        building_position.y = 0;
		glm::vec3 building_scale = buildings[i].scale;

        float x1 = building_position.x - building_scale.x / 2 - 3;
        float x2 = building_position.x + building_scale.x / 2 + 3;
        float z1 = building_position.z - building_scale.z / 2 - 3;
        float z2 = building_position.z + building_scale.z / 2 + 3;

        if (tank_position.x >= x1 && tank_position.x <= x2 && tank_position.z >= z1 && tank_position.z <= z2) {
            glm::vec3 dif = glm::normalize(tank_position - building_position);
            float P = 3 * deltaTimeSeconds * tankSpeed;
            return dif * P;
        }
	}
	return glm::vec3(-1);
}

// check if tank crosses the map limits
glm::vec3 isCollidingMap(glm::mat4 tank_modelMatrix, float deltaTimeSeconds, float tankSpeed) {
    glm::vec3 tank_position = glm::vec3(tank_modelMatrix[3]);

    if (abs(tank_position.x) >= 120 || abs(tank_position.z) >= 120) {
        glm::vec3 dif = glm::normalize(tank_position);
        float P = 3 * deltaTimeSeconds * tankSpeed;
        return dif * P;
    }
    return glm::vec3(-1);
}

// function to check if a projectile is colliding with a building
bool isCollidingBuildingProjectile(vector <building> buildings, glm::mat4 projectile_modelMatrix) {
	glm::vec3 projectile_position = glm::vec3(projectile_modelMatrix[3]);

    for (int i = 0; i < buildings.size(); i++) {
		glm::vec3 building_position = buildings[i].position;
		glm::vec3 building_scale = buildings[i].scale;

		float x1 = building_position.x - building_scale.x / 2;
		float x2 = building_position.x + building_scale.x / 2;
		float z1 = building_position.z - building_scale.z / 2;
		float z2 = building_position.z + building_scale.z / 2;

        if (projectile_position.x >= x1 && projectile_position.x <= x2 && projectile_position.z >= z1 && projectile_position.z <= z2) {
			return true;
		}
	}
	return false;
}

// function to check if a projectile is colliding with an enemy (returns the index of the enemy if it is colliding, -1 otherwise)
int isCollidingProjectileEnemy(vector <enemy> enemies, glm::mat4 projectile_modelMatrix) {
    	glm::vec3 projectile_position = glm::vec3(projectile_modelMatrix[3]);

        for (int i = 0; i < enemies.size(); i++) {
		    glm::vec3 enemy_position = glm::vec3(enemies[i].modelMatrix[3]);
		    double distance = sqrt(pow(projectile_position.x - enemy_position.x, 2) + pow(projectile_position.z - enemy_position.z, 2));
            
            if (distance < 1.5) {
			    return i;
		    }
	    }
	return -1;
}

// function to check if a projectile is colliding with the tank (returns true if it is colliding, false otherwise)
bool isCollidingProjectileTank(glm::mat4 projectile_modelMatrix, glm::mat4 tank_modelMatrix) {
    glm::vec3 projectile_position = glm::vec3(projectile_modelMatrix[3]);
    glm::vec3 tank_position = glm::vec3(tank_modelMatrix[3]);

    double distance = sqrt(pow(projectile_position.x - tank_position.x, 2) + pow(projectile_position.z - tank_position.z, 2));
    if (distance < 1.5) {
        return true;
    }
    return false;
}

 // function to check if a tank is colliding with an enemy (returns the distance between the tanks)
glm::vec3 isCollidingTankEnemy(glm::mat4 tank_modelMatrix, glm::mat4 enemy_modelMatrix) {
	glm::vec3 tank_position = glm::vec3(tank_modelMatrix[3]);
	glm::vec3 enemy_position = glm::vec3(enemy_modelMatrix[3]);

    glm::vec3 dif = glm::normalize(tank_position - enemy_position);
    float distance = glm::distance(tank_position, enemy_position);

    if (distance < 7) {
        float P = abs(0.5f * (7 - distance));
        return dif * P;
    }

	return glm::vec3(-1);
}

// function to change the color of an enemy when it is hit
void changeColorEnemy(vector <enemy>& enemies, int index) {
    enemies[index].health--;
    enemies[index].color.x -= 0.1f;
}

// function to change the color of the tank when it is hit
void changeColorTank(glm::vec3& tank_color) {
    tank_color.y -= 0.04f;
}

void tema2::Init()
{
    {
        camera = new implemented::Camera();
        camera->Set(glm::vec3(0, 5.5f, 7), glm::vec3(0, 3, 0), glm::vec3(0, 1, 0));
        camera->RotateThirdPerson_OX(RADIANS(10));
        camera_angle = 0;
    }

    {
        tank_modelMatrix = glm::mat4(1);
        turela_modelMatrix = glm::mat4(1);
        turela_angle = 0;
        tank_direction = glm::vec3(0, 0, -1);
        tank_angle = 0;
        shoot_timer = 1;
        tank_health = 4;
        tank_color = glm::vec3(0, 0.2, 0);
    }

    tankSpeed = 7;
    cameraSpeedRotation = 0.5f;
    time_to_play = 90;
    points = 0;
    time_to_spawn = (float)randomInts(7, 12);

    {
        lightPosition = glm::vec3(0, 100, -100);
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
    }

    //generate buildings
    for (int i = 0; i < 20; i++) {
		buildings.push_back(generateBuilding());
	}

    //generate enemies
    for (int i = 0; i < 5; i++) {
        enemies.push_back(generateEnemy(false));
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("corp");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models"), "corp.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("senile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models"), "senile.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("tun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models"), "tun.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("turela");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "my_models"), "turela.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("TemaShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "my_shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "my_shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    FOV = 65;
    projectionMatrix = glm::perspective(RADIANS(FOV), window->props.aspectRatio, 0.01f, 200.0f);
}


void tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.5f, 0.5f, 0.6f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
    window->CenterPointer();
    window->HidePointer();
}


void tema2::Update(float deltaTimeSeconds)
{
    // Render plane
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.00f, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(5));
    RenderSimpleMesh(meshes["plane"], shaders["TemaShader"], modelMatrix, glm::vec3(0, 0.6, 0.1), -1);

    // Render light source
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, lightPosition);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(50));
    RenderSimpleMesh(meshes["sphere"], shaders["TemaShader"], modelMatrix, glm::vec3(1, 0.8f, 0), -1);
    
    //check if tank is colliding with a building
    glm::vec3 P = isCollidingBuildingTank(buildings, tank_modelMatrix, deltaTimeSeconds, tankSpeed);
    if (P.x != -1) {
        glm::vec3 tank_position = glm::vec3(tank_modelMatrix[3]) + P;   
        tank_modelMatrix = glm::translate(glm::mat4(1), tank_position);
        tank_modelMatrix = glm::rotate(tank_modelMatrix, tank_angle, glm::vec3(0, 1, 0));

        camera->Set(tank_position + glm::vec3(0, 5.5, 7), tank_position + glm::vec3(0, 3, 0), glm::vec3(0, 1, 0));
        camera->RotateThirdPerson_OX(RADIANS(10));
        camera->RotateThirdPerson_OY(camera_angle);
    }

    //check if tank is colliding with an enemy
    for (int i = 0; i < enemies.size(); i++) {
        P = isCollidingTankEnemy(tank_modelMatrix, enemies[i].modelMatrix);
        if (P.x != -1) {
            glm::vec3 tank_position = glm::vec3(tank_modelMatrix[3]) + P;
            glm::vec3 enemy_position = glm::vec3(enemies[i].modelMatrix[3]) - P;
            
            tank_modelMatrix = glm::translate(glm::mat4(1), tank_position);
            tank_modelMatrix = glm::rotate(tank_modelMatrix, tank_angle, glm::vec3(0, 1, 0));
            enemies[i].modelMatrix = glm::translate(glm::mat4(1), enemy_position);
            enemies[i].modelMatrix = glm::rotate(enemies[i].modelMatrix, enemies[i].tank_angle, glm::vec3(0, 1, 0));
     
            camera->Set(tank_position + glm::vec3(0, 5.5, 7), tank_position + glm::vec3(0, 3, 0), glm::vec3(0, 1, 0));
            camera->RotateThirdPerson_OX(RADIANS(10));
            camera->RotateThirdPerson_OY(camera_angle);
		}
	}

    //check if tank is colliding with the map limits
    P = isCollidingMap(tank_modelMatrix, deltaTimeSeconds, tankSpeed);
    if (P.x != -1) {
		glm::vec3 tank_position = glm::vec3(tank_modelMatrix[3]) - P;
		tank_modelMatrix = glm::translate(glm::mat4(1), tank_position);
		tank_modelMatrix = glm::rotate(tank_modelMatrix, tank_angle, glm::vec3(0, 1, 0));
		
        camera->Set(tank_position + glm::vec3(0, 5.5, 7), tank_position + glm::vec3(0, 3, 0), glm::vec3(0, 1, 0));
		camera->RotateThirdPerson_OX(RADIANS(10));
		camera->RotateThirdPerson_OY(camera_angle);
	}

    turela_modelMatrix = glm::rotate(tank_modelMatrix, turela_angle, glm::vec3(0, 1, 0));

    // render tank
    RenderSimpleMesh(meshes["corp"], shaders["TemaShader"], tank_modelMatrix, tank_color, tank_health);
    RenderSimpleMesh(meshes["turela"], shaders["TemaShader"], turela_modelMatrix, glm::vec3(0, 0.4, 0), -1);
    RenderSimpleMesh(meshes["tun"], shaders["TemaShader"], turela_modelMatrix, glm::vec3(0.5, 0.5, 0.5), -1);
    RenderSimpleMesh(meshes["senile"], shaders["TemaShader"], tank_modelMatrix, glm::vec3(0.5, 0.5, 0.5), -1);
    
    //update tank direction
    tank_direction = glm::normalize(glm::vec3(tank_modelMatrix[2]));

    //update tank shoot timer
    shoot_timer += deltaTimeSeconds;

    //render buildings
    for (int i = 0; i < buildings.size(); i++) {
        RenderSimpleMesh(meshes["box"], shaders["TemaShader"], buildings[i].modelMatrix, glm::vec3(0.2, 0.2, 0.2), -1);
    }

    // check if enemies are colliding with buildings
    for (int i = 0; i < enemies.size(); i++) {
        P = isCollidingBuildingTank(buildings, enemies[i].modelMatrix, deltaTimeSeconds, tankSpeed);
        if (P.x != -1) {
			glm::vec3 enemy_position = glm::vec3(enemies[i].modelMatrix[3]) + P;
			enemies[i].modelMatrix = glm::translate(glm::mat4(1), enemy_position);
			enemies[i].modelMatrix = glm::rotate(enemies[i].modelMatrix, enemies[i].tank_angle, glm::vec3(0, 1, 0));
		}
	}

    // check if enemies are colliding with other enemies
    for (int i = 0; i < enemies.size(); i++) {
        for (int j = i + 1; j < enemies.size(); j++) {
            P = isCollidingTankEnemy(enemies[i].modelMatrix, enemies[j].modelMatrix);
            if (P.x != -1) {
                glm::vec3 enemy1_position = glm::vec3(enemies[i].modelMatrix[3]) + P;
                glm::vec3 enemy2_position = glm::vec3(enemies[j].modelMatrix[3]) - P;

                enemies[i].modelMatrix = glm::translate(glm::mat4(1), enemy1_position);
                enemies[i].modelMatrix = glm::rotate(enemies[i].modelMatrix, enemies[i].tank_angle, glm::vec3(0, 1, 0));

                enemies[j].modelMatrix = glm::translate(glm::mat4(1), enemy2_position);
                enemies[j].modelMatrix = glm::rotate(enemies[j].modelMatrix, enemies[j].tank_angle, glm::vec3(0, 1, 0));
            }
		}
	}

    // check if enemies are colliding with the map limits
    for (int i = 0; i < enemies.size(); i++) {
        P = isCollidingMap(enemies[i].modelMatrix, deltaTimeSeconds, tankSpeed);
        if (P.x != -1) {
            glm::vec3 enemy_position = glm::vec3(enemies[i].modelMatrix[3]) - P;
            enemies[i].modelMatrix = glm::translate(glm::mat4(1), enemy_position);
            enemies[i].modelMatrix = glm::rotate(enemies[i].modelMatrix, enemies[i].tank_angle, glm::vec3(0, 1, 0));
        }
    }

    // check if enemies are in range for shooting
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i].shoot_timer += deltaTimeSeconds;
		glm::vec3 enemy_position = glm::vec3(enemies[i].modelMatrix[3]);
		glm::vec3 tank_position = glm::vec3(tank_modelMatrix[3]);
		float distance = glm::distance(enemy_position, tank_position);
        
        if (enemies[i].state == MovementState::Shoot) {
            enemies[i].state = MovementState::GoingForward;
            enemies[i].timer = (float)randomInts(1, 5);
        }
        if (distance < 40 && enemies[i].state != MovementState::Stop) {
			enemies[i].state = MovementState::Shoot;
		}
	}

    //update enemies
    for (int i = 0; i < enemies.size(); i++) {
		enemies[i].timer -= deltaTimeSeconds;
        if (enemies[i].timer <= 0 && enemies[i].state != MovementState::Shoot && enemies[i].state != MovementState::Stop) {
			enemies[i].state = GetNextMovementState(enemies[i].state);
			enemies[i].timer = (float)randomInts(1, 5);
		}

        switch (enemies[i].state)
        {
		case MovementState::GoingForward:
			enemies[i].modelMatrix = glm::translate(enemies[i].modelMatrix, glm::vec3(0, 0, -deltaTimeSeconds * tankSpeed));
			break;

		case MovementState::GoingBackward:
			enemies[i].modelMatrix = glm::translate(enemies[i].modelMatrix, glm::vec3(0, 0, deltaTimeSeconds * tankSpeed));
			break;

		case MovementState::InPlaceRotationLeft:
			enemies[i].modelMatrix = glm::rotate(enemies[i].modelMatrix, deltaTimeSeconds * cameraSpeedRotation, glm::vec3(0, 1, 0));
            enemies[i].tank_angle += deltaTimeSeconds * cameraSpeedRotation;
			break;

		case MovementState::InPlaceRotationRight:
			enemies[i].modelMatrix = glm::rotate(enemies[i].modelMatrix, -deltaTimeSeconds * cameraSpeedRotation, glm::vec3(0, 1, 0));
            enemies[i].tank_angle += -deltaTimeSeconds * cameraSpeedRotation;
			break;

		case MovementState::Shoot:
            if (enemies[i].shoot_timer >= 1.0) {
                glm::vec3 enemy_position = glm::vec3(enemies[i].modelMatrix[3]);
                glm::vec3 tank_position = glm::vec3(tank_modelMatrix[3]);

                modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(enemies[i].turela_modelMatrix, glm::vec3(0, 2.25f, -4.5f));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
				projectiles.push_back(projectile(modelMatrix));
				enemies[i].shoot_timer = 0;
			}
			break;

		default:
			break;
		}
	}

    //render enemies
    for (int i = 0; i < enemies.size(); i++) {
        glm::vec3 enemy_position = glm::vec3(enemies[i].modelMatrix[3]);
        glm::vec3 tank_position = glm::vec3(tank_modelMatrix[3]);
        if (enemies[i].state == MovementState::Shoot) {
            enemies[i].turela_angle = atan2(enemy_position.x - tank_position.x, enemy_position.z - tank_position.z) - enemies[i].tank_angle;
        }

        if (enemies[i].to_fall) {
			enemies[i].modelMatrix = glm::translate(enemies[i].modelMatrix, glm::vec3(0, -deltaTimeSeconds * 30.0, 0));
            enemy_position = glm::vec3(enemies[i].modelMatrix[3]);  
            if (enemy_position.y <= 0) {
				enemies[i].to_fall = false;
                enemies[i].modelMatrix[3][1] = 0;
			}
		}

		enemies[i].turela_modelMatrix = glm::rotate(enemies[i].modelMatrix, enemies[i].turela_angle, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["corp"], shaders["TemaShader"], enemies[i].modelMatrix, enemies[i].color, enemies[i].health);
		RenderSimpleMesh(meshes["turela"], shaders["TemaShader"], enemies[i].turela_modelMatrix, glm::vec3(0.4, 0.4, 0.4), -1);
		RenderSimpleMesh(meshes["tun"], shaders["TemaShader"], enemies[i].turela_modelMatrix, glm::vec3(0.5, 0.5, 0.5), -1);
		RenderSimpleMesh(meshes["senile"], shaders["TemaShader"], enemies[i].modelMatrix, glm::vec3(0.5, 0.5, 0.5), -1);
	}

    //render projectiles
    for (int i = 0; i < projectiles.size(); i++) {
        projectiles[i].time_to_live -= deltaTimeSeconds;

        int enemy_index = isCollidingProjectileEnemy(enemies, projectiles[i].modelMatrix);
        if (enemy_index != -1) {
            projectiles[i].time_to_live = 0;
            changeColorEnemy(enemies, enemy_index);
            if (enemies[enemy_index].health <= 0) {
                enemies.erase(enemies.begin() + enemy_index);
                points += 100;
            }
            else if (enemies[enemy_index].health == 1) {
                enemies[enemy_index].state = MovementState::Stop;
                enemies[enemy_index].timer = 100000;
            }
        }

        if (isCollidingProjectileTank(projectiles[i].modelMatrix, tank_modelMatrix)) {
			projectiles[i].time_to_live = 0;
			tank_health--;
			changeColorTank(tank_color);
		}

        if (projectiles[i].time_to_live <= 0 || isCollidingBuildingProjectile(buildings, projectiles[i].modelMatrix)) {
            projectiles.erase(projectiles.begin() + i);
            i--;
            continue;
        }
        projectiles[i].modelMatrix = glm::translate(projectiles[i].modelMatrix, glm::vec3(0, 0, -deltaTimeSeconds * 100.0));
        RenderSimpleMesh(meshes["sphere"], shaders["TemaShader"], projectiles[i].modelMatrix, glm::vec3(0, 0, 0), -1);
    }

    time_to_spawn -= deltaTimeSeconds;
    if (time_to_spawn <= 0) {
		enemies.push_back(generateEnemy(true));
        time_to_spawn = (float)randomInts(5, 10);
	}

    time_to_play -= deltaTimeSeconds;
    if (time_to_play <= 0) {
		cout << "GAME OVER! TIME'S UP!" << endl;
		cout << "Points: " << points << endl;
        window->ShowPointer();
        while (true) {}
	}

    if (tank_health <= 0) {
        cout << "GAME OVER!" << endl;
        cout << "WASTED!" << endl;
        window->ShowPointer();
        while (true) {}
    }
}


void tema2::FrameEnd()
{
}


void tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, const int& health)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set light position uniform
    int location = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(location, 1, glm::value_ptr(lightPosition));

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = camera->position;
    location = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(location, 1, glm::value_ptr(eyePosition));

    // Set material property uniforms (shininess, kd, ks, object color)
    location = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(location, materialShininess);

    location = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(location, materialKd);

    location = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(location, materialKs);

    // // Set shader uniform "Model" to modelMatrix
    location = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Set shader uniform "View" to viewMatrix
    location = glGetUniformLocation(shader->program, "View");
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Set shader uniform "Projection" to projectionMatrix
    location = glGetUniformLocation(shader->program, "Projection");
    glm::mat4 projectionMatrixLocal = projectionMatrix;
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrixLocal));

    // Get shader location for uniform vec3 "object_color"
    location = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(location, 1, glm::value_ptr(color));

    // Set shader uniform "health"
    location = glGetUniformLocation(shader->program, "health");
    glUniform1i(location, health);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


void tema2::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_W)) {
        camera->MoveForward(-deltaTime * tankSpeed, tank_direction);
        tank_modelMatrix = glm::translate(tank_modelMatrix, glm::vec3(0, 0, -deltaTime * tankSpeed));
    }
    if (window->KeyHold(GLFW_KEY_S)) {
		camera->MoveForward(deltaTime * tankSpeed, tank_direction);
		tank_modelMatrix = glm::translate(tank_modelMatrix, glm::vec3(0, 0, deltaTime * tankSpeed));
	}
    if (window->KeyHold(GLFW_KEY_A)) {
        tank_modelMatrix = glm::rotate(tank_modelMatrix, deltaTime * cameraSpeedRotation, glm::vec3(0, 1, 0));
        tank_angle += deltaTime * cameraSpeedRotation;
        camera->RotateThirdPerson_OY(deltaTime * cameraSpeedRotation);
        camera_angle += deltaTime * cameraSpeedRotation;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        tank_modelMatrix = glm::rotate(tank_modelMatrix, -deltaTime * cameraSpeedRotation, glm::vec3(0, 1, 0));
        tank_angle += -deltaTime * cameraSpeedRotation;
        camera->RotateThirdPerson_OY(-deltaTime * cameraSpeedRotation);
        camera_angle += -deltaTime * cameraSpeedRotation;
    }
}


void tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    float sensivityOY = 0.0025f;

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {   
        camera->RotateThirdPerson_OY(-deltaX * sensivityOY);
        camera_angle += -deltaX * sensivityOY;
    }
    else if (abs(deltaX) < 100) {
        turela_angle += -deltaX * sensivityOY;
    }
}


void tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == GLFW_MOUSE_BUTTON_2 && shoot_timer >= 1.0) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(turela_modelMatrix, glm::vec3(0, 2.25f, -4.5f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f));
		projectiles.push_back(projectile(modelMatrix));
		shoot_timer = 0;
	}
}


void tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void tema2::OnWindowResize(int width, int height)
{
}
