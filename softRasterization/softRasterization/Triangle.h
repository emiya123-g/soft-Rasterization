#pragma once
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include<vector>
class Vertex {
public:
	Vertex() = default;
	Vertex(glm::vec4& p, const glm::vec4& c, const glm::vec3& n, const glm::vec2& t) :position(p), color(c), normal(n), texcoord(t) {}
	Vertex(const Vertex& v) :position(v.position),color(v.color),normal(v.normal),texcoord(v.texcoord){}
	Vertex(const glm::vec4& p, const glm::vec4& c = glm::vec4(0, 0, 0, 0), 
		const glm::vec3& n = glm::vec3(0, 0, 0), const glm::vec2& t= glm::vec2(0, 0)) :position(p), color(c), normal(n), texcoord(t) {}
public:
	glm::vec4 position;
	glm::vec4 color;
	glm::vec3 normal;
	glm::vec2 texcoord;
};

class Triangle {
public:
	Triangle() = default;
	Triangle(const Vertex& a, const Vertex& b, const Vertex& c) :va(a),vb(b),vc(c) {}
public:
	Vertex va;
	Vertex vb;
	Vertex vc;
};

class Mesh {
public:
	Mesh() = default;
	Mesh(int vertexNum, int indexNum) {
		vertexBuffer.resize(vertexNum);
		indexBuffer.resize(indexNum);
	}
	void addTriangle(const Vertex &v1,const Vertex &v2,const Vertex &v3) {
		int offset = vertexBuffer.size();
		vertexBuffer.push_back(v1);
		vertexBuffer.push_back(v2);
		vertexBuffer.push_back(v3);
		indexBuffer.push_back(offset);
		indexBuffer.push_back(offset + 1);
		indexBuffer.push_back(offset + 2);
	}
	void addMesh(const Mesh& mesh) {
		int offset = vertexBuffer.size();
		vertexBuffer.insert(vertexBuffer.end(), mesh.vertexBuffer.begin(), mesh.vertexBuffer.end());
		indexBuffer.reserve(indexBuffer.size() + mesh.indexBuffer.size());
		for (int i = 0; i < mesh.indexBuffer.size(); i++) {
			indexBuffer.push_back(mesh.indexBuffer[i] + offset);
		}
	}
	
public:
	std::vector<Vertex> vertexBuffer;
	std::vector<unsigned int> indexBuffer;
};

