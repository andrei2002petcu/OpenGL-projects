#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 center, float length, glm::vec3 color, bool fill = false);

    // Create rectangle with given bottom left corner, length, width and color
    Mesh* CreateRectangle(const std::string &name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 color, bool fill = false);

    // Create turret with given color
    Mesh* CreateTurret(const std::string &name, glm::vec3 center ,float length, glm::vec3 color);

    // Create Hexagon with given center, length and color
    Mesh* CreateHexagon(const std::string &name, glm::vec3 center, float length, glm::vec3 color, int inner);

    // Create star with given center, length and color
    Mesh* CreateStar(const std::string &name, glm::vec3 center, float length, glm::vec3 color, int z);
}
