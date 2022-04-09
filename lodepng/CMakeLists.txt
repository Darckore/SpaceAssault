cmake_minimum_required(VERSION 3.23)
include("${OPT_UTILS_DIR}/utils.cmake")

project(${TARGET_NAME} CXX)

collect_sources(SOURCE_FILES HEADERS ADDITIONAL_FILES)
add_library(${TARGET_NAME} STATIC ${SOURCE_FILES} ${HEADERS} ${OPT_PCH_NAME} ${ADDITIONAL_FILES})
set_build_opts(${TARGET_NAME} "${ADDITIONAL_FILES}")

if(MSVC)
  target_compile_options(${TARGET_NAME} PRIVATE /W0 /JMC $<$<CONFIG:RELEASE>:/GL /Ot /Oi>)
  target_link_options(${TARGET_NAME} PRIVATE $<$<CONFIG:RELEASE>:/LTCG>)
  set_target_properties( ${TARGET_NAME} PROPERTIES
                         VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")
else()
  target_compile_options(${TARGET_NAME} PRIVATE -w)
endif()

make_src_groups("${SOURCE_FILES}" "${HEADERS}" "${ADDITIONAL_FILES}")