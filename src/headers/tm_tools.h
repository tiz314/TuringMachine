typedef struct
{
    char element;
    struct cell *next;
    struct cell *prev;
} cell;

void init_tape(cell *c);
void copy_tape(cell *orig, cell *dest);
cell *get_first_element(cell *c);
void fill_tape(FILE *input_file, cell *c);
void free_tape(cell *c);

void print_cell_sequence(cell *c); // Only for debugging