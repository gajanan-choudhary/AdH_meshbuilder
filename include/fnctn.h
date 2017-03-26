#ifndef _FNCTN_H
#define _FNCTN_H

// Input
void remove_spaces(char **);
CARD parse_card(char **);
int read_int_field(char **, int *);
double read_dbl_field(char **, int *);
void read_mesh_data(MESH **, int *, char *);

// Builder
void generate_nodes(MESH *);
void generate_elements(MESH *);

// Output
void write_mesh_file(MESH *mesh);
void write_bc_file(MESH *mesh);
void write_hotstart_file(MESH *mesh);
void write_superfile(MESH *mesh, int nmeshes);
int count_nodes(ELEM1D *elem1d, int nelems1d, int str);

#endif
