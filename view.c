#include "view.h"

Status MP3_view(char *argv[]) {

    //function call to validate the second argument
    if (validate_view(argv) == mp3_success)
    {
        //open the sample.mp3 file in read mode
        FILE *fp = fopen(argv[2], "r");

        if(fp == NULL){
            printf("ERROR : Failed to open the sample.mp3 file\n");
            return mp3_failure;
        }

        //fetch first 3 bytes of header -->ID3
        char buff[4];
        fread(buff, 3, 1, fp);
        buff[3] = '\0';

        //compare buffer with ID3
        if(strcmp(buff, "ID3") != 0){
            printf("size of buffer is : %ld\n",sizeof(buff));
            printf("ID3 is the header id not %s\n",buff);
            printf("DEBUG: Current file position = %ld\n", ftell(fp));

            return mp3_failure;

        } else {

            printf("The Header ID is %s\n",buff);
        }

        //Skip the next 7 bytes after ID3
        fseek(fp, 7, SEEK_CUR);
        printf("DEBUG : Current file pointer position is %ld\n",ftell(fp));

        //Run the loop to read all 6 tags 

        for(int i = 0; i < 6; i++) {

            //Read the 4 bytes of tag ID
            char tag[5] = {0};
            fread(tag, 4, 1, fp);

            printf("%d. %s ---> ",i+1,tag);

            //Read the 4 bytes for size of the content
            int size;
            fread(&size, sizeof(int), 1, fp);

            //printf("Size in big_endian system : %d\n",size);

            //convert the output from big endian to little endian system
            if (big_endian_to_little_endian(&size) == mp3_success){
                //printf("Size of the content in little_endian system is : %d\n",size);
            }

            //skip next 3 bytes of data
            fseek(fp, 3, SEEK_CUR);

            if(size <= 1) {
                printf("Error : Empty or invalid content\n");
                continue;  //to skip to next frame
            }

            // Read the frame content: ALLOCATE DYNAMICALLY
            int text_len = size - 1;
            // Allocate size+1 bytes (size for content + 1 for null terminator)
            char *content_buff = (char *)malloc(text_len + 1); 
            if (content_buff == NULL) {
                printf("ERROR : Memory allocation failed\n");
                return mp3_failure;
            }

            // Read 'size' bytes of content
            if (fread(content_buff, 1, text_len, fp) != text_len) {
                printf("ERROR : Failed to read frame content\n");
                free(content_buff);
                return mp3_failure;
            }

            // Null-terminate the content buffer at the correct position
            content_buff[text_len] = '\0'; 

            printf("%s\n", content_buff);
            
            // Free the allocated memory
            free(content_buff);
        }
    } 
    else {
        return mp3_failure;
    }

    return mp3_success;

}

Status validate_view(char *argv[]){

    if (strstr(argv[2],".mp3") == NULL) {
        printf("ERROR : file name shoule be of .mp3 extention\n");
        return mp3_failure;
    }
     
    if(strcmp( strstr(argv[2], ".mp3"), ".mp3") == 0){
        printf("INFO : File_name contains .mp3 extention\n");
        //return mp3_success;

    } else {
        printf("ERROR : File name shoulb be of .mp3 extention\n");
        return mp3_failure;
    }
    return mp3_success;
}

Status big_endian_to_little_endian(int *num){

    unsigned char *ptr = (unsigned char*)num;
    for(int i = 0; i < (sizeof(int)/2); i++){
        unsigned char temp = ptr[i];
        ptr[i] = ptr[sizeof(int)-i-1];
        ptr[sizeof(int)-i-1] = temp;
    }
    return mp3_success;
}