#pragma once

#include <memory>

#include "Model/scheduler.h"

class Action {
 public:
  Action() = default;
  virtual ~Action() = default;
  virtual void operator()(const SchedulerPtr& scheduler) = 0;
};

typedef std::shared_ptr<Action> ActionPtr;