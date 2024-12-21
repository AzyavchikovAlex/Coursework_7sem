#pragma once

#include "Model/scheduler.h"
#include <Model/test_case.h>

namespace FirstFail {

typedef long double Percent;

Percent Measure(const SchedulerPtr& scheduler, const TestCase& test);

} // FirstFail
