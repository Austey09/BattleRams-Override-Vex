#pragma once

void chainLift();
void chainLift_reverse();
void chainLift_brake();
void lift();
void roller();

void resetLocalization(float x, float y, float theta);
bool isHeadingNear(float headingDeg, float targetDeg, float tolDeg);
void localizeNow();