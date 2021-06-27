#ifndef CORRECTION_H
#define CORRECTION_H

void put_tabs(FILE* out, int cnt_tabs);

int str_fit(FILE* in, FILE* out, NODEE* com, char buf[], char type[50][40], int cnt_types, int* cnt_tabs, int* chek, NODEE* stack);

void dir_correction(FILE* out, char buf[], char dir[50][40], int* cnt_dir);
void struct_correction(FILE* out, char buf[], char type[50][40], int cnt_types);
void func_correction(FILE* out, char buf[]);
void var_correction(FILE* out, char buf[]);
void condition_correction(FILE* out, char buf[], int* chek);
void for_correction(FILE* out, char buf[], int* chek);
void case_correction(FILE* out, char buf[]);
void break_correction(FILE* out, char buf[]);
void return_correction(FILE* out, char buf[]);
void str_correction(FILE* out, char buf[]);
void put_left_bracket(FILE* out, char buf[]);
void put_right_bracket(FILE* out, char buf[]);
void after_struct_correction(FILE* out, char buf[], char type[50][40], int cnt_types);
void big_com_correction(FILE* out, char buf[], int cnt);
void small_com_correction(char buf[], char com[]);

int check_type(char buf[], char type[50][40], int cnt_types);
int chek_dirs(char buf[], char type[50][40], int cnt);

#endif