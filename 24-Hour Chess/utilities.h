#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

typedef unsigned int texture;

namespace utilities {

	void genTexture(std::string address, unsigned int* texture, bool rgba);

	double RoundAwayFromZero(double x);

	class Vec2 {
	public:
		Vec2(float inX, float inY) {
			x = inX;
			y = inY;
		}

		float x;
		float y;

		void operator+=(Vec2 v2) {
			x += v2.x;
			y += v2.y;
		}

		void operator-=(Vec2 v2) {
			x -= v2.x;
			y -= v2.y;
		}

		void operator*=(float m) {
			x *= m;
			y *= m;
		}

		void operator/=(float m) {
			x /= m;
			y /= m;
		}

		Vec2 operator+(Vec2 v2) {
			float x3 = x + v2.x;
			float y3 = y + v2.y;
			return Vec2(x3, y3);
		}

		Vec2 operator-(Vec2 v2) {
			float x3 = x - v2.x;
			float y3 = y - v2.y;
			return Vec2(x3, y3);
		}

		Vec2 operator*(float m) {
			float x3 = x * m;
			float y3 = y * m;
			return Vec2(x3, y3);
		}

		Vec2 direction() {
			return Vec2(RoundAwayFromZero(x / length()), RoundAwayFromZero(y / length()));
		}

		float length() {
			return (sqrtf(powf(x, 2) + powf(y, 2)));
		}

		void print() {
			std::cout << x << ", " << y << std::endl;
		}
	};
}