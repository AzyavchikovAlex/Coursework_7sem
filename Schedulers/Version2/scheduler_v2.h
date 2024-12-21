#pragma once

#include <map>
#include <set>

#include "Model/scheduler.h"

// сортируем типы работников по (полезности / количество работников),
// изменяя полезность в зависимости от количества работников
class SchedulerV2 : public SchedulerInterface {
 public:
  SchedulerV2(WorkerType max_worker_type, TaskTypeId max_task_type);
  ~SchedulerV2() override = default;
  void AddWorkerTypeInfo(WorkerType type, WorkerAbilities abilities) override;
  void AddWorker(Worker worker) override;
  void AddTask(Task task,
               std::function<void(Worker)> onScheduledCallback) override;

  void UpdateSelfTime(Time time) override;

 private:
  typedef Probability WorkerMetrics;
  std::vector<Probability> type_to_utility_; // WorkerType -> Sum(Probability)
  std::set<std::pair<WorkerMetrics, TaskTypeId>> sorted_types_{};
  std::vector<std::vector<Worker>> typed_workers_; // TaskTypeId -> {Worker...}
  std::vector<std::vector<bool>> is_able_; // i - worker_type, j = task_type_id
  bool are_types_ordered_{false};
  void OrderTypes();
};