##############################################################################
# Build executable
list(APPEND meshbuilder_sources
    meshbuilder.c
    )
# Add main as build target
add_executable(meshbuilder ${meshbuilder_sources})

# Specify library dependencies
target_link_libraries(meshbuilder
  structs
  builder
  output
 )
