#ifndef EDIT_H
#define EDIT_H

#include<stdio.h>
#include<string.h>
#include "view.h"

typedef struct{
    char *TIT2;
    char *TPE1;
    char *TALB;
    char *TYER;
    char *TCON;
    char *TCOM;
}tag;

Status validate_edit(char *argv[]);

Status MP3_edit(char *argv[]);

Status mp3_edit_content(char*argv[], char *content);



#endif

