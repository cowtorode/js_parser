add_executable(example main.cpp)

# Link the json library — this brings in its include path too
target_link_libraries(example PRIVATE json)
