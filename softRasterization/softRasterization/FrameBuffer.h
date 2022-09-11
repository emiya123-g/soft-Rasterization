#pragma once
#include<vector>
#include<algorithm>
class FrameBuffer {
public:
	FrameBuffer(int w, int h,bool deepBuffer = false):m_width(w),m_height(h),
		m_colorBuffer(deepBuffer?0:w*h*4,0),m_depthBuffer(w*h,1.0f),depthOnly(deepBuffer) {
		
	}
	void resize(const int &w, const int &h) {
		m_width = w;
		m_height = h;
		if(!depthOnly)
			m_colorBuffer.resize(w*h*4);
		m_depthBuffer.resize(w * h);
		clear();
	}
	void clear() {
		if (!depthOnly)
			std::fill(m_colorBuffer.begin(), m_colorBuffer.end(), 0);
		std::fill(m_depthBuffer.begin(), m_depthBuffer.end(), 1.0f);
	}
	void writePoint(int x,int y,const glm::vec4& color) {
		if(x<0||x>= m_width ||y<0 ||y>=m_height)
			return;
		int pos = 4 * (y * m_width + x);
		m_colorBuffer[pos] = color.r;
		m_colorBuffer[pos + 1] = color.g;
		m_colorBuffer[pos + 2] = color.b;
		m_colorBuffer[pos + 3] = color.a;
	}
	float getDepth(int x, int y) {
		if (x < 0 || x >= m_width || y < 0 || y >= m_height)
			return 1.0f;
		return *(m_depthBuffer.data() + y * m_width + x);
	}
	bool depthCmp(int x, int y, float depth) {
		if (x < 0 || x >= m_width || y < 0 || y >= m_height)
			return false;
		float depthSampler = *(m_depthBuffer.data() + y * m_width + x);
		if (depthSampler > depth) {
			*(m_depthBuffer.data() + y * m_width + x) = depth;
			return true;
		}
		return false;
	}
public:
	bool depthOnly;
	int m_width;
	int m_height;
	std::vector<unsigned char> m_colorBuffer;
	std::vector<float> m_depthBuffer;
};