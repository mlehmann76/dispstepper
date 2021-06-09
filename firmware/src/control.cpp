#include "control.h"
#include "stepper.h"

control::control(stepCtrl *_s) : m_stepper(_s) {}

//
void control::run() {}
//
void control::onModeChange(viewMode) {}
template <> void control::onButtonChange(uint8_t, bool) {}