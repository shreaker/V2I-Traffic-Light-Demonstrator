project(TrafficLight)
cmake_minimum_required(VERSION 3.2)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++") #otherwise build will fail

#add custom target to show header files in QtCreator
FILE(GLOB_RECURSE HeadFiles "*.h")
add_custom_target(headers SOURCES ${HeadFiles})

aux_source_directory(. SRC_LIST)
find_package(OpenCV REQUIRED)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} -lpthread -lboost_system -lcrypto -lssl -lcpprest -lwiringPi)
