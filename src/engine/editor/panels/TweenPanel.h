//
// Created by Diego S. Seabra on 31/01/23
//

#ifndef FEATHER_TWEEN_PANEL_H
#define FEATHER_TWEEN_PANEL_H

#include <glm/glm.hpp>

namespace Feather
{
	class TweenPanel
	{
	public:
		void Draw();

	private:
		glm::vec2 tweenFromTo = glm::vec2(0.f, 100.f);
		int tweenDuration = 0;
	};
}

#endif //FEATHER_TWEEN_PANEL_H
