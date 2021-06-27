#ifndef CORRECTION_H
#define CORRECTION_H

void put_tabs(FILE* out, int cnt_tabs);

int str_fit(FILE* in, FILE* out, NODEE* com, char buf[], char type[50][40], int cnt_types, int* cnt_tabs, int* chek, NODEE* stack);

void dir_correction(FILE* out, char buf[], char dir[50][40], int* cnt_dir);
void struct_correction(FILE* out, char buf[], char type[50][40], int cnt_types);
void func_correction(FILE* out, char buf[]);
void var_correction(FILE* out, char buf[]);

int check_type(char buf[], char type[50][40], int cnt_types);
int chek_dirs(char buf[], char type[50][40], int cnt);

#endif