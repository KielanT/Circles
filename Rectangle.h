#pragma once

struct Rectangle
{
	float X;
	float Y;

	float Width;
	float Height;

	Rectangle(float x, float y, float width, float height) : X(x), Y(y), Width(width), Height(height) {}
	Rectangle() = default;
};