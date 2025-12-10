#include "ObjectAnimator.h"
#include <cmath>

// STEP 1: Constructor - Initialize with default values
ObjectAnimator::ObjectAnimator(glm::vec3 startPosition)
    : startPosition(startPosition),
    currentPosition(startPosition),
    center(glm::vec3(0.0f)),
    axis(glm::vec3(0.0f, 1.0f, 0.0f)),  // Default: rotate around Y-axis
    amplitude(glm::vec3(1.0f)),
    animType(CIRCULAR),
    radius(3.0f),
    speed(1.0f),
    enabled(true),
    customFunc(nullptr)
{
}

// STEP 2: Setters - Configure animation parameters
void ObjectAnimator::setAnimationType(AnimationType type)
{
    animType = type;
}

void ObjectAnimator::setRadius(float r)
{
    radius = r;
}

void ObjectAnimator::setSpeed(float s)
{
    speed = s;
}

void ObjectAnimator::setAmplitude(glm::vec3 amp)
{
    amplitude = amp;
}

void ObjectAnimator::setCenter(glm::vec3 c)
{
    center = c;
}

void ObjectAnimator::setAxis(glm::vec3 a)
{
    axis = glm::normalize(a);
}

void ObjectAnimator::setEnabled(bool e)
{
    enabled = e;
}

bool ObjectAnimator::isEnabled() const
{
    return enabled;
}

void ObjectAnimator::setCustomFunction(std::function<glm::vec3(glm::vec3, float, float)> func)
{
    customFunc = func;
}

void ObjectAnimator::reset()
{
    currentPosition = startPosition;
}

glm::vec3 ObjectAnimator::getPosition() const
{
    return currentPosition;
}

// STEP 3: Main Update Function - Called every frame
glm::vec3 ObjectAnimator::update(float currentTime)
{
    // If animation is disabled, return start position
    if (!enabled)
    {
        currentPosition = startPosition;
        return currentPosition;
    }

    // Apply speed multiplier to time
    float t = currentTime * speed;

    // Calculate new position based on animation type
    switch (animType)
    {
    case CIRCULAR:
        currentPosition = calculateCircular(t);
        break;

    case LINEAR:
        currentPosition = calculateLinear(t);
        break;

    case FIGURE_EIGHT:
        currentPosition = calculateFigureEight(t);
        break;

    case ORBIT:
        currentPosition = calculateOrbit(t);
        break;

    case BOUNCE:
        currentPosition = calculateBounce(t);
        break;

    case CUSTOM:
        if (customFunc)
            currentPosition = customFunc(startPosition, t, speed);
        else
            currentPosition = startPosition;
        break;

    default:
        currentPosition = startPosition;
        break;
    }

    return currentPosition;
}

// STEP 4: Animation Implementations

// Move in a circle in the XZ plane
glm::vec3 ObjectAnimator::calculateCircular(float time)
{
    float x = center.x + radius * cos(time);
    float y = center.y + startPosition.y;  // Keep Y constant
    float z = center.z + radius * sin(time);
    return glm::vec3(x, y, z);
}

// Move back and forth along a line
glm::vec3 ObjectAnimator::calculateLinear(float time)
{
    // Use sine wave for smooth back-and-forth motion
    float offset = sin(time) * amplitude.x;
    return startPosition + glm::vec3(offset, 0.0f, 0.0f);
}

// Move in a figure-8 pattern (Lissajous curve)
glm::vec3 ObjectAnimator::calculateFigureEight(float time)
{
    float x = center.x + radius * sin(time);
    float y = center.y + radius * sin(time * 2.0f) * 0.5f;  // Double frequency
    float z = center.z;
    return glm::vec3(x, y, z);
}

// Orbit around a center point with custom axis
glm::vec3 ObjectAnimator::calculateOrbit(float time)
{
    // Create rotation matrix around the specified axis
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), time, axis);

    // Calculate offset from center
    glm::vec3 offset = startPosition - center;

    // Apply rotation
    glm::vec4 rotatedOffset = rotation * glm::vec4(offset, 1.0f);

    // Return rotated position
    return center + glm::vec3(rotatedOffset);
}

// Bounce up and down
glm::vec3 ObjectAnimator::calculateBounce(float time)
{
    // Use absolute sine for bounce effect
    float bounce = abs(sin(time)) * amplitude.y;
    return startPosition + glm::vec3(0.0f, bounce, 0.0f);
}