void clear_screen();

void print_boot();
void print_help();
void print_about();
void print_settings();
void print_tape(cell *c);
void print_instructions(char **instructions, int instructions_n, int mode);
void print_machine_iteration(char status, cell *c);

void print_changing_tm_mode();

void print_mode_change_success(char *input);
void print_success_loading();

void print_loading_warning();
void print_warning_no_instructions();
void print_no_instruction_found_alert();
void print_warning_no_instructions();

void print_error_during_loading();
void print_command_not_found();

void print_exiting();