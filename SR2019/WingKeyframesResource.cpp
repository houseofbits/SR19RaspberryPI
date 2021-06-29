
#include "Core.h"

WingKeyframesResource::WingKeyframesResource()
{
}


WingKeyframesResource::~WingKeyframesResource()
{
}

bool WingKeyframesResource::ReadFromFile(string name)
{
	sFileData data;

	FILE* fin = fopen(name.c_str(), "rb");

	if (!fin) {
		cout << "File open failed" << endl;
		return false;
	}

	fread(&data.numFrames, sizeof(unsigned int), 1, fin);
	fread(&data.tStart, sizeof(float), 1, fin);
	fread(&data.tEnd, sizeof(float), 1, fin);

	data.keyframes = new sKeyframe[data.numFrames];

	for (unsigned int k = 0; k < data.numFrames; k++) {
		fread(&data.keyframes[k], sizeof(sKeyframe), 1, fin);
	}

	fclose(fin);

	if (keyframes.size() > 0) {
		for (unsigned int i = 0; i < keyframes.size(); i++) {
			delete keyframes[i];
		}
		keyframes.clear();
	}
	for (unsigned int k = 0; k < data.numFrames; k++) {
		WingsKeyframe* kf = new WingsKeyframe();
		kf->t = data.keyframes[k].t;

		for (int w = 0; w < 12; w++) {
			for (int c = 0; c < 7; c++) {
				kf->colors[w][c].x = data.keyframes[k].data[w].colors[c].r;
				kf->colors[w][c].y = data.keyframes[k].data[w].colors[c].g;
				kf->colors[w][c].z = data.keyframes[k].data[w].colors[c].b;
			}
		}
		keyframes.push_back(kf);
	}

	tstart = data.tStart;
	tend = data.tEnd;

	return true;
}

bool WingKeyframesResource::getInterpolatedKeyframe(float t, WingsKeyframe& keyframe)
{
	if (keyframes.size() < 2)return false;

	for (unsigned int i = 0; i < keyframes.size() - 1; i++) {
		if (keyframes[i]->t < t && keyframes[i + 1]->t >= t) {

			float dt = (t - keyframes[i]->t) / (keyframes[i + 1]->t - keyframes[i]->t);

			keyframe = keyframes[i]->getInterpolated(1.0f - dt, *keyframes[i + 1]);

			return true;
		}
	}
	return false;
}