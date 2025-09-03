# CMake 최소 요구 버전 설정
cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름과 C++ 버전 설정
project(MyBossFighter VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------
# SFML 라이브러리 찾기
# ------------------------------------------------------------------
find_package(SFML 3.0.0 COMPONENTS Graphics Window REQUIRED)

# ------------------------------------------------------------------
# 소스 파일 목록 관리 
# ------------------------------------------------------------------
# src 폴더와 그 하위 폴더의 모든 .cpp 파일 및 main.cpp를 찾아 추가합니다.
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

# ------------------------------------------------------------------
# 실행 파일(Executable) 생성
# ------------------------------------------------------------------
add_executable(${PROJECT_NAME} ${SOURCES})

# ------------------------------------------------------------------
# 헤더 파일 경로 설정
# ------------------------------------------------------------------
# 컴파일러에게 헤더 파일을 찾을 기준 경로를 알려줍니다.
# 프로젝트의 "src" 폴더를 포함 경로로 추가합니다.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ------------------------------------------------------------------
# 라이브러리 및 컴파일 옵션 설정
# ------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()

# ------------------------------------------------------------------
# 실행 파일 출력 경로 설정 (선택 사항)
# ------------------------------------------------------------------
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

