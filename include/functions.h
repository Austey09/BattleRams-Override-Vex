#pragma once

void intake();
void intake_reverse();
void intake_brake();
void lift();
void arm();
void roller();

void resetLocalization(float x, float y, float theta);
bool isHeadingNear(float headingDeg, float targetDeg, float tolDeg);
void localizeNow();