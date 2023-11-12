message(STATUS "Building nndeploy demo")

set(NNDEPLOY_DEPEND_LIBRARY_DEMO)
set(NNDEPLOY_SYSTEM_LIBRARY_DEMO)
set(NNDEPLOY_THIRD_PARTY_LIBRARY_DEMO)
include(${ROOT_PATH}/cmake/nndeploy_demo.cmake)

if (ENABLE_NNDEPLOY_MODEL_DETECT)
  include(${ROOT_PATH}/demo/detect/config.cmake)
endif()

if (ENABLE_NNDEPLOY_MODEL_DETECT_CONCURRENCY)
  include(${ROOT_PATH}/demo/detect_concurrency/config.cmake)
endif()

if (ENABLE_NNDEPLOY_MODEL_SEGMENT)
  include(${ROOT_PATH}/demo/segment/config.cmake)
endif()

if (ENABLE_NNDEPLOY_DEMO_PIPELINE_DAG)
  include(${ROOT_PATH}/demo/pipeline_dag/config.cmake)
endif()

