#ifndef OBJECT_ANIMATOR_H
#define OBJECT_ANIMATOR_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <functional>

// Enum for animation types
enum AnimationType {
    CIRCULAR,      // Move in a circle
    LINEAR,        // Move back and forth in a line
    FIGURE_EIGHT,  // Move in figure-8 pattern
    ORBIT,         // Orbit around a point
    BOUNCE,        // Bounce up and down
    CUSTOM         // Use custom function
};

// Class to animate objects (lights or boxes)
class ObjectAnimator
{
public:
    // Constructor
    ObjectAnimator(glm::vec3 startPosition = glm::vec3(0.0f));

    // Set animation type
    void setAnimationType(AnimationType type);

    // Set animation parameters
    void setRadius(float r);              // For circular/orbit animations
    void setSpeed(float s);               // Animation speed multiplier
    void setAmplitude(glm::vec3 amp);     // For linear/bounce animations
    void setCenter(glm::vec3 c);          // Center point for orbit/circular
    void setAxis(glm::vec3 a);            // Axis for circular motion

    // Enable/disable animation
    void setEnabled(bool enabled);
    bool isEnabled() const;

    // Update position based on time
    glm::vec3 update(float currentTime);

    // Get current position
    glm::vec3 getPosition() const;

    // Set custom animation function
    // Function takes (startPos, currentTime, speed) and returns new position
    void setCustomFunction(std::function<glm::vec3(glm::vec3, float, float)> func);

    // Reset to start position
    void reset();

private:
    glm::vec3 startPosition;     // Initial position
    glm::vec3 currentPosition;   // Current position
    glm::vec3 center;            // Center for orbit/circular
    glm::vec3 axis;              // Rotation axis
    glm::vec3 amplitude;         // Movement amplitude

    AnimationType animType;      // Current animation type
    float radius;                // Radius for circular motion
    float speed;                 // Speed multiplier
    bool enabled;                // Is animation active

    // Custom animation function
    std::function<glm::vec3(glm::vec3, float, float)> customFunc;

    // Internal animation calculations
    glm::vec3 calculateCircular(float time);
    glm::vec3 calculateLinear(float time);
    glm::vec3 calculateFigureEight(float time);
    glm::vec3 calculateOrbit(float time);
    glm::vec3 calculateBounce(float time);
};

#endif