#include "offline_scheduler.h"

#include <cassert>
#include <iostream>

OfflineScheduler::OfflineScheduler(
    WorkerType maxWorkerType,
    TaskTypeId maxTaskType,
    size_t tasksCount
) : SchedulerInterface(maxWorkerType, maxTaskType),
    worker_types_abilities_(maxWorkerType),
    tasks_count_(tasksCount) {
}

void OfflineScheduler::AddWorkerTypeInfo(
    WorkerType type,
    WorkerAbilities abilities
) {
  assert(type < max_worker_type_);
  for (auto ability : abilities) {
    worker_types_abilities_[type].insert(ability.id);
  }
}

void OfflineScheduler::AddWorker(
    Worker worker
) {
  workers_.push_back(worker);
  a_.emplace_back(tasks_count_, infinity_cost_);
  assert(worker.type < max_worker_type_);
}

void OfflineScheduler::AddTask(
    Task task,
    std::function<void(Worker)> onScheduledCallback
) {
  tasks_.push_back(task);
  tasks_callbacks.push_back(onScheduledCallback);
  ++current_task_index_;
  if (current_task_index_ < tasks_count_) {
    return;
  }
  // build a matrix
  for (size_t worker_index = 0; worker_index < workers_.size();
       ++worker_index) {
    auto worker_type = workers_[worker_index].type;
    for (size_t i = 0; i < tasks_count_; ++i) {
      assert(i < tasks_.size());
      TaskTypeId task_type = tasks_[i].type.id;
      if (worker_types_abilities_[worker_type].contains(task_type)) {
        a_[worker_index][i] = 1;
      }
    }
  }

  // преобразования в нужную индексацию
  a_.insert(a_.begin(), std::vector<int64_t>(tasks_count_, infinity_cost_));
  for (auto& row : a_) {
    row.insert(row.begin(), infinity_cost_);
    while (row.size() < a_.size()) {
      row.push_back(infinity_cost_);
    }
  }


  assert(current_task_index_ <= tasks_count_);
  int m = static_cast<int>(a_[0].size()) - 1;
  int n = static_cast<int>(a_.size()) - 1;
  int64_t INF = std::numeric_limits<int64_t>::max();

  std::vector<int64_t> u(n + 1), v(m + 1), p(m + 1, 0), way(m + 1);
  for (int64_t i = 1; i <= n; ++i) {
    p[0] = i;
    int64_t j0 = 0;
    std::vector<int64_t> minv(m + 1, INF);
    std::vector<char> used(m + 1, false);
    do {
      used[j0] = true;
      int64_t i0 = p[j0], delta = INF, j1;
      for (int j = 1; j <= m; ++j)
        if (!used[j]) {
          int64_t cur = a_[i0][j] - u[i0] - v[j];
          if (cur < minv[j])
            minv[j] = cur, way[j] = j0;
          if (minv[j] < delta)
            delta = minv[j], j1 = j;
        }
      if (delta >= INF) {
        return;
      }
      for (int j = 0; j <= m; ++j)
        if (used[j])
          u[p[j]] += delta, v[j] -= delta;
        else
          minv[j] -= delta;
      j0 = j1;
    } while (p[j0] != 0);
    do {
      int64_t j1 = way[j0];
      p[j0] = p[j1];
      j0 = j1;
    } while (j0);
  }

  std::vector<int> ans(n + 1);
  for (int j = 1; j <= m; ++j) {
    ans[p[j]] = j;
  }

  for (size_t i = 1; i < ans.size(); ++i) {
    int task_index = ans[i] - 1;
    if (task_index >= tasks_count_) {
      continue;
    }
    assert(task_index < tasks_count_);
    int worker_index = static_cast<int>(i) - 1;
    assert(worker_index < workers_.size());
    if (a_[worker_index + 1][task_index + 1] < infinity_cost_) {
      tasks_callbacks[task_index](workers_[worker_index]);
    }
  }
}

void OfflineScheduler::UpdateSelfTime(Time /*time*/) {
  // ignore
}
