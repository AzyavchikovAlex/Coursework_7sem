#pragma once

#include <memory>
#include <functional>

#include "scheduler.h"
#include "action.h"

struct Query {
  ActionPtr action;
  Time time;
};
