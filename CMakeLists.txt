cmake_minimum_required(VERSION 3.12)
project(game-of-life VERSION 1.0.0 LANGUAGES CXX)
find_package(atta 0.2.0.0 REQUIRED)

atta_add_target(project_script "src/projectScript.cpp")
