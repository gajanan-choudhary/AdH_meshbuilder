# General source files (always included)
list(APPEND builder_srcs
  # Insert new files below here
  generate_nodes.c
  generate_elements.c
  )

list(REMOVE_DUPLICATES builder_srcs)

add_library("builder" STATIC EXCLUDE_FROM_ALL ${builder_srcs})
