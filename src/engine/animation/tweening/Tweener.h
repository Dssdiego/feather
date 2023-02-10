//
// Created by Diego S. Seabra on 09/01/23
//

#include "../../core/Time.h"
#include <vector>
#include <tweeny/tweeny.h>

#ifndef FEATHER_TWEENER_H
#define FEATHER_TWEENER_H

namespace Feather
{
	class Tweener
	{
	public:
		static void AddTween(tweeny::tween<float> tween);
		static void Update();

	private:
		inline static std::vector<tweeny::tween<float>> tweens{};
	};
}

#endif //FEATHER_TWEENER_H
