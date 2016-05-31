#pragma once
#include "cpu/cpu.h"
#include "platform/platform_fiber.h"

namespace coreinit
{
struct OSThread;
}

namespace kernel
{

struct Fiber;

void initialise();
void set_game_name(const std::string& name);

coreinit::OSThread * getCurrentThread();
void exitThreadNoLock();
void queueThreadNoLock(coreinit::OSThread *thread);
void rescheduleNoLock(bool yielding);

}