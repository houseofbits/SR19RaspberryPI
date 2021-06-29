#include "Core.h"

WingsKeyframe WingsKeyframe::getInterpolated(float td, WingsKeyframe& other)
{
	WingsKeyframe k;
	for (int i = 0; i < 12; i++) {
		for (int a = 0; a < 7; a++) {
			k.colors[i][a] = colors[i][a].getInterpolated(other.colors[i][a], td);
		}
	}
	k.t = t + td;
	return k;
}
