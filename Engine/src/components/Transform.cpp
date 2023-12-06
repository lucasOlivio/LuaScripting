#include "components/Transform.h"
#include "common/utils.h"
#include "common/utilsMat.h"
#include "common/opengl.h"
#include "common/constants.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/euler_angles.hpp>

void TransformComponent::SetOrientation(glm::vec3 value)
{
	using namespace glm;

	float radX = radians(value.x);
	float radY = radians(value.y);
	float radZ = radians(value.z);

	vec3 eulerAngles(radX, radY, radZ);

	// Combine quaternions
	m_qOrientation = quat(eulerAngles);
}

void TransformComponent::SetOrientation(glm::quat value)
{
	m_qOrientation = value;
}

void TransformComponent::AdjustOrientation(glm::vec3 value)
{
	using namespace glm;

	// To combine quaternion values, you multiply them together
	// Make a quaternion that represents that CHANGE in angle
	quat qChange = quat(radians(value));

	// Multiply them together to get the change
	// Just like with matrix math
	m_qOrientation *= qChange;
}

void TransformComponent::Move(glm::vec3 deltaValue)
{
	m_oldPosition = m_position;
	m_position.x += deltaValue.x;
	m_position.y += deltaValue.y;
	m_position.z += deltaValue.z;
}

void TransformComponent::SetFramePosition()
{
	m_framePosition = m_position;
	return;
}

void TransformComponent::MoveTo(glm::vec3 value)
{
	m_oldPosition = m_position;
	m_position = value;
}

void TransformComponent::SetPosition(glm::vec3 value)
{
	m_oldPosition = m_position;
	m_position = value;
	m_initialPosition = value;
	return;
}

void TransformComponent::SetDistanceToCamera(float value)
{
	m_distanceToCamera = value;
}

float TransformComponent::GetDistanceToCamera()
{
	return m_distanceToCamera;
}

void TransformComponent::ResetPosition()
{
	m_position = m_initialPosition;
}

void TransformComponent::ResetFramePosition()
{
	m_position = m_framePosition;
}

void TransformComponent::SetOldPosition()
{
	glm::vec3 tempPosDelta = m_position - m_oldPosition;
	m_position = m_oldPosition;
	m_oldPosition = m_oldPosition - tempPosDelta;
	return;
}

void TransformComponent::SetOldPosition(int n)
{
	glm::vec3 tempPosDelta = m_position - m_oldPosition;
	m_position = m_position - (tempPosDelta * (float)n);
	m_oldPosition = m_oldPosition - tempPosDelta;
	return;
}

glm::vec3 TransformComponent::GetPosition()
{
	return m_position;
}

glm::vec3 TransformComponent::GetOldPosition()
{
	return m_oldPosition;
}

glm::quat TransformComponent::GetQuatOrientation()
{
	return m_qOrientation;
}

glm::vec3 TransformComponent::GetOrientation()
{
	using namespace glm;


	// Extract rotation angles in radians
	vec3 rotationRadians = eulerAngles(m_qOrientation);

	vec3 rotationDegrees = degrees(rotationRadians);

	return rotationDegrees;
}

glm::vec3 TransformComponent::GetUpVector()
{
	return GetQuatOrientation() * UP_VECTOR;
}

glm::vec3 TransformComponent::GetRightVector()
{
	return GetQuatOrientation() * RIGHT_VECTOR;
}

glm::vec3 TransformComponent::GetForwardVector()
{
	return GetQuatOrientation() * FORWARD_VECTOR;
}

glm::vec3 TransformComponent::GetRelativeVector(const glm::vec3& directionIn)
{
	return GetQuatOrientation() * directionIn;
}

void TransformComponent::AdjustScale(float value)
{
	m_scale += value;
}

void TransformComponent::SetScale(float value)
{
	m_scale = value;
}

float TransformComponent::GetScale()
{
	return m_scale;
}

glm::mat4 TransformComponent::GetTransform(glm::mat4& transformMat)
{
	using namespace myutils;

	ApplyTranslation(GetPosition(), transformMat);
	ApplyRotation(GetQuatOrientation(), transformMat);
	ApplyScale(GetScale(), transformMat);

	return transformMat;
}

glm::mat4 TransformComponent::GetTransform()
{
	glm::mat4 transformMat = glm::mat4(1.0f);

	return GetTransform(transformMat);
}

glm::mat4 TransformComponent::GetTransformNoRotation()
{
	using namespace myutils;

	glm::mat4 transformMat = glm::mat4(1.0f);
	ApplyTranslation(GetPosition(), transformMat);
	ApplyScale(GetScale(), transformMat);

	return transformMat;
}

glm::mat4 TransformComponent::GetTransformNoScale()
{
	using namespace myutils;

	glm::mat4 transformMat = glm::mat4(1.0f);
	ApplyTranslation(GetPosition(), transformMat);
	ApplyRotation(GetQuatOrientation(), transformMat);

	return transformMat;
}

void TransformComponent::GetInfo(sComponentInfo& compInfoOut)
{
	compInfoOut.componentName = "transform";
	compInfoOut.componentParameters.clear();

	AddCompParInfo("position", "vec3", GetPosition(), compInfoOut);
	AddCompParInfo("scale", "float", GetScale(), compInfoOut);
	AddCompParInfo("orientation", "vec3", GetOrientation(), compInfoOut);

	Component::GetInfo(compInfoOut);
}

void TransformComponent::SetParameter(sParameterInfo& parameterIn)
{
	Component::SetParameter(parameterIn);

	if (parameterIn.parameterName == "position") {
		SetPosition(parameterIn.parameterVec3Value);
	}
	else if (parameterIn.parameterName == "orientation") {
		SetOrientation(parameterIn.parameterVec3Value);
	}
	else if (parameterIn.parameterName == "scale") {
		SetScale(parameterIn.parameterFloatValue);
	}

	return;
}

bool SortTransformFromCamera(TransformComponent* pTransformA,
							 TransformComponent* pTransformB)
{
	// Sort from more distant, to closer
	return pTransformA->GetDistanceToCamera() > pTransformB->GetDistanceToCamera();
}