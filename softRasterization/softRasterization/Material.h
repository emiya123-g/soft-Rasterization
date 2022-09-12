#pragma once
#include"glm/glm.hpp"
#include"Texture2D.h"

class Material
{
public:
	Material() = default;
	Material(glm::vec3 baseColor) :albedo(baseColor),texture(),metalness(0.5f),roughness(0.5f){}
	Material(const std::string& filename):texture(filename),metalness(0.1f),roughness(0.1f){}
	void addMetalness(const float& x) { metalness += x; metalness = std::max(std::min(metalness, 1.0f), 0.0f); }
	void addRoughness(const float& x) { roughness += x; roughness = std::max(std::min(roughness, 1.0f), 0.0f); }
	glm::vec3 cookTorrance(const glm::vec3& normal, const glm::vec3& viewDir, const glm::vec3& lightDir, const glm::vec3& albedo) {

		glm::vec3 halfVec = glm::normalize(viewDir + lightDir);

		glm::vec3 F0 = glm::vec3(0.04);
		F0 = glm::mix(F0, albedo, metalness);
		glm::vec3 F = fresnelSchlick(std::max(glm::dot(halfVec, viewDir), 0.0f), F0);


		float NDF = DistributionGGX(normal, halfVec, roughness);
		float G = GeometrySmith(normal, viewDir, lightDir, roughness);

		glm::vec3 nominator = NDF * G * F;
		float denominator = 4.0 * std::max(glm::dot(normal, viewDir), 0.0f) * std::max(glm::dot(normal, lightDir), 0.0f) + 0.001f;

		glm::vec3 specular = nominator / denominator;

		glm::vec3 kS = F;
		glm::vec3 kD = glm::vec3(1.0) - kS;

		kD *= 1.0f - metalness;

		float NdotL = std::max(glm::dot(normal, lightDir), 0.0f);

		return (kD * albedo  + specular) * NdotL;
	}
private:
	glm::vec3 fresnelSchlick(const float& cosTheta, const glm::vec3& F0)
	{
		return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
	}

	float DistributionGGX(const glm::vec3& N, const glm::vec3& H, const float& roughness)
	{
		float a = roughness * roughness;
		float a2 = a * a;
		float NdotH = std::max(glm::dot(N, H), 0.0f);
		float NdotH2 = NdotH * NdotH;

		float nom = a2;
		float denom = (NdotH2 * (a2 - 1.0) + 1.0);
		denom = 3.1415926f * denom * denom;

		return nom / denom;
	}

	float GeometrySchlickGGX(const float& NdotV, const float& roughness)
	{
		float r = (roughness + 1.0);
		float k = (r * r) / 8.0;

		float nom = NdotV;
		float denom = NdotV * (1.0 - k) + k;

		return nom / denom;
	}

	float GeometrySmith(const glm::vec3& N, const glm::vec3& V, const glm::vec3& L, const float& roughness)
	{
		float NdotV = std::max(glm::dot(N, V), 0.0f);
		float NdotL = std::max(glm::dot(N, L), 0.0f);
		float ggx2 = GeometrySchlickGGX(NdotV, roughness);
		float ggx1 = GeometrySchlickGGX(NdotL, roughness);

		return ggx1 * ggx2;
	}

public:
	glm::vec3 albedo;
	Texture2D texture;
	float metalness;
	float roughness;

};

