cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(FBClassic)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "FlappyBird Classic")
set(VITA_TITLEID  "FLAPPY001")
set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

include_directories(
)

link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/utils.cpp
  src/collision.cpp
  src/core.cpp
)

target_link_libraries(${PROJECT_NAME}
	vita2d 
	soloud
	pthread
	SceAudio_stub
	SceDisplay_stub 
	SceTouch_stub
	SceGxm_stub
	SceSysmodule_stub 
	SceCtrl_stub 
	ScePgf_stub 
	ScePvf_stub
	SceIofilemgr_stub 
	SceCommonDialog_stub
	freetype 
	png 
	jpeg 
	z 
	m 
	c
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
  
  FILE sounds/sfx_hit.ogg sounds/sfx_hit.ogg
  FILE sounds/sfx_die.ogg sounds/sfx_die.ogg
  FILE sounds/sfx_point.ogg sounds/sfx_point.ogg
  FILE sounds/sfx_wing.ogg sounds/sfx_wing.ogg
  FILE sounds/sfx_swooshing.ogg sounds/sfx_swooshing.ogg

  FILE assets.png assets.png
)
