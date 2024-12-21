#include "mock_scheduler.h"

#include <cassert>

#include "Schedulers/Actions/add_worker_type_info.h"
#include "Schedulers/Actions/add_worker.h"
#include "Schedulers/Actions/add_task.h"

MockScheduler::MockScheduler(WorkerType max_worker_type,
                             TaskTypeId max_task_type,
                             size_t queries_count,
                             std::ofstream&& out)
    : SchedulerInterface(max_worker_type, max_task_type),
      out_(std::move(out)),
      queries_count_(queries_count) {
  out_ << queries_count << "\n";
  out_ << max_worker_type << " " << max_task_type << "\n";
}

MockScheduler::~MockScheduler() {
  out_.flush();
  out_.close();
}

void MockScheduler::AddWorkerTypeInfo(WorkerType type,
                                      WorkerAbilities abilities) {
  ++query_index_;
  assert(query_index_ <= queries_count_);
  out_ << "type" << "\t" << type << "\n";
  out_ << abilities.size() << "\n";
  for (auto a : abilities) {
    out_ << a.id << " " << a.probability << "\n";
  }
  out_ << "\n";
}

void MockScheduler::AddWorker(Worker worker) {
  ++query_index_;
  assert(query_index_ <= queries_count_);
  out_ << "worker\t" << worker.type << "\n";
}

void MockScheduler::AddTask(Task task,
                            std::function<void(Worker)> /*onScheduledCallback*/) {
  ++query_index_;
  assert(query_index_ <= queries_count_);
  out_ << "task\t" << task.type.id << " " << task.type.probability << "\n";
}

void MockScheduler::UpdateSelfTime(Time /*time*/) {
}

TestCase MockScheduler::ParseTestCase(std::ifstream in) {
  TestCase test;
  size_t queries_count;
  in >> queries_count;
  in >> test.max_worker_type >> test.max_task_type;
  test.queries.reserve(queries_count);
  WorkerId worker_id = 0;
  TaskId task_id = 0;
  std::string query_type;
  for (size_t i = 0; i < queries_count; ++i) {
    in >> query_type;
    if (query_type == "worker") {
      WorkerType type;
      in >> type;
      test.queries.push_back(Query{
          std::make_shared<AddWorkerAction>(Worker{type, worker_id++})});
    } else if (query_type == "task") {
      TaskType type{};
      in >> type.id;
      in >> type.probability;
      test.queries.push_back(Query{
          std::make_shared<AddTaskAction>(Task{type, task_id++})});
    } else if (query_type == "type") {
      WorkerType type;
      in >> type;
      size_t abilities_count;
      in >> abilities_count;
      WorkerAbilities abilities(abilities_count);
      for (auto& e : abilities) {
        in >> e.id >> e.probability;
      }
      test.queries.push_back(Query{
          std::make_shared<AddWorkerTypeInfoAction>(type, abilities)});
    } else {
      assert(false);
    }
  }
  return test;
}
