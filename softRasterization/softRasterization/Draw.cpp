#include"Draw.h"

extern glm::mat4 viewPort;

const std::vector<glm::vec4> CVVclip::viewLines = {
	//near
	glm::vec4(0,0,1,1),
	//far
	glm::vec4(0,0,-1,1),
	//left
	glm::vec4(1,0,0,1),
	//right
	glm::vec4(-1,0,0,1),
	//top 
	glm::vec4(0,-1,0,1),
	//bottom
	glm::vec4(0,1,0,1)
};

void Scanline::scanLineTriangle(const V2f& v1, const V2f& v2, const V2f& v3, FrameBuffer& buffer, Shader& shader)
{
	std::vector<V2f> arr = { v1,v2,v3 };
	sort(arr.begin(), arr.end(), [](V2f& v1, V2f& v2) {
		return v1.posH.y < v2.posH.y;
	});
	
	if (arr[1].posH.y == arr[2].posH.y) {
		downTriangle(arr[1], arr[2], arr[0],buffer,shader);
	}
	else if (arr[1].posH.y == arr[0].posH.y) {
		upTriangle(arr[1], arr[0], arr[2], buffer, shader);
	}
	else {
		float factor = (arr[2].posH.y - arr[1].posH.y) / (arr[2].posH.y - arr[0].posH.y);
		V2f newEdge = V2f::lerp(arr[2], arr[0], factor);
		upTriangle(arr[1], newEdge, arr[2], buffer, shader);
		downTriangle(arr[1], newEdge, arr[0], buffer, shader);
	}
}

void Scanline::upTriangle(const V2f& v1, const V2f& v2, const V2f& v3, FrameBuffer& buffer, Shader& shader)
{
	V2f left, right, top;
	left = v1.posH.x > v2.posH.x ? v2 : v1;
	right = v1.posH.x > v2.posH.x ? v1 : v2;
	top = v3;
	left.posH.x = int(left.posH.x);
	int dy = top.posH.y - left.posH.y;
	int nowY = top.posH.y;

	for (int i = dy; i >= 0; --i) {
		float factor = 0;
		
		if (dy != 0) {
			factor = (float)i / dy;
		}
		V2f newLeft = V2f::lerp(left, top, factor);
		V2f newRight = V2f::lerp(right, top, factor);
		newLeft.posH.x = int(newLeft.posH.x);
		newRight.posH.x = int(newRight.posH.x+0.5);
		newLeft.posH.y = newRight.posH.y = nowY;
		scanLine(newLeft, newRight, buffer, shader);
		nowY--;
	}
}

void Scanline::downTriangle(const V2f& v1, const V2f& v2, const V2f& v3, FrameBuffer& buffer, Shader& shader)
{
	V2f left, right, bottom;
	left = v1.posH.x > v2.posH.x ? v2 : v1;
	right = v1.posH.x > v2.posH.x ? v1 : v2;
	bottom = v3;
	int dy = left.posH.y - bottom.posH.y;
	int nowY = left.posH.y;
	
	for (int i = 0; i < dy; ++i) {
		float weight = 0;
		weight = (float)i / dy;
		V2f newLeft = V2f::lerp(left, bottom, weight);
		V2f newRight = V2f::lerp(right, bottom, weight);
		newLeft.posH.x = int(newLeft.posH.x);
		newRight.posH.x = int(newRight.posH.x+0.5);
		newLeft.posH.y = newRight.posH.y = nowY;
		scanLine(newLeft, newRight, buffer, shader);
		nowY--;
	}
}

void Scanline::scanLine(const V2f& left, const V2f& right,FrameBuffer& buffer,Shader& shader)
{
	int length = right.posH.x - left.posH.x;
	for (int i = 0; i < length; i++) {
		V2f v = V2f::lerp(left, right, (float)i / length);
		v.posH.x = left.posH.x + i;
		v.posH.y = left.posH.y;

		if (buffer.depthCmp(v.posH.x, v.posH.y, v.posH.z)) {
			if (!buffer.depthOnly) {
				//correct the persperctive distortion
				v.posW /= v.inverseZ;
				v.normal /= v.inverseZ;
				v.color /= v.inverseZ;
				v.texcoord /= v.inverseZ;
				buffer.writePoint(v.posH.x, v.posH.y, shader.FS(v));
			}
		}
	}
}

