
#include "Color.h"

Color Red = Color(1.0f, 0.0f, 0.0f);
Color Green = Color(0.0f, 1.0f, 0.0f);
Color Blue = Color(0.0f, 0.0f, 1.0f);
Color Black = Color(0.0f, 0.0f, 0.0f);
Color White = Color(1.0f, 1.0f, 1.0f);

// New colors
Color Cyan = Color(0.0f, 1.0f, 1.0f);
Color Yellow = Color(1.0f, 1.0f, 0.0f);
Color Magenta = Color(1.0f, 0.0f, 1.0f);
Color Orange = Color(0.88f, 0.49f, 0.06f);
Color Grey = Color(0.5f, 0.5f, 0.5f);

struct Color GetColor(int color)
{
	switch (color)
	{
	case RED:
		return Red;
		break;

	case GREEN:
		return Green;
		break;

	case BLUE:
		return Blue;
		break;

	case BLACK:
		return Black;
		break;

	case WHITE:
		return White;
		break;

	case CYAN:
		return Cyan;
		break;

	case YELLOW:
		return Yellow;
		break;

	case MAGENTA:
		return Magenta;
		break;

	case ORANGE:
		return Orange;
		break;

	case GREY:
		return Grey;
		break;

	}
};