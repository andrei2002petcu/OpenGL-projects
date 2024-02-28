#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 center,
    float length,
    glm::vec3 color,
    bool fill)
{
    length /= 2;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center + glm::vec3(length, length, 0), color),
        VertexFormat(center + glm::vec3(length, -length, 0), color),
        VertexFormat(center + glm::vec3(-length, -length, 0), color),
        VertexFormat(center + glm::vec3(-length, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 center,
    float length,
    float width,
    glm::vec3 color,
    bool fill)
{
	length /= 2;
	width /= 2;
	std::vector<VertexFormat> vertices =
    {
		VertexFormat(center + glm::vec3(width, length, 0), color),
		VertexFormat(center + glm::vec3(width, -length, 0), color),
		VertexFormat(center + glm::vec3(-width, -length, 0), color),
		VertexFormat(center + glm::vec3(-width, length, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
		rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
		// Draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

Mesh* object2D::CreateTurret(
    const std::string& name,
    glm::vec3 center,
    float length,
    glm::vec3 color)
{
    length /= 2;
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, -length, 1), color), //0 A
        VertexFormat(glm::vec3(0, length, 1), color), //1 B
        VertexFormat(glm::vec3(-0.5f * length, 0, 1), color), //2 C 
        VertexFormat(glm::vec3(0.5f * length, 0, 1), color), //3 D

        VertexFormat(glm::vec3(0.25f * length, 0.2f * length, 1), color), //4 E
        VertexFormat(glm::vec3(0.25f * length, -0.2f * length, 1), color), //5 F
        VertexFormat(glm::vec3(0.85f * length, 0.2f * length, 1), color), //6 G
        VertexFormat(glm::vec3(0.85f * length, -0.2f * length, 1), color), //7 H
    };

    Mesh* turret = new Mesh(name);

    std::vector<unsigned int> indices =
    {
        0, 2, 3,
        1, 2, 3,
        4, 5, 6,
        5, 7, 6
    };


    turret->SetDrawMode(GL_TRIANGLES);
    turret->InitFromData(vertices, indices);
    return turret;
}

Mesh* object2D::CreateHexagon(
    const std::string& name,
    glm::vec3 center,
    float length,
    glm::vec3 color,
    int inner)
{
    length /= 2;

    int z = 2;
    if (inner) {
        z++;
	}
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, length, z), color),
        VertexFormat(glm::vec3(0.866f * length, 0.5f * length, z), color),
        VertexFormat(glm::vec3(0.866f * length, -0.5f * length, z), color),
        VertexFormat(glm::vec3(0, -length, z), color),
        VertexFormat(glm::vec3(-0.866f * length, -0.5f * length, z), color),
        VertexFormat(glm::vec3(-0.866f * length, 0.5f * length, z), color)
    };

	Mesh* hexagon = new Mesh(name);

	std::vector<unsigned int> indices =
    {
        0, 1, 2, 3, 4, 5
	};

    hexagon->SetDrawMode(GL_TRIANGLE_FAN);
    hexagon->InitFromData(vertices, indices);
    return hexagon;
}

Mesh* object2D::CreateStar(
    const std::string& name,
    glm::vec3 center,
    float length,
    glm::vec3 color,
    int z)
{

    length /= 2;
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(0, length, z), color), //0 A
        VertexFormat(glm::vec3(-0.22f * length, 0.31f * length, z), color), //1 B
        VertexFormat(glm::vec3(-0.94f * length, 0.31f * length, z), color), //2 C 
        VertexFormat(glm::vec3(-0.36f * length, -0.1f * length, z), color), //3 D
		VertexFormat(glm::vec3(-0.6f * length, -0.8f * length, z), color), //4 E
		VertexFormat(glm::vec3(0, -0.36f * length, z), color), //5 F
		VertexFormat(glm::vec3(0.6f * length, -0.8f * length, z), color), //6 G
		VertexFormat(glm::vec3(0.36f * length, -0.1f * length, z), color), //7 H
        VertexFormat(glm::vec3(0.94f * length, 0.31f * length, z), color), //8 I
        VertexFormat(glm::vec3(0.22f * length, 0.31f * length, z), color), //9 J
        VertexFormat(glm::vec3(0, 0, z), color), //10 K
	};

    Mesh* star = new Mesh(name);

    std::vector<unsigned int> indices =
    {
		10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0
	};

    star->SetDrawMode(GL_TRIANGLE_FAN);
	star->InitFromData(vertices, indices);
	return star;
}