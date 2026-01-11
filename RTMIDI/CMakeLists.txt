add_library(RTMIDI-Interface STATIC)

target_include_directories(RTMIDI-Interface
        PRIVATE ${RTMIDI_SOURCE_DIR}
        PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../)

target_compile_definitions(RTMIDI-Interface PRIVATE __MACOSX_CORE__=1)
target_sources(RTMIDI-Interface PRIVATE
        ${RTMIDI_SOURCE_DIR}/RtMidi.cpp
        ${RTMIDI_SOURCE_DIR}/RtMidi_c.cpp
        Interface.cpp)

target_link_libraries(RTMIDI-Interface PRIVATE
        "-framework CoreAudio"
        "-framework CoreMIDI"
        "-framework CoreFoundation")

