# General source files (always included)
list(APPEND input_srcs
  # Insert new files below here
  reader.c
  read_int_field.c
  read_dbl_field.c
  read_mesh_data.c
  )

list(REMOVE_DUPLICATES input_srcs)

add_library("input" STATIC EXCLUDE_FROM_ALL ${input_srcs})
