#include <gtest/gtest.h>

#include "Model/scheduler.h"
#include "offline_scheduler.h"

TEST(OfflineScheduler, Small) {
  {
    WorkerType max_worker_type = 2;
    TaskTypeId max_task_type = 4;
    std::vector<Worker> workers = {
        {0, 0},
        {0, 1},
        {1, 2},
        {1, 3},
    };
    std::vector<WorkerAbilities> worker_types_info = {
        WorkerAbilities{TaskType{0, 1}, TaskType{2, 1}},
        WorkerAbilities{TaskType{1, 1}, TaskType{3, 1}},
    };
    std::vector<Task> tasks = {
        {0, 1, 0},
        {1, 1, 1},
        {2, 1, 2},
        {3, 1, 3},
    };

    SchedulerPtr scheduler = std::make_shared<OfflineScheduler>(max_worker_type,
                                                                max_task_type,
                                                                tasks.size());
    for (size_t i = 0; i < worker_types_info.size(); ++i) {
      scheduler->AddWorkerTypeInfo(static_cast<int64_t>(i),
                                   worker_types_info[i]);
    }
    for (auto worker : workers) {
      scheduler->AddWorker(worker);
    }
    std::vector<WorkerId> ans(tasks.size(), -1);
    for (size_t i = 0; i < tasks.size(); ++i) {
      scheduler->AddTask(tasks[i], [&ans, i](Worker w) {
        ans[i] = w.id;
      });
    }
    EXPECT_TRUE(ans[0] == 0 || ans[0] == 1);
    EXPECT_TRUE(ans[2] == 0 || ans[2] == 1);
    EXPECT_TRUE(ans[0] != ans[2]);

    EXPECT_TRUE(ans[1] == 2 || ans[1] == 3);
    EXPECT_TRUE(ans[3] == 2 || ans[3] == 3);
    EXPECT_TRUE(ans[1] != ans[3]);
    for (auto e : ans) {
      std::cerr << e << "\t";
    }
    std::cerr << "\n";
  }
}