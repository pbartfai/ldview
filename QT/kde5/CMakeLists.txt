project(ldviewthumbnail)
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
set(QT_MIN_VERSION "5.2.0")

find_package(ECM 1.0.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(FeatureSummary)
include(WriteBasicConfigVersionFile)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
find_package(KF5 REQUIRED COMPONENTS KIO)
set(ldviewthumbnail_SRCS ldviewthumbnailcreator.cpp)
add_library(ldviewthumbnail MODULE ${ldviewthumbnail_SRCS})

target_link_libraries(ldviewthumbnail PRIVATE KF5::KIOWidgets )
install(TARGETS ldviewthumbnail DESTINATION ${PLUGIN_INSTALL_DIR} )
install(FILES  ldviewthumbnailcreator.desktop DESTINATION ${SERVICES_INSTALL_DIR})

