cmake_minimum_required (VERSION 2.8.7)

project (fiducial_detector)

set (CMAKE_CXX_FLAGS "-std=c++0x -g")
set (CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /usr/local/lib)
include_directories (include /usr/local/include/ITK-4.12 /home/pranav/Libraries/itkSpectralClustering)
add_subdirectory (src)


