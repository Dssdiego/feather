//
// Created by Diego S. Seabra on 31/01/23
//

#include "TweenPanel.h"

#include <imgui.h>
#include <imgui/IconsFontAwesome6.h>
#include <tweeny/tweeny.h>
#include "../../animation/tweening/Tweener.h"

namespace Feather
{
	void TweenPanel::Draw()
	{
		ImGui::Begin(ICON_FA_ANGLES_RIGHT " Tween");

		ImGui::InputFloat2("FromTo", (float*)&tweenFromTo);
		ImGui::InputInt("DurationSec", &tweenDuration);

		if (ImGui::Button("Execute"))
		{
			auto tween = tweeny::from(tweenFromTo.x).to(tweenFromTo.y).during(tweenDuration);
			//while (tween.progress() < 1) {
			//    tween.step((float) Time::Delta);
			//    std::cout << tween.progress() << std::endl;
			//    //std::cout << tween.peek() << std::endl;
			//}
			//Tween tween = Tween(tweenFromTo.x, tweenFromTo.y, tweenDuration, TweenEasing::IN_OUT, TweenEquation::LINEAR);
			Tweener::AddTween(tween);
			//tween.Execute();
			//auto cards = Solitaire::GetCards();
			//cards[0].MoveAnimate();
		}

		ImGui::End();
	}
}

