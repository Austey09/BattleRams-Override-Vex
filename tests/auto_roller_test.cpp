#include "auto_roller.hpp"
#include <cassert>
#include <limits>
#include <iostream>

int main() {
    using namespace auto_roller;
    Control c;
    auto step = [&](bool down, bool manual, double hue, std::uint32_t time,
                    double saturation = 0.8, int proximity = 150) {
        return c.update(down, manual, hue, saturation, proximity, time);
    };
    assert(step(false, false, 60, 0) == 0);
    assert(step(false, true, 60, 0) == -127);
    assert(step(true, false, 60, 0) == 0 && c.enabled());
    assert(step(false, false, 60, 99) == 0);
    assert(step(false, false, 60, 100) == AUTO_POWER);
    assert(step(false, true, 220, 130) == AUTO_POWER); // Auto owns roller.
    assert(step(false, true, 10, 160) == 0 && c.enabled());
    assert(step(false, false, 350, 190) == 0);
    assert(step(false, false, 220, 200) == 0);
    assert(step(false, false, 220, 300) == AUTO_POWER); // Resumes automatically.
    assert(step(false, false, 220, 310, 0.8, 0) == 0); // No nearby object.
    assert(step(false, false, 220, 320, 0.1) == 0);
    assert(step(false, false, std::numeric_limits<double>::infinity(), 330) == 0);
    assert(step(false, false, 220, 340, 0.8, 2147483647) == 0);
    assert(step(false, false, 220, 350) == 0);
    assert(step(false, false, 220, 450) == AUTO_POWER);
    assert(step(true, true, 220, 460) == 0 && !c.enabled());
    assert(step(false, true, 220, 490) == -127);
    c.reset();
    assert(!c.enabled());
    assert(step(true, false, 60, 0xfffffff0U) == 0);
    assert(step(false, false, 60, 0x54U) == AUTO_POWER); // Clock rollover.
    c.reset();
    assert(c.update(true, false, 220, 0.8, 150, 0, Color::Blue) == 0);
    assert(c.update(false, false, 60, 0.8, 150, 10, Color::Blue) == 0);
    assert(c.update(false, false, 60, 0.8, 150, 110, Color::Blue) == AUTO_POWER);
    assert(c.update(false, false, 220, 0.8, 150, 140, Color::Blue) == 0);
    assert(isTargetHue(60, Color::Yellow));
    assert(!isTargetHue(60, Color::Red));
    std::cout << "PASS: toggle, manual control, all colors, continuous correction, "
                 "restart filtering, invalid readings, reset, and clock rollover\n";
}
