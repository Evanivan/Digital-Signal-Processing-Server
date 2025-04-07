# test/CMakeLists.txt
add_executable(unit_tests
  unit_tests.cpp
  test_client.cpp
  ../source/proccessing/signal_proccessing.cpp
)

target_link_libraries(unit_tests
  PRIVATE
    Boost::system
    gtest
    gtest_main
)

add_test(
  NAME unit_tests
  COMMAND unit_tests
)

add_executable(integration_client
  test_client.cpp
  ../source/proccessing/signal_proccessing.cpp
)

target_link_libraries(integration_client PRIVATE 
    Boost::system
    gtest
    gtest_main
)
