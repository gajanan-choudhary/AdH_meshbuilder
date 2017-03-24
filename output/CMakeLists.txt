# General source files (always included)
list(APPEND output_srcs
  # Insert new files below here
  writer.c
  )

list(REMOVE_DUPLICATES output_srcs)

add_library("output" STATIC EXCLUDE_FROM_ALL ${output_srcs})
