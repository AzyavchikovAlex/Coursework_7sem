#pragma once

#include <vector>
#include <memory>
#include <optional>
#include <functional>

#include "worker.h"
#include "task.h"

typedef std::function<void(Worker)> WorkerCallback;

class SchedulerInterface {
 public:
  SchedulerInterface() = delete;
  virtual ~SchedulerInterface() = default;
  SchedulerInterface(WorkerType max_worker_type, TaskTypeId max_task_type)
      : max_task_type_(max_task_type), max_worker_type_(max_worker_type) {
  }
  virtual void AddWorkerTypeInfo(WorkerType type,
                                 WorkerAbilities abilities) = 0;
  virtual void AddWorker(Worker worker) = 0;
  virtual void AddTask(Task task,
                       std::function<void(Worker)> onScheduledCallback) = 0;

  virtual void UpdateSelfTime(Time time) = 0;

 protected:
  const WorkerType max_worker_type_;
  const TaskTypeId max_task_type_;
};

typedef std::shared_ptr<SchedulerInterface> SchedulerPtr;