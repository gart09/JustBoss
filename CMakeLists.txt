# CMake 최소 요구 버전 설정
cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름과 C++ 버전 설정
project(MyBossFighter VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- ?????? 수정된 부분: UTF-8 옵션 전역 적용 ?????? ---
# Visual Studio 컴파일러(MSVC) 사용 시, UTF-8 인코딩을 전역으로 강제합니다.
# 이렇게 하면 우리 프로젝트뿐만 아니라 모든 하위 라이브러리에도 적용됩니다.
if(MSVC)
    add_compile_options(/utf-8)
endif()

# ------------------------------------------------------------------
# SFML 라이브러리 찾기
# ------------------------------------------------------------------
find_package(SFML 3.0.0 COMPONENTS Graphics Window REQUIRED)

# --- ?????? 최종 수정: ImGui/ImGui-SFML을 소스 코드로 직접 포함 ?????? ---
# 1. ImGui와 ImGui-SFML의 소스 코드 파일 목록을 변수에 저장합니다.
file(GLOB IMGUI_SOURCES
    "lib/imgui/imgui.cpp"
    "lib/imgui/imgui_draw.cpp"
    "lib/imgui/imgui_tables.cpp"
    "lib/imgui/imgui_widgets.cpp"
    "lib/imgui-sfml/imgui-SFML.cpp" # ImGui-SFML 소스 추가
)

# 2. ImGui-SFML은 CMakeLists.txt가 있으므로 add_subdirectory를 그대로 사용합니다.
add_subdirectory(lib/imgui-sfml)

# ------------------------------------------------------------------
# 소스 파일 목록 관리 
# ------------------------------------------------------------------
# src 폴더와 그 하위 폴더의 모든 .cpp 파일 및 main.cpp를 찾아 추가합니다.
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/**/*.cpp")

# ------------------------------------------------------------------
# 실행 파일(Executable) 생성
# ------------------------------------------------------------------
# 우리 게임 소스와 ImGui 소스를 함께 묶어 실행 파일을 만듭니다.
add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES})

# ------------------------------------------------------------------
# 헤더 파일 경로 설정
# ------------------------------------------------------------------
# 컴파일러가 헤더 파일을 찾을 수 있도록 모든 관련 경로를 알려줍니다.
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/lib/imgui-sfml" # ImGui-SFML 헤더 경로 추가
)

# ------------------------------------------------------------------
# 라이브러리 및 컴파일 옵션 설정
# ------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Graphics SFML::Window)

# ------------------------------------------------------------------
# 실행 파일 출력 경로 설정 (선택 사항)
# ------------------------------------------------------------------
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

