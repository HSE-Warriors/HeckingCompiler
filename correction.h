#ifndef CORRECTION_H
#define CORRECTION_H

void put_tabs(FILE* out, int cnt_tabs);

int str_fit(FILE* in, FILE* out, NODEE* com, char buf[], char type[50][40], int cnt_types, int* cnt_tabs, int* chek, NODEE* stack);

int check_type(char buf[], char type[50][40], int cnt_types);

#endif