#ifndef STRUCT_H
#define	STRUCT_H

typedef struct{
    char btn1_acc;
    char btn2_acc;
    char btn3_acc;
    char btn4_acc;
    
    char btn1_state;
    char btn2_state;
    char btn3_state;
    char btn4_state;
}button_debounc;

typedef struct{
    int freq;
    int full_note;
    char flag;
    int counter;
    int array_len;
}reprak_control;

#endif	/* STRUCT_H */

