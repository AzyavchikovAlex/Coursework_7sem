#include <cassert>
#include "scheduler_v3.h"

SchedulerV3::SchedulerV3(WorkerType max_worker_type, TaskTypeId max_task_type)
    : SchedulerInterface(max_worker_type, max_task_type),
      TaskIdToType_(max_task_type_, TaskType{}),
      types_metrics_(max_worker_type, std::numeric_limits<Probability>::max()),
      is_able_(max_worker_type, std::vector<bool>(max_task_type)) {
}

void SchedulerV3::AddWorkerTypeInfo(WorkerType type,
                                    WorkerAbilities abilities) {
  for (auto& ability : abilities) {
    is_able_[type][ability.id] = true;
  }
  assert(type < types_metrics_.size());
  types_metrics_[type] = 0;
  for (auto& ability : abilities) {
    assert(ability.id < TaskIdToType_.size());
    types_metrics_[type] += TaskIdToType_[ability.id].probability;
  }
}

void SchedulerV3::AddWorker(Worker worker) {
  workers_.push_back(worker);
}

void SchedulerV3::AddTask(Task task,
                          std::function<void(Worker)> onScheduledCallback) {
  size_t best_worker_index = workers_.size();
  auto min_metrics = std::numeric_limits<Probability>::max();
  for (size_t i = 0; i < workers_.size(); ++i) {
    auto type = workers_[i].type;
    if (is_able_[type][task.type.id] && types_metrics_[type] <= min_metrics) {
      min_metrics = types_metrics_[type];
      best_worker_index = i;
    }
  }
  if (best_worker_index >= workers_.size()) {
    return;
  }
  Worker using_worker = workers_[best_worker_index];
  std::swap(workers_[best_worker_index], workers_.back());
  workers_.pop_back();
  onScheduledCallback(using_worker);
}

void SchedulerV3::UpdateSelfTime(Time /*time*/) {
}