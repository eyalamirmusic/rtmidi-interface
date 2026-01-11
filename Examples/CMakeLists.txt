add_executable(Example Main.cpp)
target_link_libraries(Example PRIVATE RTMIDI-Interface)
set_high_warnings_level(Example)