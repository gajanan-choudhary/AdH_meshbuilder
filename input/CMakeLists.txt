# General source files (always included)
list(APPEND input_srcs
  # Insert new files below here
  reader.c
  )

list(REMOVE_DUPLICATES input_srcs)

add_library("input" STATIC EXCLUDE_FROM_ALL ${input_srcs})
