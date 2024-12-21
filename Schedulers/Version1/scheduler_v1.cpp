#include <cassert>
#include <ranges>
#include "scheduler_v1.h"

SchedulerV1::SchedulerV1(WorkerType max_worker_type, TaskTypeId max_task_type)
    : SchedulerInterface(max_worker_type, max_task_type),
      typed_workers_(max_worker_type, std::vector<Worker>()),
      is_able_(max_worker_type, std::vector<bool>(max_task_type)) {
}

void SchedulerV1::AddWorkerTypeInfo(WorkerType type,
                                    WorkerAbilities abilities) {
  WorkerMetrics type_metrics = 0;
  for (auto& ability : abilities) {
    is_able_[type][ability.id] = true;
    type_metrics += ability.probability;
  }
  sorted_types_.insert({type_metrics, type});
}

void SchedulerV1::AddWorker(Worker worker) {
  typed_workers_[worker.type].push_back(worker);
}

void SchedulerV1::AddTask(Task task,
                          std::function<void(Worker)> onScheduledCallback) {
  std::optional<Worker> best_worker;
  for (auto & sorted_type : sorted_types_) {
    auto worker_type = sorted_type.second;
    auto& workers = typed_workers_[worker_type];
    if (!workers.empty()) {
      if (this->is_able_[worker_type][task.type.id]) {
        best_worker = workers.back();
        workers.pop_back();
        break;
      }
    }
  }
  if (best_worker == std::nullopt) {
    return;
  }
  onScheduledCallback(best_worker.value());
}

void SchedulerV1::UpdateSelfTime(Time /*time*/) {
}