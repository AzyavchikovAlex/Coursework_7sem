#pragma once

#include "Model/scheduler.h"
#include "Model/action.h"

class AddWorkerAction : public Action {
 public:
  explicit AddWorkerAction(Worker worker) : worker_(worker) {}

  void operator()(const SchedulerPtr& scheduler) override {
    scheduler->AddWorker(worker_);
  }

 private:
  Worker worker_;
};

