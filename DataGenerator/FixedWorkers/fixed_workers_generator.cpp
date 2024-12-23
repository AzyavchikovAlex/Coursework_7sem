#pragma once

#include <cassert>
#include <utility>
#include <iostream>
#include <fstream>

#include "fixed_workers_generator.h"
#include "Schedulers/Offline/offline_scheduler.h"
#include "Schedulers/Actions/add_worker_type_info.h"
#include "Schedulers/Actions/add_worker.h"
#include "Schedulers/Actions/add_task.h"

TasksAfterWorkersGenerator::TasksAfterWorkersGenerator(
    WorkerType max_worker_type,
    TaskTypeId max_task_type,
    size_t workers_count,
    const std::vector<WorkerAbilities>& worker_types_abilities,
    const std::vector<Probability>& task_types_probabilities,
    const std::vector<Probability>& worker_types_probabilities
) : worker_types_abilities_(worker_types_abilities),
    task_types_probabilities_(task_types_probabilities),
    tasks_generator_(task_types_probabilities),
    workers_generator_(worker_types_probabilities),
    max_worker_type_(max_worker_type),
    max_task_type_(max_task_type),
    workers_count_(workers_count) {
  assert(worker_types_abilities_.size() ==
      static_cast<size_t>(max_worker_type_));
  assert(task_types_probabilities_.size() ==
      static_cast<size_t>(max_task_type_));
  assert(worker_types_abilities_.size() ==
      static_cast<size_t>(max_worker_type_));
}

TestCase TasksAfterWorkersGenerator::Generate() {
  // TODO: set real time
  // add worker types info
  std::vector<Query> preparation_queries(GeneratePreparationQueries());

  // add workers
  std::vector<Query> worker_queries;
  for (size_t i = 0; i < workers_count_; ++i) {
    worker_queries.push_back(Query{
        std::make_shared<AddWorkerAction>(GenerateWorker(static_cast<WorkerId>(i))),
        Time{}
    });
  }

  // add tasks
  std::vector<Query> task_queries;
  size_t finished_tasks_count = 0;
  std::vector<WorkerId> executors(workers_count_);
  for (size_t i = 0; i < workers_count_; ++i) {
    auto id = static_cast<TaskId>(i);
    task_queries.push_back(Query{
        std::make_shared<AddTaskAction>(GenerateTask(id),
                                        [&finished_tasks_count, &executors, id](
                                            Worker worker) {
                                          ++finished_tasks_count;
                                          executors[id] = worker.id;
                                        }),
        Time{}
    });
  }

  auto is_feasible_sequence = [&](size_t using_workers_count) {
    finished_tasks_count = 0;
    SchedulerPtr offline_scheduler = std::make_shared<OfflineScheduler>(
        max_worker_type_,
        max_task_type_,
        task_queries.size());
    for (auto& q : preparation_queries) {
      q.action->operator()(offline_scheduler);
    }
    assert(using_workers_count <= worker_queries.size());
    for (size_t i = 0; i < using_workers_count; ++i) {
      worker_queries[i].action->operator()(offline_scheduler);
    }
    for (auto& q : task_queries) {
      q.action->operator()(offline_scheduler);
    }
    return finished_tasks_count == static_cast<int>(task_queries.size());
  };

  size_t l = 0; // impossible
  size_t r = workers_count_; // possible
  while (!is_feasible_sequence(r)) {
    r <<= 1;
    if (r > workers_count_ * 8) {
      std::cerr << r << "- too many workers\n";
    }
    // assert(r <= workers_count_ * 8); // 4000
    while (worker_queries.size() < r) {
      worker_queries.push_back(Query{
          std::make_shared<AddWorkerAction>(GenerateWorker(static_cast<WorkerId>(worker_queries.size()))),
          Time{}});
    }
  }
  while (l + 1 < r) {
    size_t mid = (r + l) >> 1;
    if (is_feasible_sequence(mid)) {
      r = mid;
    } else {
      l = mid;
    }
  }

  // clear redundant workers
  std::iota(executors.begin(), executors.end(), -1);
  finished_tasks_count = 0;
  is_feasible_sequence(r);
  std::unordered_set<WorkerId> used_workers;
  for (WorkerId id : executors) {
    used_workers.insert(id);
  }
  size_t insert_pos = 0;
  for (size_t i = 0; i < worker_queries.size(); ++i) {
    if (used_workers.contains(static_cast<WorkerId>(i))) {
      worker_queries[insert_pos++] = worker_queries[i];
    }
  }

  for (const auto& q : worker_queries) {
    if (auto action = dynamic_cast<AddTaskAction*>(q.action.get());
        action != nullptr) {
      action->SetCallback([](Worker) {});
    }
  }
  worker_queries.resize(insert_pos);
  assert(task_queries.size() == worker_queries.size());

  TestCase test;
  std::for_each(preparation_queries.begin(),
                preparation_queries.end(),
                [&](const Query& q) {
                  test.queries.push_back(q);
                });
  std::for_each(worker_queries.begin(),
                worker_queries.end(),
                [&](const Query& q) {
                  test.queries.push_back(q);
                });
  std::for_each(task_queries.begin(), task_queries.end(), [&](const Query& q) {
    test.queries.push_back(q);
  });
  test.max_worker_type = GetMaxWorkerType();
  test.max_task_type = GetMaxTaskTypeId();

  return test;
}

