cmake_minimum_required(VERSION 3.9)
project(manVcat)

set(CMAKE_CXX_STANDARD 11)

add_executable(manVcat Graphics.cpp Graphics.h Model.cpp Bullet.cpp Tank.cpp Cat.cpp Loader.cpp main.cpp Model.h Bullet.h Loader.h Tank.h Cat.h Bush.cpp Bush.h Common.cpp Common.h CollisionManager.cpp AutoPilot.cpp AutoPilot.h)
target_link_libraries(manVcat GL GLU glut)