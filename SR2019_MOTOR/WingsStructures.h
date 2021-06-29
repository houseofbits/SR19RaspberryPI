#pragma once

typedef struct {
	unsigned char g;
	unsigned char r;
	unsigned char b;
}LightColor;
typedef struct {
	LightColor colors[7];
}LightWing;
typedef struct {
	LightColor	colors[7];
	uint8_t		wingId;
}LightWingData;

struct LightDataOutStructure {
	LightWing wingData[12];
};

class Wing {
public:
	Wing() {}
	Vector3 colors[7];
};

class WingsKeyframe {
public:
	WingsKeyframe() {}
	WingsKeyframe getInterpolated(float t, WingsKeyframe& other);
	float t;
	Vector3 colors[12][7];
};

