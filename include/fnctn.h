void remove_spaces(char *, char **);
unsigned int read_word(char*, char **);
void read_input_file(MESH **, int *, char **);
void generate_nodes(MESH *);
void generate_elements(MESH *);
void write_mesh_file(MESH *mesh);
void write_bc_file(MESH *mesh);
void write_hotstart_file(MESH *mesh);
