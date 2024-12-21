#pragma once

#include <map>

#include "Model/scheduler.h"

// сортируем типы работников по полезности
class SchedulerV1 : public SchedulerInterface {
 public:
  SchedulerV1(WorkerType max_worker_type, TaskTypeId max_task_type);
  ~SchedulerV1() override = default;
  void AddWorkerTypeInfo(WorkerType type, WorkerAbilities abilities) override;
  void AddWorker(Worker worker) override;
  void AddTask(Task task,
               std::function<void(Worker)> onScheduledCallback) override;

  void UpdateSelfTime(Time time) override;

 private:
  typedef Probability WorkerMetrics;
  std::multimap<WorkerMetrics, TaskTypeId> sorted_types_{};
  std::vector<std::vector<Worker>> typed_workers_; // TaskTypeId -> {Worker...}
  std::vector<std::vector<bool>> is_able_; // i - worker_type, j = task_type_id
};