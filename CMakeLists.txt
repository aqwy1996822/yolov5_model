cmake_minimum_required(VERSION 3.2)
project(yolov5_model)

option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

find_package(CUDA REQUIRED)

# cuda
include_directories(/usr/local/cuda/include)
link_directories(/usr/local/cuda/lib64)
# tensorrt
include_directories(/home/leo/LibProjects/TensorRT-7.2.2.3/include/)
link_directories(/home/leo/LibProjects/TensorRT-7.2.2.3/lib/)
#opencv
find_package(OpenCV 3.4 REQUIRED)
message(STATUS "OpenCV Version: ${OpenCV_VERSION}")
message(STATUS "OpenCV_LIBS: ${OpenCV_LIBS}")
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(include)

cuda_add_library(yolov5_model SHARED
        src/yolo/yolov5_model.cpp
        src/yolo/calibrator.cpp
        src/yolo/preprocess.cu
        src/yolo/common.cpp
        src/yolo/yololayer.cu)
target_link_libraries(yolov5_model nvinfer)
target_link_libraries(yolov5_model cudart)
target_link_libraries(yolov5_model ${OpenCV_LIBS})

cuda_add_executable(yolov5 main.cpp)
target_link_libraries(yolov5 yolov5_model)