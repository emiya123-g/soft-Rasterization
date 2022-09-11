#pragma once
#include"glm/glm.hpp"
#include"Shader.h"
#include<vector>
#include<algorithm>
#include"FrameBuffer.h"
#include"Shader.h"
#include"Transform.h"
#include<tuple>
#include<memory>


class Scanline {
public:
	static void scanLineTriangle(const V2f& v1, const V2f& v2, const V2f& v3, FrameBuffer& buffer, Shader& shader);
	static void upTriangle(const V2f& v1, const V2f& v2, const V2f& v3, FrameBuffer& buffer, Shader& shader);
	static void downTriangle(const V2f& v1, const V2f& v2, const V2f& v3, FrameBuffer& buffer, Shader& shader);
	static void scanLine(const V2f& left, const V2f& right, FrameBuffer& buffer, Shader& shader);
};

class halfSpace {
public:
	static void halfSpaceTriangle(const V2f& v1, const V2f& v2, const V2f& v3, FrameBuffer& buffer, Shader& shader);
private:
	static std::tuple<float, float, float> calBarycentricCoord(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p);

	static bool halfSpaceTest(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p);
};

class CVVclip {
public:
	static std::vector<V2f> sutherlandHodgeman(const V2f& v1, const V2f& v2, const V2f& v3);
	static void perspectiveDivision(V2f& v) {
		v.inverseZ = 1.0 / v.posH.w; //actally w = -z ,but after that the interpolated attribution would div the interpolated inverseZ,so ingonre the nagetive sign
		v.posH /= v.posH.w;
		v.posH.w = 1.0f;
		v.posH.z = (v.posH.z + 1.0) * 0.5f;

		v.posW *= v.inverseZ;     //modify perspective distortion. step 1
		v.texcoord *= v.inverseZ;
		v.color *= v.inverseZ;
		v.normal *= v.inverseZ;
	}
private:
	static bool Inside(const glm::vec4& line, const glm::vec4& p) {
		return line.x * p.x + line.y * p.y + line.z * p.z + line.w * p.w>=0;
	}
	static V2f Intersect(const V2f& v1, const V2f& v2, const glm::vec4& line) {
		float d1 = v1.posH.x * line.x + v1.posH.y * line.y + v1.posH.z * line.z + v1.posH.w * line.w;
		float d2 = v2.posH.x * line.x + v2.posH.y * line.y + v2.posH.z * line.z + v2.posH.w * line.w;

		float weight = d1 / (d1 - d2);
		return V2f::lerp(v1, v2, weight);
	}
	static bool allVertexInside(const std::vector<V2f>& src) {
		for (const V2f& v : src) {
			for (const glm::vec4& line : viewLines) {
				if (!Inside(line, v.posH))
					return false;
			}
		}
		return true;
	}
	static const std::vector<glm::vec4> viewLines;
};

class RasterEngine {
public:
	RasterEngine(int w, int h) :m_width(w),m_height(h) {}
	
	void drawTriangleScanLine(const Vertex& v1, const Vertex& v2, const Vertex& v3);
	
	void drawMeshScanLine(const Mesh& mesh);
	
	void drawMeshHalfSpace(const Mesh& mesh);
	
	void update(int w, int h);

public:
	std::shared_ptr<FrameBuffer> buffer;
	std::shared_ptr<Shader> shader;
	int m_width;
	int m_height;
};

