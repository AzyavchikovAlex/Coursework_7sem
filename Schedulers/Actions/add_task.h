#pragma once

#include "Model/action.h"
#include "Model/scheduler.h"

class AddTaskAction : public Action {
 public:
  explicit AddTaskAction(Task task,
                         WorkerCallback callback = [](Worker /*worker*/) {})
      : task_(task), callback_(std::move(callback)) {}
  ~AddTaskAction() override {
    callback_ = std::move([](Worker) {});
  }

  void operator()(const SchedulerPtr& scheduler) override {
    scheduler->AddTask(task_, callback_);
  }

  void SetCallback(WorkerCallback callback) {
    callback_ = std::move(callback);
  }

 private:
  Task task_;
  WorkerCallback callback_;
};