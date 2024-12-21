#pragma once

#include <utility>

#include "Model/action.h"

class AddWorkerTypeInfoAction : public Action {
 public:
  AddWorkerTypeInfoAction(WorkerType type, WorkerAbilities  abilities)
  : abilities_(std::move(abilities)), type_(type) {}

  void operator()(const SchedulerPtr& scheduler) override {
    scheduler->AddWorkerTypeInfo(type_, abilities_);
  }

 private:
  WorkerAbilities abilities_;
  WorkerType type_;
};