Worker TasksAfterWorkersGenerator::GenerateWorker(WorkerId id) const {
  return Worker{
      static_cast<WorkerType>(workers_generator_.GetRandomEventIndex()), id};
}

Task TasksAfterWorkersGenerator::GenerateTask(TaskId id) const {
  auto type = static_cast<TaskTypeId>(tasks_generator_.GetRandomEventIndex());
  auto p = task_types_probabilities_[type];
  return Task(TaskType{type, p}, id);
}

std::vector<Query> TasksAfterWorkersGenerator::GeneratePreparationQueries() const {
  std::vector<Query> preparation_queries;
  preparation_queries.reserve(max_worker_type_);
  for (size_t i = 0; i < max_worker_type_; ++i) {
    auto type = static_cast<WorkerType>(i);
    auto abilities = worker_types_abilities_[i];
    preparation_queries.push_back(Query{
        std::make_shared<AddWorkerTypeInfoAction>(type, abilities),
        Time{}
    });
  }
  return preparation_queries;
}

std::shared_ptr<TasksAfterWorkersGenerator> TasksAfterWorkersGenerator::CreateGenerator(
    const std::filesystem::path& filepath,
    size_t test_size) {
  std::ifstream in(filepath);
  if (!in.is_open()) {
    throw std::runtime_error{filepath.string() + " - not found"};
  }

  size_t workers_count = test_size;
  WorkerType max_worker_type;
  in >> max_worker_type;
  std::vector<Probability> workers_probabilities(max_worker_type);
  for (auto& p : workers_probabilities) {
    in >> p;
  }
  TaskTypeId max_task_type;
  in >> max_task_type;
  std::vector<Probability> tasks_probabilities(max_task_type);
  for (auto& p : tasks_probabilities) {
    in >> p;
  }
  std::vector<WorkerAbilities> workers_abilities(max_worker_type);
  for (auto& abilities : workers_abilities) {
    size_t size;
    in >> size;
    for (size_t i = 0; i < size; ++i) {
      TaskType type{};
      in >> type.id;
      type.probability = tasks_probabilities[type.id];
      abilities.push_back(type);
    }
  }
  in.close();
  return std::make_shared<TasksAfterWorkersGenerator>(max_worker_type,
                                                      max_task_type,
                                                      workers_count,
                                                      workers_abilities,
                                                      tasks_probabilities,
                                                      workers_probabilities);
}

WorkerType TasksAfterWorkersGenerator::GetMaxWorkerType() const {
  return max_worker_type_;
}

TaskTypeId TasksAfterWorkersGenerator::GetMaxTaskTypeId() const {
  return max_task_type_;
}
