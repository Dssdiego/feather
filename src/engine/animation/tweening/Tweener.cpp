//
// Created by Diego S. Seabra on 09/01/23
//

#include "Tweener.h"
#include <iostream>

namespace Feather
{
	void Tweener::AddTween(tweeny::tween<float> tween)
	{
		tweens.push_back(tween);
	}

	void Tweener::Update()
	{
		for (auto& t : tweens)
		{
			if (t.progress() < 1)
			{
				t.step((float)Time::Delta);
				std::cout << t.peek() << std::endl;
			}
		}
	}
}
