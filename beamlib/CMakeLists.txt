add_library(beamlib
    src/blib.cpp
    include/beamlib/blib.h
)

target_include_directories(beamlib PUBLIC "include/")
target_link_libraries(beamlib PUBLIC glfw glad imgui)