void halfSpace::halfSpaceTriangle(const V2f& v1, const V2f& v2, const V2f& v3, FrameBuffer& buffer, Shader& shader)
{
	glm::vec3 a = glm::vec3(v1.posH.x, v1.posH.y, v1.posH.z);
	glm::vec3 b = glm::vec3(v2.posH.x, v2.posH.y, v2.posH.z);
	glm::vec3 c = glm::vec3(v3.posH.x, v3.posH.y, v3.posH.z);

	int xmin = floor(std::min(a.x, std::min(b.x, c.x)));
	int xmax = floor(std::max(a.x, std::max(b.x, c.x)));
	int ymin = floor(std::min(a.y, std::min(b.y, c.y)));
	int ymax = floor(std::max(a.y, std::max(b.y, c.y)));


	for (int x = xmin; x <= xmax; x++)
		for (int y = ymin; y <= ymax; y++) {
			float px = x + 0.5;
			float py = y + 0.5;
			glm::vec3 p(px, py, 0);
			if (!halfSpaceTest(a, b, c, p))
				continue;
			std::tuple<float, float, float>bc = calBarycentricCoord(a, b, c, p);
			float alpha = std::get<0>(bc);
			float beta = std::get<1>(bc);
			float gamma = std::get<2>(bc);
			
			V2f v = V2f::BarycentricLerp(v1, v2, v3, alpha, beta, gamma);
	
			v.posH.x = int(v.posH.x);
			v.posH.y = int(v.posH.y);
			if (buffer.depthCmp(v.posH.x, v.posH.y, v.posH.z)) {
				if (!buffer.depthOnly) {
					float Z = 1.0f / v.inverseZ;       //correct the persperctive distortion
					v.posW *= Z;
					v.normal *= Z;
					v.color *= Z;
					v.texcoord *= Z;
					buffer.writePoint(v.posH.x, v.posH.y, shader.FS(v));
				}
			}
		}


}

std::tuple<float, float, float> halfSpace::calBarycentricCoord(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p)
{
	float gamma = ((a.y - b.y) * p.x + (b.x - a.x) * p.y + a.x * b.y - b.x * a.y) / ((a.y - b.y) * c.x + (b.x - a.x) * c.y + a.x * b.y - b.x * a.y);
	float beta = ((a.y - c.y) * p.x + (c.x - a.x) * p.y + a.x * c.y - c.x * a.y) / ((a.y - c.y) * b.x + (c.x - a.x) * b.y + a.x * c.y - c.x * a.y);
	float alpha = 1 - beta - gamma;

	return std::tuple<float, float, float>(alpha, beta, gamma);
}

bool halfSpace::halfSpaceTest(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& p)
{

	glm::vec3 ab = b - a;
	glm::vec3 bc = c - b;
	glm::vec3 ca = a - c;
	glm::vec3 ap = p - a;
	glm::vec3 bp = p - b;
	glm::vec3 cp = p - c;

	float z1 = cross(ab, ap).z;
	float z2 = cross(bc, bp).z;
	float z3 = cross(ca, cp).z;

	return z1 >= 0 && z2 >= 0 && z3 >= 0;
}

std::vector<V2f> CVVclip::sutherlandHodgeman(const V2f& v1, const V2f& v2, const V2f& v3)
{
	std::vector<V2f> out = { v1,v2,v3 };
	if (allVertexInside(out)) {
		return out;
	}
	for (const glm::vec4& line : viewLines) {
		std::vector<V2f> in(out);
		out.clear();
		for (int j = 0; j < in.size(); j++) {
			V2f cur = in[j];
			V2f beforeCur = in[(j + in.size() - 1) % in.size()];
			if (Inside(line, cur.posH)) {
				if (!Inside(line, beforeCur.posH)) {
					V2f intersetion = Intersect(beforeCur, cur, line);
					out.push_back(intersetion);
				}
				out.push_back(cur);
			}
			else if (Inside(line, beforeCur.posH)) {
				V2f intersection = Intersect(beforeCur, cur, line);
				out.push_back(intersection);
			}
		}
	}
	return out;
}

