void init_tape(char *tape);
void copy_tape(char *orig, char *dest);

typedef struct
{
    unsigned short pos;
    char content[TAPE_DIM];

} tape;