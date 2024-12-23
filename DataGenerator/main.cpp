#include <iostream>
#include <cassert>
#include <thread>
#include <stack>

#include "Model/scheduler.h"
#include "DataGenerator/FixedWorkers/fixed_workers_generator.h"
#include "Schedulers/Mock/mock_scheduler.h"
#include "DataGenerator/Setups/MetaInfo/utils.h"

#include <iostream>
#include <cassert>
#include <thread>
#include <stack>
#include <queue>

#include "Model/scheduler.h"
#include "DataGenerator/FixedWorkers/fixed_workers_generator.h"
#include "Schedulers/Mock/mock_scheduler.h"
#include "DataGenerator/Setups/MetaInfo/utils.h"
#include "Utils/filesystem.h"

TestCase GenerateTest(const std::filesystem::path& setup_path,
                      size_t test_size) {
  auto gen = TasksAfterWorkersGenerator::CreateGenerator(setup_path, test_size);
  return gen->Generate();
}

void SaveTest(const std::filesystem::path& save_path, const TestCase& test) {
  std::ofstream out(save_path, std::fstream::out | std::fstream::trunc);
  assert(out.is_open());
  SchedulerPtr fake = std::make_shared<MockScheduler>(test.max_worker_type,
                                                      test.max_task_type,
                                                      test.queries.size(),
                                                      std::move(out));
  for (const auto& q : test.queries) {
    q.action->operator()(fake);
  }
  out.flush();
  out.close();
}

void GenerateTestsForAllDirectoriesSetups(const std::filesystem::path& save_path_prefix,
                                          const std::filesystem::path& dir,
                                          size_t tests_size,
                                          size_t tests_count) {

  if (!exists(save_path_prefix)) {
    std::filesystem::create_directory(save_path_prefix);
  }
  std::queue<std::thread> threads;

  auto setups = GetAllDirectoryFiles(dir);
  for (auto& setup : setups) {
    auto save_path = save_path_prefix;
    auto filename = setup.filename().string();
    int pos = static_cast<int>(filename.find(".txt"));
    std::string name(filename.begin(), std::next(filename.begin(), pos));
    save_path /= name;
    if (!exists(save_path)) {
      std::filesystem::create_directory(save_path);
    }

    for (size_t version = 1; version <= tests_count; ++version) {
      threads.emplace([save_path, setup, tests_size, version]() {
        auto test = GenerateTest(setup, tests_size);
        SaveTest(save_path / ("test-v" + std::to_string(version) + ".txt"),
                 test);
      });
    }
  }

  while (!threads.empty()) {
    threads.front().join();
    threads.pop();
  }
}

int main() {
  std::filesystem::path save_prefix =
      "/home/alexazyavchikov/Documents/BSU/Coursework_7sem/Tests/100/EqualProbability";
  std::filesystem::path setup_prefix =
      "/home/alexazyavchikov/Documents/BSU/Coursework_7sem/DataGenerator/Setups/EqualProbability";
  // for (int p = 10; p < 100; p += 10) {
  int p = 90;
  GenerateTestsForAllDirectoriesSetups(
      save_prefix / ("task-appearance=" + std::to_string(p) + "%"),
      setup_prefix / ("task-appearance=" + std::to_string(p) + "%"),
      100,
      20);
  // }


  // // do {
  // size_t first_test_index = 1;
  // if (all_tests_info.contains(info)) {
  //   first_test_index += all_tests_info[info];
  // }
  // std::stack<std::thread> threads;
  // for (size_t i = 0; i < tests_to_generate; ++i) {
  //   threads.emplace([i, first_test_index, info]() {
  //     std::filesystem::path setup_path;
  //     setup_path /= "..";
  //     setup_path /= "DataGenerator";
  //     setup_path /= "Setups";
  //     // if (versions_count == 0) {
  //     setup_path /= info.name + ".txt";
  //     // } else {
  //     //   setup_path /= info.name + "-v" + std::to_string(version) + ".txt";
  //     // }
  //     auto gen =
  //         TasksAfterWorkersGenerator::CreateGenerator(setup_path, info.size);
  //     auto test = gen->Generate();
  //
  //     std::filesystem::path output;
  //     output /= "..";
  //     output /= "Tests";
  //     output /= std::to_string(info.size);
  //     if (!exists(output)) {
  //       std::filesystem::create_directory(output);
  //     }
  //     output /= info.name;
  //     if (!exists(output)) {
  //       std::filesystem::create_directory(output);
  //     }
  //     output /= "test" + std::to_string(i + first_test_index) + ".txt";
  //
  //   });
  // }
  // while (!threads.empty()) {
  //   threads.top().join();
  //   threads.pop();
  // }
  // return 0;
}


// int main() {
//   auto all_tests_info = GetAllTests();
//
//   TestInfo info;
//   std::cout << "Enter setup name:\n";
//   std::cin >> info.name;
//   std::cout << "Enter tests size:\n";
//   std::cin >> info.size;
//   std::cout << "Enter tests count:\n";
//   size_t tests_to_generate = 10;
//   std::cin >> tests_to_generate;
//   // size_t versions_count = 0;
//   // std::cin >> versions_count;
//   // size_t version = 1;
//
//   // do {
//   size_t first_test_index = 1;
//   if (all_tests_info.contains(info)) {
//     first_test_index += all_tests_info[info];
//   }
//   std::stack<std::thread> threads;
//   for (size_t i = 0; i < tests_to_generate; ++i) {
//     threads.emplace([i, first_test_index, info]() {
//       std::filesystem::path setup_path;
//       setup_path /= "..";
//       setup_path /= "DataGenerator";
//       setup_path /= "Setups";
//       // if (versions_count == 0) {
//       setup_path /= info.name + ".txt";
//       // } else {
//       //   setup_path /= info.name + "-v" + std::to_string(version) + ".txt";
//       // }
//       auto gen =
//           TasksAfterWorkersGenerator::CreateGenerator(setup_path, info.size);
//       auto test = gen->Generate();
//
//       std::filesystem::path output;
//       output /= "..";
//       output /= "Tests";
//       output /= std::to_string(info.size);
//       if (!exists(output)) {
//         std::filesystem::create_directory(output);
//       }
//       output /= info.name;
//       if (!exists(output)) {
//         std::filesystem::create_directory(output);
//       }
//       output /= "test" + std::to_string(i + first_test_index) + ".txt";
//       std::ofstream out(output, std::fstream::out | std::fstream::trunc);
//       assert(out.is_open());
//       SchedulerPtr
//           fake = std::make_shared<MockScheduler>(gen->GetMaxWorkerType(),
//                                                  gen->GetMaxTaskTypeId(),
//                                                  test.queries.size(),
//                                                  std::move(out));
//       for (const auto& q : test.queries) {
//         q.action->operator()(fake);
//       }
//     });
//   }
//   while (!threads.empty()) {
//     threads.top().join();
//     threads.pop();
//   }
//   SaveTestInfo(info, first_test_index + tests_to_generate - 1);
//   // ++version;
//   // } while (version > versions_count);
//   return 0;
// }
