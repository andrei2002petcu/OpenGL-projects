#pragma once

#include "components/simple_scene.h"


namespace m1
{
    struct Resource {
        float x, y; //coordinates

        //constructor
        Resource(float x, float y) {
            this->x = x;
            this->y = y;
        }
    };

    struct Turret {
        float x, y; //coordinates
        int color; //color
		int cost; //cost
        int row;
        bool placed; //if it was placed on the grid
        bool toDelete; //if it was deleted from the grid
        float scale;
        float r; //radius
        float timer;

		//constructor
        Turret(float x, float y, int color, int cost, int row, bool placed) {
			this->x = x;
			this->y = y;
			this->row = row;
			this->color = color;
			this->cost = cost;
            this->placed = placed;
            this->toDelete = false;
            this->scale = 1;
            this->r = 42.5f;
            this->timer = 0;
		}
	};


    struct Bullet {
        float x, y; //coordinates
        int color; //color
        int row;
        float angularStep;
        float r; //radius

        //constructor
        Bullet(float x, float y, int color, int row) {
            this->x = x;
            this->y = y;
            this->color = color;
            this->row = row;
            this->r = 30;
            this->angularStep = 0;
        }
    };

    struct Enemy {
        float x, y; //coordinates
        int color; //color
        int health; //health
        bool toDelete;
        float scale;
        int row;
        float r; //radius

        //constructor
        Enemy(float y, int color, int row) {
			this->x = 1330;
            this->y = y;
			this->color = color;
			this->health = 3;
			this->toDelete = false;
			this->scale = 1;
			this->row = row;
            this->r = 43.3f;
		}
    };
    
    class tema1 : public gfxc::SimpleScene
    {
     public:
         tema1();
        ~tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        float cx, cy;
        glm::mat3 modelMatrix;

        //square grid coordinates
        float gridX, gridY;

        //square dimensions
        float squareSide;

        //turret GUI coordinates
        float turretX, turretY;

        //lives remaining
        int lives;

        //lives GUI coordinates
        float livesX, livesY;

        //resources remaining
        int resources;

        //vector of resources
        std::vector<Resource> resourceVector;
        // timer for resource generation
        float resourceTimer;
        int resourceTimerMax;

        //vector of turrets for GUI
        std::vector<Turret> GUIturretVector;

        //drag and drop turret
        bool dragTurret;
        int dragTurretColor;
        int dragTurretCost;
        float dragX, dragY;

        //vector of turrets
        std::vector<Turret> turretVector;

        //vector of enemies
        std::vector<Enemy> enemyVector;
        float enemyTimer;
        int enemyTimerMax;

        //vector of bullets
        std::vector<Bullet> bulletVector;
    };
}   // namespace m1
