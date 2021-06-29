#pragma once

class WingKeyframesResource {
public:

	WingKeyframesResource();
	~WingKeyframesResource();

	typedef struct {
		float r;
		float g;
		float b;
	}sColor;
	typedef struct {
		sColor colors[7];
	}sWing;
	typedef struct {
		float t;
		sWing data[12];
	}sKeyframe;

	typedef struct {
		unsigned int	numFrames;
		float			tStart;
		float			tEnd;
		sKeyframe* keyframes;
	}sFileData;

	vector<WingsKeyframe*>	keyframes;

	bool ReadFromFile(string);

	bool getInterpolatedKeyframe(float t, WingsKeyframe& keyframe);

	float tstart;
	float tend;
};
