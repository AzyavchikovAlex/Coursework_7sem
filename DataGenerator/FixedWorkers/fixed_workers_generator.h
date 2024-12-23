#pragma once

#include <filesystem>

#include "DataGenerator/generator.h"
#include "Model/test_case.h"
#include "Utils/Random/random_event_generator.h"

class TasksAfterWorkersGenerator : public TestsGenerator {
 public:
  TasksAfterWorkersGenerator() = delete;
  TasksAfterWorkersGenerator(WorkerType max_worker_type,
                             TaskTypeId max_task_type,
                             size_t workers_count,
                             const std::vector<WorkerAbilities>& worker_types_abilities,
                             const std::vector<Probability>& task_types_probabilities,
                             const std::vector<Probability>& worker_types_probabilities);

  TestCase Generate() override;

  static std::shared_ptr<TasksAfterWorkersGenerator> CreateGenerator(const std::filesystem::path& filepath,
                                                                     size_t test_size);

  WorkerType GetMaxWorkerType() const;
  TaskTypeId GetMaxTaskTypeId() const;

 private:
  std::vector<WorkerAbilities> worker_types_abilities_;
  std::vector<Probability> task_types_probabilities_;
  RandomEventGenerator tasks_generator_;
  RandomEventGenerator workers_generator_;
  WorkerType max_worker_type_;
  TaskTypeId max_task_type_;
  size_t workers_count_;

  Worker GenerateWorker(WorkerId id) const;
  Task GenerateTask(TaskId id) const;
  std::vector<Query> GeneratePreparationQueries() const;
};