#include "first_fail.h"

#include <cassert>

#include "Schedulers/Actions/add_task.h"

namespace FirstFail {

Percent Measure(const SchedulerPtr& scheduler, const TestCase& test) {
  size_t tasks_count = 0;
  for (auto& q : test.queries) {
    if (auto ptr = dynamic_cast<AddTaskAction*>(q.action.get()); ptr
        != nullptr) {
      ++tasks_count;
    }
  }
  size_t finished_tasks_count = 0;
  for (auto& q : test.queries) {
    bool is_task_query = false;
    bool is_finished = false;
    if (auto ptr = dynamic_cast<AddTaskAction*>(q.action.get()); ptr
        != nullptr) {
      is_task_query = true;
      ptr->SetCallback([&is_finished](Worker /*worker*/) {
        is_finished = true;
      });
    }
    q.action->operator()(scheduler);

    if (is_task_query) {
      dynamic_cast<AddTaskAction*>(q.action.get())->SetCallback([](Worker /*worker*/) {});
      if (!is_finished) {
        break;
      } else {
        ++finished_tasks_count;
      }
    }
  }
  assert(tasks_count > 0);
  return static_cast<Percent>(finished_tasks_count)
      / static_cast<Percent>(tasks_count);
}

} // namespace FirstFail
