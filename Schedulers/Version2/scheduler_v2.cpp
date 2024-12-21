#include <cassert>
#include <ranges>
#include "scheduler_v2.h"

SchedulerV2::SchedulerV2(WorkerType max_worker_type, TaskTypeId max_task_type)
    : SchedulerInterface(max_worker_type, max_task_type),
      type_to_utility_(max_worker_type, 0),
      typed_workers_(max_worker_type, std::vector<Worker>()),
      is_able_(max_worker_type, std::vector<bool>(max_task_type)) {
}

void SchedulerV2::AddWorkerTypeInfo(WorkerType type,
                                    WorkerAbilities abilities) {
  are_types_ordered_ = false;
  WorkerMetrics type_metrics = 0;
  for (auto& ability : abilities) {
    is_able_[type][ability.id] = true;
    type_metrics += ability.probability;
  }
  type_to_utility_[type] = type_metrics;
}

void SchedulerV2::AddWorker(Worker worker) {
  typed_workers_[worker.type].push_back(worker);
}

#include <unordered_set>
void SchedulerV2::AddTask(Task task,
                          std::function<void(Worker)> onScheduledCallback) {
  OrderTypes();
  std::optional<Worker> best_worker;
  for (auto worker_type_it = sorted_types_.begin(); worker_type_it != sorted_types_.end(); ++worker_type_it) {
    auto worker_type = worker_type_it->second;
    auto& workers = typed_workers_[worker_type];
    if (!workers.empty()) {
      if (this->is_able_[worker_type][task.type.id]) {
        best_worker = workers.back();
        workers.pop_back();

        worker_type_it = sorted_types_.erase(worker_type_it);
        if (!workers.empty()) {
          WorkerMetrics metrics = type_to_utility_[worker_type];
          metrics /= workers.size();
          sorted_types_.insert({metrics, worker_type});
        }
        break;
      }
    }
  }
  if (best_worker == std::nullopt) {
    return;
  }
  auto id = best_worker.value().id;
  static std::unordered_set<WorkerId> used;
  if (task.id == 0) {
    used.clear();
  }
  assert(!used.contains(id));
  used.insert(id);
  onScheduledCallback(best_worker.value());
}

void SchedulerV2::UpdateSelfTime(Time /*time*/) {
}

void SchedulerV2::OrderTypes() {
  if (are_types_ordered_) {
    return;
  }
  are_types_ordered_ = true;
  for (WorkerType type = 0; type < max_worker_type_; ++type) {
    auto& workers = typed_workers_[type];
    if (workers.empty()) {
      continue;
    }
    WorkerMetrics metrics = type_to_utility_[type] / static_cast<WorkerMetrics>(workers.size());
    sorted_types_.insert({metrics, type});
  }
}