//
// Created by Diego S. Seabra on 11/09/22.
//

#ifndef FEATHER_TIME_H
#define FEATHER_TIME_H

#include <cstdint>

namespace Feather
{
	class Time
	{
	public:
		inline static void SetDelta(double dt) { Delta = dt; }
		//inline static double Delta() { return delta; }

		inline static double Delta = 0.f;
	};
}

#endif //FEATHER_TIME_H
