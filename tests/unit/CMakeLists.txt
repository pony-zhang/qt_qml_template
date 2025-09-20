include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src)

add_qt_test(test_data_model
    test_data_model.cpp
    ../../src/data/models/DataModel.cpp
)