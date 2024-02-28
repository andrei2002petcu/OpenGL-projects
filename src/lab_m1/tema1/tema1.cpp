#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


tema1::tema1()
{
}


tema1::~tema1()
{
}

// function to generate random numbers in given range
int randomInt(int min, int max)
{
	return min + rand() % (max - min + 1);
}

// function to add 3 new resources at random positions in the vector
void generateResources(vector<Resource> &resources) {
	float x, y;
    for (int i = 0; i < 3; i++) {
		x = (float)randomInt(0,  1280);
		y = (float)randomInt(0, 720);
		resources.push_back(Resource(x, y));
	}
}

//function to generate a random enemy on a random row
Enemy generateEnemy(float squareSide) {
	int row = randomInt(0, 2);
	int color = randomInt(1, 4);
    float y = 720 - ((squareSide / 2 + 25) + row * (squareSide + 25));
	return Enemy(y, color, row);
}

//function to convert color code to string for turrets
string colorToStringTurret(int color) {
    if (color == 1) {
		return "turret_o";
	}
    if (color == 2) {
		return "turret_b";
	}
    if (color == 3) {
		return "turret_y";
	}
    if (color == 4) {
		return "turret_p";
	}
    printf("ERROR: color turret %d\n", color);
	return "none";
}

// function to convert color code to string for enemies
string colorToStringEnemy(int color) {
    if (color == 1) {
		return "hexagon_o";
	}
    if (color == 2) {
		return "hexagon_b";
	}
    if (color == 3) {
		return "hexagon_y";
	}
    if (color == 4) {
		return "hexagon_p";
	}
    printf("ERROR: color hexagon %d\n", color);
	return "none";
}

 // function to convert color code to string for stars
string colorToStringStar(int color) {
    if (color == 1) {
		return "star_o";
	}
    if (color == 2) {
		return "star_b";
	}
    if (color == 3) {
		return "star_y";
	}
    if (color == 4) {
		return "star_p";
	}
    printf("ERROR: color star %d\n", color);
    return "none";
}

// function to check if a resource is clicked
bool checkResourceClick(int x, int y, Resource r) {
    if (x >= r.x - 40 && x <= r.x + 40 && y >= r.y - 40 && y <= r.y + 40) {
        return true;
    }
    return false;
}

// function to check if a turret is clicked
bool checkTurretClick(int x, int y, Turret t) {
    if (x >= t.x - 60 && x <= t.x + 60 && y >= t.y - 60 && y <= t.y + 60) {
		return true;
	}
	return false;
}

// function to check if turret is touched by enemy (collision using circles)
bool checkTurretTouch(Enemy e, Turret t) {
	float dx = e.x - t.x;
	float dy = e.y - t.y;
	float distance = sqrt(dx * dx + dy * dy);
    if (distance < e.r + t.r) {
		return true;
	}
	return false;
}

// function to check if there is an enemy with same color on the same row as the turret
// return true if the enemy is in front of the turret
bool checkTurretRow(Enemy e, Turret t) {
    if (e.row == t.row && e.color == t.color && e.x > t.x) {
		return true;
	}
	return false;
}

// function to check if there is a collision between a bullet and an enemy
bool checkBulletCollision(Bullet b, Enemy e) {
	float dx = b.x - e.x;
	float dy = b.y - e.y;
	float distance = sqrt(dx * dx + dy * dy);
    if (distance < b.r + e.r) {
		return true;
	}
	return false;
}


void tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 squareCenter = glm::vec3(0, 0, 0);
    squareSide = 120;

    // Initialize grid coordinates
    gridX = squareSide + 40;
    gridY = squareSide/2 + 25;

    //Initialize turret GUI coordinates
    turretX = squareSide / 2 + 20;
    turretY = 720 - squareSide / 2 - 20;

    // Initialize lives
    lives = 3;
    livesX = 800;
    livesY = 720 - squareSide / 2 - 20;

    // Initialize resources
    resources = 5;
    resourceTimer = 0;
    resourceTimerMax = 2;

    // Initialize GUI vector
    GUIturretVector.push_back(Turret(turretX, 720 - turretY, 1, 1, -1, true));
    GUIturretVector.push_back(Turret(turretX + squareSide + 25, 720 - turretY, 2, 2, -1, true));
    GUIturretVector.push_back(Turret(turretX + 2 * (squareSide + 25), 720 - turretY, 3, 2, -1, true));
    GUIturretVector.push_back(Turret(turretX + 3 * (squareSide + 25), 720 - turretY, 4, 3, -1, true));

    // Initialize turret vector
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            turretVector.push_back(Turret(gridX + i * (squareSide + 25), 720 - (gridY + j * (squareSide + 25)), 0, 0, j, false));
        }
    }

    // Intialize drag and drop parameters
    dragTurret = false;
    dragTurretColor = 1;
    dragTurretCost = 1;
    dragX = 0;
    dragY = 0;

    // Initialize enemies
    enemyTimer = 0;
    enemyTimerMax = 2;

    // Turret spaces
    Mesh* square = object2D::CreateSquare("square", squareCenter, squareSide, glm::vec3(0, 0.5f, 0), true);
    AddMeshToList(square);

    // Outline square for turret GUI
    Mesh* outline_sq = object2D::CreateSquare("outline_sq", squareCenter, squareSide, glm::vec3(1, 1, 1), false);
    AddMeshToList(outline_sq);

    // Squares for lives
    Mesh* lives_sq = object2D::CreateSquare("lives_sq", squareCenter, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(lives_sq);

    // Red rectangle for base
    Mesh* rectangle = object2D::CreateRectangle("rectangle", squareCenter, 3 * squareSide + 50, squareSide / 2, glm::vec3(1, 0, 0), true);
    AddMeshToList(rectangle);

    // Turrets
    Mesh* turret_b = object2D::CreateTurret("turret_b", squareCenter, squareSide - 20, glm::vec3(0, 0, 1));
    AddMeshToList(turret_b);
    Mesh* turret_p = object2D::CreateTurret("turret_p", squareCenter, squareSide - 20, glm::vec3(0.8f, 0, 1));
    AddMeshToList(turret_p);
    Mesh* turret_y = object2D::CreateTurret("turret_y", squareCenter, squareSide - 20, glm::vec3(1, 0.8f, 0));
    AddMeshToList(turret_y);
    Mesh* turret_o = object2D::CreateTurret("turret_o", squareCenter, squareSide - 20, glm::vec3(1, 0.4f, 0));
    AddMeshToList(turret_o);

    // Hexagons
    Mesh* hexagon_b = object2D::CreateHexagon("hexagon_b", squareCenter, squareSide - 20, glm::vec3(0, 0, 1), 0);
    AddMeshToList(hexagon_b);
    Mesh* hexagon_p = object2D::CreateHexagon("hexagon_p", squareCenter, squareSide - 20, glm::vec3(0.8f, 0, 1), 0);
    AddMeshToList(hexagon_p);
    Mesh* hexagon_y = object2D::CreateHexagon("hexagon_y", squareCenter, squareSide - 20, glm::vec3(1, 0.8f, 0), 0);
    AddMeshToList(hexagon_y);
    Mesh* hexagon_o = object2D::CreateHexagon("hexagon_o", squareCenter, squareSide - 20, glm::vec3(1, 0.4f, 0), 0);
    AddMeshToList(hexagon_o);
    Mesh* hexagon_in = object2D::CreateHexagon("hexagon_in", squareCenter, squareSide - 50, glm::vec3(0.7f, 0.7f, 0.7f), 1);
    AddMeshToList(hexagon_in);

    // Stars for projectiles
    Mesh* star_b = object2D::CreateStar("star_b", squareCenter, squareSide/2, glm::vec3(0, 0, 1), 2);
    AddMeshToList(star_b);
    Mesh* star_p = object2D::CreateStar("star_p", squareCenter, squareSide/2, glm::vec3(0.8f, 0, 1), 2);
    AddMeshToList(star_p);
    Mesh* star_y = object2D::CreateStar("star_y", squareCenter, squareSide/2, glm::vec3(1, 0.8f, 0), 2);
    AddMeshToList(star_y);
    Mesh* star_o = object2D::CreateStar("star_o", squareCenter, squareSide/2, glm::vec3(1, 0.4f, 0), 2);
    AddMeshToList(star_o);

    // Stars for resources
    Mesh* star = object2D::CreateStar("star", squareCenter, squareSide - 40, glm::vec3(0, 1, 1), 5);
    AddMeshToList(star);

    // Cost for turrets
    Mesh* costStar = object2D::CreateStar("costStar", squareCenter, squareSide / 4, glm::vec3(0.8f, 0.8f, 0.8f), 0);
    AddMeshToList(costStar);
}

void tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void tema1::Update(float deltaTimeSeconds)
{
    if (lives > 0)
    {
        //create a square mesh of 3x3 squares with space between them
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(gridX + i * (squareSide + 25),gridY + j * (squareSide + 25));
                RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
                if (turretVector[i * 3 + j].placed && !turretVector[i * 3 + j].toDelete) {
					RenderMesh2D(meshes[colorToStringTurret(turretVector[i * 3 + j].color)], shaders["VertexColor"], modelMatrix);
                    turretVector[i * 3 + j].timer += deltaTimeSeconds;
				}
                else if (turretVector[i * 3 + j].toDelete) {
                    turretVector[i * 3 + j].scale -= 1.5f * deltaTimeSeconds;
                    modelMatrix *= transform2D::Scale(turretVector[i * 3 + j].scale, turretVector[i * 3 + j].scale);
                    RenderMesh2D(meshes[colorToStringTurret(turretVector[i * 3 + j].color)], shaders["VertexColor"], modelMatrix);
                    if (turretVector[i * 3 + j].scale < 0) {
						turretVector[i * 3 + j].toDelete = false;
						turretVector[i * 3 + j].placed = false;
                        turretVector[i * 3 + j].scale = 1;
                        turretVector[i * 3 + j].color = 0;
                        turretVector[i * 3 + j].cost = 0;
                        turretVector[i * 3 + j].timer = 0;
					}
                }
            }
        }

        //create GUI for turrets
        for (int i = 0; i < 4; i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(turretX + i * (squareSide + 25), turretY);
            RenderMesh2D(meshes["outline_sq"], shaders["VertexColor"], modelMatrix);
            RenderMesh2D(meshes[colorToStringTurret(GUIturretVector[i].color)], shaders["VertexColor"], modelMatrix);
            for (int j = 0; j < GUIturretVector[i].cost; j++) {
                modelMatrix = glm::mat3(1);
                modelMatrix *= transform2D::Translate(turretX + i * (squareSide + 25) + j * (squareSide/4 + 5) - 50, turretY - squareSide / 2 - 25);
                RenderMesh2D(meshes["costStar"], shaders["VertexColor"], modelMatrix);
            }
        }

        //create GUI for lives and resources
        for (int i = 0; i < lives; i++) {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(livesX + i * (squareSide + 25), livesY);
            RenderMesh2D(meshes["lives_sq"], shaders["VertexColor"], modelMatrix);
        }
        for (int i = 0; i < resources; i++) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate(livesX + i * (squareSide / 4 + 5) - 50, livesY - squareSide / 2 - 25);
			RenderMesh2D(meshes["costStar"], shaders["VertexColor"], modelMatrix);
		}

        //red rectangle
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(10 + squareSide / 4, (3 * squareSide + 50) / 2 + 25);
        RenderMesh2D(meshes["rectangle"], shaders["VertexColor"], modelMatrix);

        //render the existing resources from vector
        for (Resource r : resourceVector) {
			modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(r.x, 720 - r.y);
            RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
		}

        //render the turret that is being dragged
        if (dragTurret) {
			modelMatrix = glm::mat3(1);
			modelMatrix *= transform2D::Translate(dragX, 720 - dragY);
			RenderMesh2D(meshes[colorToStringTurret(dragTurretColor)], shaders["VertexColor"], modelMatrix);
		}

        //check if enemy is on the same row as a turret and generate a bullet
        for (auto it = enemyVector.begin(); it != enemyVector.end(); it++) {
            for (auto it2 = turretVector.begin(); it2 != turretVector.end(); it2++) {
                if (it2->placed && checkTurretRow(*it, *it2) && !it2->toDelete) {
                    if (it2->timer > 1.5f) {
                        bulletVector.push_back(Bullet(it2->x, it->y, it2->color, it2->row));
                        it2->timer = 0;
                    }
                }
            }
        }

        //generate new enemy at random time
        enemyTimer += deltaTimeSeconds;
        if (enemyTimer > enemyTimerMax) {
            enemyVector.push_back(generateEnemy(squareSide));
            enemyTimer = 0;
            enemyTimerMax = randomInt(2, 6);
        }

        //gemerate new resource at random time
        resourceTimer += deltaTimeSeconds;
        if (resourceTimer > resourceTimerMax && resources < 12 && resourceVector.size() < 8) {
            generateResources(resourceVector);
            resourceTimer = 0;
            resourceTimerMax = randomInt(5, 12);
        }

        //render enemies
        for (auto it = enemyVector.begin(); it != enemyVector.end(); it++) {
			
            if (!it->toDelete) {
                modelMatrix = glm::mat3(1);
                it->x -= (100 * deltaTimeSeconds);
                modelMatrix *= transform2D::Translate(it->x, gridY + it->row * (squareSide + 25));
                RenderMesh2D(meshes[colorToStringEnemy(it->color)], shaders["VertexColor"], modelMatrix);
                RenderMesh2D(meshes["hexagon_in"], shaders["VertexColor"], modelMatrix);
            }
            else {
                modelMatrix = glm::mat3(1);
				it->scale -= 1.5f * deltaTimeSeconds;
                modelMatrix *= transform2D::Translate(it->x, gridY + it->row * (squareSide + 25));
				modelMatrix *= transform2D::Scale(it->scale, it->scale);
				RenderMesh2D(meshes[colorToStringEnemy(it->color)], shaders["VertexColor"], modelMatrix);
				RenderMesh2D(meshes["hexagon_in"], shaders["VertexColor"], modelMatrix);
			}

            //check if enemy touched a turret
            for (auto it2 = turretVector.begin(); it2 != turretVector.end(); it2++)
                if (it2->placed && checkTurretTouch(*it, *it2))
					it2->toDelete = true;
        }

        //render bullets
        for (auto it = bulletVector.begin(); it != bulletVector.end(); it++) {
            modelMatrix = glm::mat3(1);
            it->x += (300 * deltaTimeSeconds);
            it->angularStep -= 4 * deltaTimeSeconds;
            modelMatrix *= transform2D::Translate(it->x, gridY + it->row * (squareSide + 25));
            modelMatrix *= transform2D::Rotate(it->angularStep);
            RenderMesh2D(meshes[colorToStringStar(it->color)], shaders["VertexColor"], modelMatrix);
        }

        //check if bullet touched an enemy
        for (auto it = bulletVector.begin(); it != bulletVector.end(); it++) {
            int toDelete = 0;
            for (auto it2 = enemyVector.begin(); it2 != enemyVector.end(); it2++) {
                if (it->color == it2->color && checkBulletCollision(*it, *it2)) {
					it2->health--;
                    toDelete = 1;
                    break;
				}
			}
            if (toDelete) {
				it = bulletVector.erase(it);
				break;
			}
		}
        
        //check if enemy has 0 health
        for (auto it = enemyVector.begin(); it != enemyVector.end(); it++) {
            if (it->health <= 0) {
                it->toDelete = true;
                if (it->scale < 0) {
                    it = enemyVector.erase(it);
                    break;
                }
            }
        }

        //check if enemy reached base
        for (auto it = enemyVector.begin(); it != enemyVector.end(); it++) {
            if (it->x < 10 + squareSide / 4) {
                it = enemyVector.erase(it);
                lives--;
                break;
            }
        }
    }
}


void tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    //decrement lives
    if (key == GLFW_KEY_SPACE) {
		lives--;
	}
}


void tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    if (dragTurret) {
        dragX = (float)mouseX;
        dragY = (float)mouseY;
    }
}


void tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event

    if (button == GLFW_MOUSE_BUTTON_2) {
        
        //check if a resource is clicked
        for (auto it = resourceVector.begin(); it != resourceVector.end(); it++) {
            if (checkResourceClick(mouseX, mouseY, *it)) {
				it = resourceVector.erase(it);
				resources++;
                break;
			}
		}

        //check if a turret is clicked on GUI
        for (auto it = GUIturretVector.begin(); it != GUIturretVector.end(); it++) {
            if (checkTurretClick(mouseX, mouseY, *it) && it->cost <= resources) {
                dragTurret = true;
                dragTurretColor = it->color;
                dragTurretCost = it->cost;
                dragX = (float)mouseX;
                dragY = (float)mouseY;
                break;
            }
        }
    }

    if (button == GLFW_MOUSE_BUTTON_3) {
		//check if a turret is clicked on grid
        for (auto it = turretVector.begin(); it != turretVector.end(); it++) {
            if (checkTurretClick(mouseX, mouseY, *it)) {
                if (it->placed) {
					it->toDelete = true;
				}
			}
		}
	}
}


void tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event

    //check if turret is dropped on grid
    if (button == GLFW_MOUSE_BUTTON_2 && dragTurret) {
        //check if turret is dropped on grid
        for (auto it = turretVector.begin(); it != turretVector.end(); it++) {
            if (checkTurretClick(mouseX, mouseY, *it)) {
                //check turret cost and resources
                if (!it->placed && resources >= dragTurretCost) {
					it->placed = true;
					it->color = dragTurretColor;
                    it->cost = dragTurretCost;
					resources -= it->cost;
					break;
				}
			}
		}
        dragTurret = false;
    }
}


void tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void tema1::OnWindowResize(int width, int height)
{
}
