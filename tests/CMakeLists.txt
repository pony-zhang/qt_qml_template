add_subdirectory(unit)
add_subdirectory(integration)

include(CTest)

function(add_qt_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})
    target_link_libraries(${TEST_NAME} PRIVATE
        Qt6::Core
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Test
    )
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()