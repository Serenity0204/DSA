
# DSA
Just Some Implementations of General Data Structures in C++ Generic Forms.


## Implemented Data Structures
* Vector
* Singly Linked List
* Stack
* Queue
* Heap
* Priority Queue
* AVL Tree
* B Tree
* B+ Tree
* Map
* MultiMap


## Design
change the repo name:
Can be found in https://github.com/Serenity0204/DSA/blob/master/design.txt

## Installation

```
cmake_minimum_required(VERSION 3.12)
project(YourProjectName) # Replace "YourProjectName" with the actual name of your project

# Set CMP0002 policy to NEW to suppress the warning
cmake_policy(SET CMP0002 NEW)

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

## Fetch the content
include(FetchContent)
FetchContent_Declare(
  DSA
  GIT_REPOSITORY https://github.com/Serenity0204/DSA.git
)
FetchContent_MakeAvailable(DSA)

## Linking
ADD_EXECUTABLE(main main.cpp)
TARGET_LINK_LIBRARIES(main PRIVATE DSA)
```

