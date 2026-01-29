# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "Black-Scholes_autogen"
  "CMakeFiles\\Black-Scholes_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Black-Scholes_autogen.dir\\ParseCache.txt"
  )
endif()
