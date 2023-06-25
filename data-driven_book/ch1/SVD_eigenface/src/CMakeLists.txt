
add_executable(main main.cpp utils.cpp)

target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/include)

target_compile_options(main PUBLIC -std=c++17 -O2)

target_link_libraries(main PRIVATE opencv_core opencv_highgui opencv_imgproc opencv_imgcodecs)

