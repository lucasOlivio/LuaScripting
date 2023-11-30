#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

void __CheckEngineError(const char* msg, const char* file, int line);
#define CheckEngineError(msg) __CheckEngineError(msg, __FILE__, __LINE__)

namespace myutils
{
    // Get string after last "." in file name
    std::string GetFileExtension(const std::string& fileName);

    // Remove paths from string and return only the file name
    char* GetFileFromPath(const char* fullPath);

    // Split string based on delimiter
    void SplitString(const std::string& stringIn,
        char delimiter,
        std::vector<std::string>& vecOut);

    // Convert array to a string separated by delimiter
    template <typename T>
    std::string ArrayToString(char delimiter, const std::vector<T>& arrIn);

    // Converts bool to strings "0" or "1"
    std::string BoolToString(bool input);

    // Get only sign of number (1 or -1)
    int Sign(int val);

    // TODO: Improve this, is too repetitive
    // Pseudo random float between a and b
    float GetRandFloat(float a, float b);

    // Pseudo random float between a and b excluding c to d
    float GetRandFloat(float a, float b, float c, float d);

    // Wrap number between min and max (both end inclusives)
    void WrapMinMax(int min, int max, int& numOut);
    void WrapMinMax(float min, float max, float& numOut);

    // Converts the glm vec string from "string_cast" back to glm::vec
    glm::vec4 StringToVec4(const std::string& glmstr);
    glm::vec3 StringToVec3(const std::string& glmstr);

    // Converts glm to an array string [x, y, z]
    std::string VecToString(glm::vec3 glmVec);
    std::string VecToString(glm::vec4 glmVec);

    double FormatFloatWithPrecision(float value, int decimalPlaces);

    bool TxtToPointVector(std::string filename, char one, char zero, glm::vec3 minXY, glm::vec3 maxXY, std::vector<glm::vec3>& vecOut);

    // Calculate the vector proportion by a given ratio 
    glm::vec3 CalculateVector(glm::vec3 startXYZ, glm::vec3 endXYZ, float ratio);

    // Calculate new velocity based on time, initial velocity and acceleration
    glm::vec3 IncreaseVelocity(glm::vec3 initialVelocity, glm::vec3 acceleration, float deltaTime);
    // Calculate the needed velocity to go from start to end in the given time
    glm::vec3 CalculateVelocity(glm::vec3 startXYZ, glm::vec3 endXYZ,
                                float initialTime, float finalTime);

    // Calculate acceleration needed to reach the target position at the specified time
    glm::vec3 CalculateAcceleration(glm::vec3 startXYZ, glm::vec3 endXYZ,
                                    glm::vec3 initialVelocity,
                                    float initialTime, float finalTime);

    // Calculate acceleration needed to reach the target position at the specified time
    glm::vec3 CalculateAcceleration(glm::vec3 startXYZ, glm::vec3 endXYZ,
                                    float initialTime, float finalTime);

    // Calculate the position on the Bezier curve at a given time
    glm::vec3 CalculateBezierPoint(std::vector<glm::vec3>& controlPoints, float time);
    // Calculate the time along the Bezier curve for a given distance
    float CalculateBezierTime(std::vector<glm::vec3>& controlPoints, float distance);

    // Generates an unique UUID using random lib and uniform dist
    // 16 bits to simplify and be able to use with Lua scripts
    uint16_t GenerateUUID();
}
