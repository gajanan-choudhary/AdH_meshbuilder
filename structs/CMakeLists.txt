# General source files (always included)
list(APPEND structs_srcs
  # Insert new files below here
  mesh.c
  )

list(REMOVE_DUPLICATES structs_srcs)

add_library("structs" STATIC EXCLUDE_FROM_ALL ${structs_srcs})
