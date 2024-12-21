#pragma once

#include "Model/scheduler.h"

class SchedulerV3 : public SchedulerInterface {
 public:
  SchedulerV3(WorkerType max_worker_type, TaskTypeId max_task_type);
  ~SchedulerV3() override = default;
  void AddWorkerTypeInfo(WorkerType type, WorkerAbilities abilities) override;
  void AddWorker(Worker worker) override;
  void AddTask(Task task,
               std::function<void(Worker)> onScheduledCallback) override;

  void UpdateSelfTime(Time time) override;

 private:
  std::vector<std::vector<bool>> is_able_; // i - worker_type, j = task_type_id
  std::vector<Worker> workers_{};
  std::vector<TaskType> TaskIdToType_;
  std::vector<Probability> types_metrics_;
};