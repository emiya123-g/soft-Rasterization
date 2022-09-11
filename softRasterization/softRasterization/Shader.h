#pragma once
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"Triangle.h"
#include"Light.h"
#include"Camera.h"
#include"Material.h"
#include<memory>


struct V2f {
	V2f() = default;
	V2f(const glm::vec4& pH, const glm::vec4& pW, const glm::vec4& c, const glm::vec3& n, const glm::vec2 t) :
		posH(pH), posW(pW), color(c), normal(n), texcoord(t) {}
	static V2f lerp(const V2f& v1, const V2f& v2, const float& factor) {
		V2f ret;
		float oneMiusFactor = 1.0f - factor;
		ret.posH = oneMiusFactor * v1.posH + factor * v2.posH;
		ret.posW = oneMiusFactor * v1.posW + factor * v2.posW;
		ret.color = oneMiusFactor * v1.color + factor * v2.color;
		ret.normal = oneMiusFactor * v1.normal + factor * v2.normal;
		ret.texcoord = oneMiusFactor * v1.texcoord + factor * v2.texcoord;
		ret.inverseZ = oneMiusFactor * v1.inverseZ + factor * v2.inverseZ;
		return ret;
	}
	static V2f BarycentricLerp(const V2f& v1, const V2f& v2, const V2f& v3, const float& alpha, const float& beta, const float& gamma) {
		V2f ret;
		
		ret.posH = alpha * v1.posH + beta * v2.posH + gamma * v3.posH;
		ret.posW = alpha * v1.posW + beta * v2.posW + gamma * v3.posW;
		ret.color = alpha * v1.color + beta * v2.color + gamma * v3.color;
		ret.normal = alpha * v1.normal + beta * v2.normal + gamma * v3.normal;
		ret.texcoord = alpha * v1.texcoord + beta * v2.texcoord + gamma * v3.texcoord;
		ret.inverseZ = alpha * v1.inverseZ + beta * v2.inverseZ + gamma * v3.inverseZ;
		return ret;
	}
	glm::vec4 posH;
	glm::vec4 posW;
	glm::vec4 color;
	glm::vec3 normal;
	glm::vec2 texcoord;
	float inverseZ;

};


class Shader {
public:
	Shader() = default;
	Shader(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) :model(m), view(v), proj(p) {}
	virtual V2f VS(const Vertex& a2v) {
		V2f o;
		o.posW = model * a2v.position;
		o.posH = proj * view * o.posW;
		o.color = a2v.color;
		o.normal = a2v.normal;
		o.texcoord = a2v.texcoord;	
		return o;
	}

	virtual glm::vec4 FS(const V2f& v);
	void setModel(const glm::mat4& m) { model = m; }
	void setView(const glm::mat4& v) { view = v; }
	void setProj(const glm::mat4& p) { proj = p; }
	void setMatrial(std::shared_ptr<Material>& mat) { material = mat; }

protected:
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	std::shared_ptr<Material> material;
};

class ShadowMapShader:public Shader{
public:
	ShadowMapShader() = default;
	
	virtual glm::vec4 FS(const V2f& v)override;
};