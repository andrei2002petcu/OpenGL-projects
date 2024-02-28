#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/lab_camera.h"


namespace m1
{
    enum class MovementState
    {
        GoingForward,
        GoingBackward,
        InPlaceRotationLeft,
        InPlaceRotationRight,
        Stop,
        Shoot
    };

    struct projectile {
        glm::mat4 modelMatrix;
        float time_to_live;

        projectile(glm::mat4 modelMatrix) {
			this->modelMatrix = modelMatrix;
			this->time_to_live = 5;
		}
    };

    struct building {

        glm::vec3 position;
        glm::vec3 scale;
        glm::mat4 modelMatrix;

        building(glm::vec3 position, glm::vec3 scale) {
			this->position = position;
			this->scale = scale;
			this->modelMatrix = glm::mat4(1);
            this->modelMatrix = glm::translate(this->modelMatrix, this->position);
            this->modelMatrix = glm::scale(this->modelMatrix, this->scale);
		}
    };

    struct enemy {
        glm::mat4 modelMatrix;
        glm::mat4 turela_modelMatrix;
        float turela_angle;
        int health = 4;
        glm::vec3 color;
        MovementState state;
        float timer;
        float tank_angle;
        float shoot_timer = -3;
        bool to_fall = false;

        enemy(glm::mat4 modelMatrix, bool to_fall) {
            this->modelMatrix = modelMatrix;
            this->turela_modelMatrix = glm::mat4(1);
            this->turela_angle = 0;
            this->color = glm::vec3(0.6, 0.2, 0);
            this->state = MovementState::GoingForward;
            this->timer = 3;
            this->tank_angle = 0;
            this->to_fall = to_fall;
        }
    };

    class tema2 : public gfxc::SimpleScene
    {
     public:
         tema2();
        ~tema2();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, const int& health);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        implemented::Camera *camera;
        glm::mat4 projectionMatrix;

        int FOV;
        float left, right, bottom, top;

        glm::mat4 tank_modelMatrix;
        glm::mat4 turela_modelMatrix;
        float turela_angle;
        glm::vec3 tank_direction;
        float tank_angle;
        float camera_angle;
        float shoot_timer;
        int tank_health;
        glm::vec3 tank_color;

        float tankSpeed;
        float cameraSpeedRotation;
        float time_to_play;
        float points;
        float time_to_spawn;

        //projectiles vector
        std::vector<projectile> projectiles;

        //buildings vector
        std::vector<building> buildings;

        //enemies vector
        std::vector<enemy> enemies;

        // light
        glm::vec3 lightPosition;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;
    };
}   // namespace m1
