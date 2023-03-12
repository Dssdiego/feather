//
// Created by Diego S. Seabra on 12/03/23
//

#include <CommandLineTestRunner.h>
#include <TestHarness.h>

#include "../../feather/src/engine/common/Rect.h"

using namespace Feather;

TEST_GROUP(RectGroup) { };

TEST(RectGroup, Rect_GetWidth)
{
    Rect r = Rect(0.f, 0.f, 1.f, 1.f);

    CHECK_EQUAL(r.GetWidth(), 1.f);
}

TEST(RectGroup, Rect_GetHeight)
{
    Rect r = Rect(0.f, 0.f, 1.f, 2.f);

    CHECK_EQUAL(r.GetHeight(), 2.f);
}

TEST(RectGroup, Rect_GetOrigin)
{
    Rect r = Rect(3.2f, 1.4f, 3.f, 7.f);

    CHECK_EQUAL(r.GetOrigin().x, 3.2f);
    CHECK_EQUAL(r.GetOrigin().y, 1.4f);
}

TEST(RectGroup, Rect_GetCenter)
{
    Rect r1 = Rect(0.f, 0.f, 3.f, 7.f);
    Rect r2 = Rect(2.5f, 2.5f, 4.f, 4.f);

    CHECK_EQUAL(r1.GetCenter().x, 1.5f);
    CHECK_EQUAL(r1.GetCenter().y, 3.5f);
    CHECK_EQUAL(r2.GetCenter().x, 4.5f);
    CHECK_EQUAL(r2.GetCenter().y, 4.5f);
}

TEST(RectGroup, Rect_GetSize)
{
    Rect r = Rect(3.2f, 1.4f, 29.f, 30.f);

    CHECK_EQUAL(r.GetSize().x, 29.f);
    CHECK_EQUAL(r.GetSize().y, 30.f);
}

TEST(RectGroup, Rect_Inside)
{
    Rect r1 = Rect(glm::vec2(0.5f, 0.5f), 2.f, 2.f);
    Rect r2 = Rect(glm::vec2(0.f, 0.f), 3.f, 3.f);
    Rect r3 = Rect(glm::vec2(0.f, 0.f), 3.f, 3.f);

    CHECK_EQUAL(Rect::IsInside(r1, r2), true);
    CHECK_EQUAL(Rect::IsInside(r2, r1), false);
    CHECK_EQUAL(Rect::IsInside(r3, r2), false);
}

TEST(RectGroup, Rect_Equal)
{
    Rect r1 = Rect(glm::vec2(0.f, 0.f), 3.f, 3.f);
    Rect r2 = Rect(glm::vec2(0.f, 0.f), 3.f, 3.f);

    CHECK_EQUAL(Rect::IsEqual(r1, r2), true);
}

TEST(RectGroup, Rect_Intersection)
{
    Rect r1 = Rect(glm::vec2(0.f, 0.f), 1.f, 1.f);
    Rect r2 = Rect(glm::vec2(0.5f, 0.5f), 1.f, 1.f);
    Rect r3 = Rect(glm::vec2(0.f, 0.f), 3.f, 3.f);
    Rect r4 = Rect(glm::vec2(2.9f, 2.9f), 0.5f, 0.5f);
    Rect r5 = Rect(glm::vec2(1.2f, 1.2f), 1.6f, 1.6f);

    CHECK_EQUAL(Rect::IsIntersecting(r1, r2), true);
    CHECK_EQUAL(Rect::IsIntersecting(r1, r3), true);
    CHECK_EQUAL(Rect::IsIntersecting(r2, r3), true);
    CHECK_EQUAL(Rect::IsIntersecting(r4, r3), true);
    CHECK_EQUAL(Rect::IsIntersecting(r3, r1), true);
    CHECK_EQUAL(Rect::IsIntersecting(r3, r2), true);
    CHECK_EQUAL(Rect::IsIntersecting(r2, r5), true);
    CHECK_EQUAL(Rect::IsIntersecting(r5, r2), true);
    CHECK_EQUAL(Rect::IsIntersecting(r5, r3), true);

    CHECK_EQUAL(Rect::IsIntersecting(r1, r4), false);
    CHECK_EQUAL(Rect::IsIntersecting(r2, r4), false);
    CHECK_EQUAL(Rect::IsIntersecting(r4, r2), false);
    CHECK_EQUAL(Rect::IsIntersecting(r4, r1), false);
    CHECK_EQUAL(Rect::IsIntersecting(r5, r1), false);
    CHECK_EQUAL(Rect::IsIntersecting(r5, r4), false);
}
