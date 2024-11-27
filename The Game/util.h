#pragma once



inline float radians(float degree) {
	return (degree * 3.1415f) / 180.0f;
}

inline float degrees(float radians) {
	return (radians * 180.0f) / 3.1415f;
}