void RasterEngine::drawTriangleScanLine(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	V2f o1 = shader->VS(v1);
	V2f o2 = shader->VS(v2);
	V2f o3 = shader->VS(v3);

	GeometryStage::toNDC(o1.posH);
	GeometryStage::toNDC(o2.posH);
	GeometryStage::toNDC(o3.posH);

	o1.posH = viewPort * o1.posH;
	o2.posH = viewPort * o2.posH;
	o3.posH = viewPort * o3.posH;

	Scanline::scanLineTriangle(o1, o2, o3, *buffer, *shader);
}

void RasterEngine::drawMeshScanLine(const Mesh& mesh)
{
	if (mesh.indexBuffer.empty())
		return;
	for (int i = 0; i < mesh.indexBuffer.size(); i += 3) {
		Vertex v1 = mesh.vertexBuffer[mesh.indexBuffer[i]];
		Vertex v2 = mesh.vertexBuffer[mesh.indexBuffer[i + 1]];
		Vertex v3 = mesh.vertexBuffer[mesh.indexBuffer[i + 2]];

		V2f o1 = shader->VS(v1);
		V2f o2 = shader->VS(v2);
		V2f o3 = shader->VS(v3);

		std::vector<V2f> clipingVertexs = CVVclip::sutherlandHodgeman(o1, o2, o3);

		for (int j = 0; j < clipingVertexs.size(); ++j) {
			CVVclip::perspectiveDivision(clipingVertexs[j]);
		}
		int n = clipingVertexs.size() - 3 + 1;
		for (int j = 0; j < n; j++) {
			o1 = clipingVertexs[0];
			o2 = clipingVertexs[j + 1];
			o3 = clipingVertexs[j + 2];

			o1.posH = viewPort * o1.posH;
			o2.posH = viewPort * o2.posH;
			o3.posH = viewPort * o3.posH;

			if (GeometryStage::FaceCulling(o1.posH, o2.posH, o3.posH)) {
				continue;
			}

			Scanline::scanLineTriangle(o1, o2, o3, *buffer, *shader);
		}
	}
}

void RasterEngine::drawMeshHalfSpace(const Mesh& mesh)
{
	if (mesh.indexBuffer.empty())
		return;
	for (int i = 0; i < mesh.indexBuffer.size(); i += 3) {
		Vertex v1 = mesh.vertexBuffer[mesh.indexBuffer[i]];
		Vertex v2 = mesh.vertexBuffer[mesh.indexBuffer[i + 1]];
		Vertex v3 = mesh.vertexBuffer[mesh.indexBuffer[i + 2]];

		V2f o1 = shader->VS(v1);
		V2f o2 = shader->VS(v2);
		V2f o3 = shader->VS(v3);



		std::vector<V2f> clipingVertexs = CVVclip::sutherlandHodgeman(o1, o2, o3);

		for (int j = 0; j < clipingVertexs.size(); ++j) {
			CVVclip::perspectiveDivision(clipingVertexs[j]);
		}
		int n = clipingVertexs.size() - 3 + 1;
		for (int j = 0; j < n; j++) {
			o1 = clipingVertexs[0];
			o2 = clipingVertexs[j + 1];
			o3 = clipingVertexs[j + 2];

			o1.posH = viewPort * o1.posH;
			o2.posH = viewPort * o2.posH;
			o3.posH = viewPort * o3.posH;

			if (GeometryStage::FaceCulling(o1.posH, o2.posH, o3.posH)) {
				continue;
			}

			halfSpace::halfSpaceTriangle(o1, o2, o3, *buffer, *shader);
		}
	}
}

void RasterEngine::update(int w, int h)
{
	if (w == m_width && h == m_height)
		return;
	m_width = w;
	m_height = h;
	viewPort = GeometryStage::getViewPortMatrix(0, 0, m_width, m_height);
}
