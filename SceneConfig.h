#ifndef SCENE_CONFIG_H
#define SCENE_CONFIG_H

#include <glm/glm.hpp>

// Window Settings
namespace WindowConfig {
    const unsigned int SCR_WIDTH = 1600;
    const unsigned int SCR_HEIGHT = 960;
}

// Classroom Dimensions
namespace ClassroomConfig {
    const float WIDTH = 50.0f;
    const float HEIGHT = 15.0f;
    const float DEPTH = 50.0f;
    const float WALL_THICKNESS = 0.2f;
}

// Window Dimensions
namespace WindowDimensions {
    const float WIDTH = 15.0f;
    const float HEIGHT = 8.0f;
    const float Y_POSITION = 7.0f;
}

// Door Dimensions
namespace DoorConfig {
    const float WIDTH = 4.0f;
    const float HEIGHT = 8.0f;
    const float Z_POSITION = -18.0f;
}

// Desk Layout
namespace DeskLayout {
    const int NUM_ROWS = 6;
    const int NUM_COLS = 2;
    const int NUM_DESKS_PER_GROUP = 3;
    const float ROW_SPACING = 6.0f;
    const float COL_SPACING = 10.0f;
    const float PAIR_SPACING = 0.5f;
    const float START_X = -20.0f;
    const float START_Z = -15.0f;
}

// Desk Dimensions
namespace DeskDimensions {
    const float MAIN_WIDTH = 5.0f;
    const float MAIN_DEPTH = 1.75f;
    const float MAIN_THICKNESS = 0.2f;
    const float SUB_WIDTH = 5.0f;
    const float SUB_DEPTH = 1.5f;
    const float SUB_THICKNESS = 0.1f;
    const float PANEL_THICKNESS = 0.1f;
    const float HEIGHT = 3.0f;
    const float LEG_WIDTH = 0.1f;
    const float GAP = 0.8f;
}

// Bench Dimensions
namespace BenchDimensions {
    const float WIDTH = 11.0f;
    const float DEPTH = 1.5f;
    const float HEIGHT = 1.5f;
    const float LEG_WIDTH = 0.1f;
}

// Ceiling Fan Configuration
namespace FanConfig {
    const glm::vec3 POSITION(0.0f, 13.5f, 0.0f);
    const float SPEED = 2.0f;
    const float DISK_RADIUS = 1.5f;
    const float DISK_HEIGHT = 0.3f;
    const float BLADE_LENGTH = 2.5f;
    const float BLADE_WIDTH = 0.6f;
    const float BLADE_THICKNESS = 0.08f;
    const int NUM_BLADES = 3;
}

// Projector Configuration
namespace ProjectorConfig {
    const float BODY_WIDTH = 1.5f;
    const float BODY_HEIGHT = 0.5f;
    const float BODY_DEPTH = 1.0f;
    const float LENS_RADIUS = 0.25f;
    const float LENS_LENGTH = 0.15f;
    const float MOUNT_RADIUS = 0.1f;
    const float MOUNT_LENGTH = 2.5f;
}

// Material Colors
namespace Colors {
    // Wood
    const glm::vec3 WOOD_AMBIENT(0.3f, 0.2f, 0.1f);
    const glm::vec3 WOOD_DIFFUSE(0.6f, 0.4f, 0.2f);
    const glm::vec3 WOOD_SPECULAR(0.3f, 0.2f, 0.1f);

    // Wall
    const glm::vec3 WALL_AMBIENT(0.4f, 0.35f, 0.25f);
    const glm::vec3 WALL_DIFFUSE(0.7f, 0.6f, 0.45f);
    const glm::vec3 WALL_SPECULAR(0.3f, 0.3f, 0.3f);

    // Floor
    const glm::vec3 FLOOR_AMBIENT(0.1f, 0.3f, 0.1f);
    const glm::vec3 FLOOR_DIFFUSE(0.2f, 0.6f, 0.2f);
    const glm::vec3 FLOOR_SPECULAR(0.1f, 0.1f, 0.1f);

    // Ceiling
    const glm::vec3 CEILING_AMBIENT(0.05f, 0.05f, 0.05f);
    const glm::vec3 CEILING_DIFFUSE(0.1f, 0.1f, 0.1f);
    const glm::vec3 CEILING_SPECULAR(0.05f, 0.05f, 0.05f);

    // Metal
    const glm::vec3 METAL_DARK_AMBIENT(0.15f, 0.15f, 0.15f);
    const glm::vec3 METAL_DARK_DIFFUSE(0.25f, 0.25f, 0.25f);
    const glm::vec3 METAL_DARK_SPECULAR(0.4f, 0.4f, 0.4f);

    const glm::vec3 METAL_LIGHT_AMBIENT(0.7f, 0.7f, 0.7f);
    const glm::vec3 METAL_LIGHT_DIFFUSE(0.85f, 0.85f, 0.85f);
    const glm::vec3 METAL_LIGHT_SPECULAR(0.9f, 0.9f, 0.9f);

    // Glass/Lens
    const glm::vec3 LENS_AMBIENT(0.05f, 0.05f, 0.15f);
    const glm::vec3 LENS_DIFFUSE(0.1f, 0.1f, 0.3f);
    const glm::vec3 LENS_SPECULAR(0.8f, 0.8f, 0.9f);

    // Window
    const glm::vec3 WINDOW_AMBIENT(0.1f, 0.1f, 0.1f);
    const glm::vec3 WINDOW_DIFFUSE(0.2f, 0.2f, 0.2f);
    const glm::vec3 WINDOW_SPECULAR(0.5f, 0.5f, 0.5f);

    // Bench (brown wood)
    const glm::vec3 BENCH_AMBIENT(0.2f, 0.13f, 0.07f);
    const glm::vec3 BENCH_DIFFUSE(0.4f, 0.25f, 0.13f);
    const glm::vec3 BENCH_SPECULAR(0.2f, 0.13f, 0.07f);
}

#endif