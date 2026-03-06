#ifndef VIEW_H
#define VIEW_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>



typedef enum {
    
    mp3_help,
    mp3_view,
    mp3_edit,
    mp3_unsupported

} operationType;

typedef enum {
    mp3_success,
    mp3_failure
} Status;

operationType check_operation_type(char *argv[]);

Status MP3_view(char *argv[]);

Status validate_view(char* argv[]);

Status big_endian_to_little_endian(int *num);

#endif
