cmake_minimum_required(VERSION 3.6)

project(recordreplay_planner_node)

### Dependencies
find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

### Compile options
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

ament_auto_add_library(${PROJECT_NAME} SHARED
  src/recordreplay_planner_node/recordreplay_planner_node.cpp)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${tf2_INCLUDE_DIRS})
autoware_set_compile_options(${PROJECT_NAME})

rclcpp_components_register_node(${PROJECT_NAME}
  PLUGIN "motion::planning::recordreplay_planner_node::RecordReplayPlannerNode"
  EXECUTABLE ${PROJECT_NAME}_exe
)

### Test
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # Linters
  ament_lint_auto_find_test_dependencies()
  # Unit test
  ament_add_gtest(recordreplay_planner_node_unit_tests
    test/gtest_main.cpp
    test/sanity_checks.cpp)
  target_link_libraries(recordreplay_planner_node_unit_tests ${PROJECT_NAME})
  target_include_directories(recordreplay_planner_node_unit_tests PRIVATE "include")
endif()

ament_auto_package(INSTALL_TO_SHARE param launch)
