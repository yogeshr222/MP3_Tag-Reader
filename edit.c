#include "edit.h"
#include<stdio.h>
#include<stdlib.h>

tag mp3_tag;  //structure variable

//function defintion
Status MP3_edit(char *argv[]) {

    //verify the command line arguments
    if (validate_edit(argv) != mp3_success) {
        return mp3_failure;
    }

    //Read the frame ID
    char frame_id[5] = {0};
    if (strcmp( argv[2] , "-t") == 0)
        strcpy(frame_id , "TIT2");

    else if (strcmp( argv[2] , "-a") == 0)
        strcpy(frame_id , "TPE1");
    
    else if (strcmp( argv[2] , "-A") == 0)
        strcpy(frame_id , "TALB");
    
    else if (strcmp( argv[2] , "-y") == 0)
        strcpy(frame_id , "TYER");
    
    else if (strcmp( argv[2] , "-m") == 0)
        strcpy(frame_id , "TCON");

    else if (strcmp( argv[2] , "-c") == 0)
        strcpy(frame_id , "COMM");

    else {
        printf("Error : Invalid frame_id\n");
        return mp3_failure;
    }
    
    printf("Frame_ID from CLA is %s and length is %ld\n\n",frame_id ,strlen(frame_id));

    //create one copy of sample.mp3 file - > duplicate.mp3
    //for that open the sample.mp3 in read mode and duplicate.mp3 in write mode
    FILE *s_fp = fopen ("sample.mp3" , "rb");
    if(!s_fp){
        printf("Error : Failed to open sample.mp3\n");
        return mp3_failure;
    }

    FILE *d_fp = fopen("duplicate.mp3" , "wb");
    if(!d_fp) {
        printf("Error : Failed to open duplicate.mp3\n");
        fclose(s_fp);
        return mp3_failure;
    }
    
    //Read first 10 bytes of data from sample.mp3 to duplicate.mp3
    char header_ID[10] = {0};
    fread(header_ID, 10 , 1, s_fp);
    //printf("DEBUG : Current sample.mp3 file pointer position is %ld\n",ftell(s_fp));

    fwrite(header_ID, 10 , 1, d_fp);
    //printf("DEBUG : Current duplicate.mp3 file pointer position is %ld\n\n",ftell(d_fp));

    //Repeat this for 6 times
    for(int i = 0; i < 6 ; i ++ ) {

        //Read the next 4 bytes from metadata i.e. tag_id
        char tag_id[5] = {0};
        if (fread(tag_id , 4 , 1 , s_fp) != 1) {
            break;
        }
        //printf("DEBUG : Current sample.mp3 file pointer position is %ld\n",ftell(s_fp));

        if(tag_id[0] == '\0'){
            break;
        }
        //compare this tag_id with frame_id in CLA
        if (strcmp (frame_id , tag_id) == 0) {

            printf("Tag_ID (in if_block) -> %s\n",tag_id);

            //write the tag into duplicate.mp3
            fwrite(tag_id , 4 , 1 , d_fp);
            //printf("DEBUG in if: Current duplicate.mp3 file pointer position is %ld\n\n",ftell(d_fp));

            //Read the size and convert endianess and store in duplicate.mp3
            unsigned int old_size = 0;
            unsigned char old_size_bytes[4];
            
            if (fread (old_size_bytes , 1 , 4 , s_fp) != 4) {
                printf("Error : Failed to read old frame size\n");
                fclose(s_fp);
                fclose(d_fp);
                return mp3_failure;
            }
            
            //convert old_siz bytes to little endian to get old size
            old_size = (old_size_bytes[0] << 24) | (old_size_bytes[1] << 16) | (old_size_bytes[2] << 8) | old_size_bytes[3] ;
            printf("DEBUG :old Frame content size = %u bytes\n", old_size);

            //calculate the new size
            unsigned int new_size = strlen(argv[3]) + 1;
            unsigned int copy_new_size = new_size;

            //convert copied new size into big endian system
            big_endian_to_little_endian((int*)&copy_new_size);
            printf("DEBUG :new Frame content size = %u bytes\n", new_size);

            //move the file pointer of sample.mp3 to +size position forward
            
            //printf("DEBUG : Current sample.mp3 file pointer position is %ld\n",ftell(s_fp));

            fwrite(&copy_new_size, 4, 1, d_fp);
            //printf("DEBUG : Current duplicate.mp3 file pointer position is %ld\n\n",ftell(d_fp));

            //Read and write flag (null to be read later) from sample.mp3 to duplicate.mp3
            char flag_null_buf[2] = {0};
            fread(flag_null_buf , 1 , 2 , s_fp);
            //printf("DEBUG : Current sample.mp3 file pointer position is %ld\n",ftell(s_fp));

            fwrite(flag_null_buf , 1 , 2 , d_fp);
            //printf("DEBUG : Current duplicate.mp3 file pointer position is %ld\n\n",ftell(d_fp));

            //skip the old content + 1 byte of prevoius null 
            fseek(s_fp , old_size , SEEK_CUR);

            //write the new content including previous 1 byte of null
            //allocate memoty dynamically for content frame because of variable length
            char *content_buf = malloc(new_size+1);
            if(!content_buf){
                printf("Error : Memory allocation failed\n");
                fclose(s_fp);
                fclose(d_fp);
                return mp3_failure;
            }
            content_buf[0] = 0;
            strcpy(content_buf+1, argv[3]);

            printf("%s -> %s\n\n",tag_id, content_buf + 1);
            
            fwrite(content_buf , 1 , new_size, d_fp);
            //printf("DEBUG : Current duplicate.mp3 file pointer position is %ld\n\n",ftell(d_fp));

            printf("Edited -> %s successfully\n\n",frame_id);

            free(content_buf);
        
        }

        else {
            //write the tag into duplicate.mp3
            fwrite(tag_id , 4 , 1 , d_fp);
            printf("Tag_ID is -> %s \n", tag_id);
            //printf("DEBUG in else: Current duplicate.mp3 file pointer position is %ld\n\n",ftell(d_fp));

            unsigned int size = 0;
            unsigned char size_bytes[4];
            if (fread(size_bytes, 1, 4, s_fp) != 4)
            {
                printf("ERROR : Failed to read frame size\n");
                fclose(s_fp);
                fclose(d_fp);
                return mp3_failure;
            }
            //printf("DEBUG : Current sample.mp3 file pointer position is %ld\n",ftell(s_fp));
            // Step 2: Convert from big endian to little endian
            size = (size_bytes[0] << 24) | (size_bytes[1] << 16) |
                (size_bytes[2] << 8)  |  size_bytes[3];

            printf("DEBUG : Frame content size = %u bytes\n", size);

            //  Write size back to duplicate file (same 4 bytes)
            fwrite(size_bytes, 1, 4, d_fp);
            //printf("DEBUG : Current duplicate.mp3 file pointer position is %ld\n\n",ftell(d_fp));

            //Read 2 bytes of flag from sample.mp3 
            char flag_null_buf[2] = {0};
            fread(flag_null_buf , 1 , 2 , s_fp);
            //printf("DEBUG : Current sample.mp3 file pointer position is %ld\n",ftell(s_fp));

            fwrite(flag_null_buf , 1 , 2 , d_fp);
            //printf("DEBUG : Current duplicate.mp3 file pointer position is %ld\n\n",ftell(d_fp));

            //Read the frame content from sample.mp3 to duplicate.mp3
            //allocate memoty dynamically for content frame because of variable length
            char *content_buf = malloc(size+1);
            if(!content_buf){
                printf("Error : Memory allocation failed\n");
                fclose(s_fp);
                fclose(d_fp);
                return mp3_failure;
            }
            //read the content from sample.mp3 of length size
            if (fread (content_buf , 1 , size, s_fp) != size) {
                printf("Error : Failed to read frame content from sample.mp3\n");
                free(content_buf);
                fclose(s_fp);
                fclose(d_fp);
                return mp3_failure;
            }

            content_buf[size] = '\0';

            //printf("DEBUG : Current sample.mp3 file pointer position is %ld\n",ftell(s_fp));
            printf("%s -> %s\n\n",tag_id, content_buf + 1); //here +1 byte is for previous null byte

            fwrite(content_buf , 1 , size, d_fp);
            //printf("DEBUG : Current duplicate.mp3 file pointer position is %ld\n\n",ftell(d_fp));

            free(content_buf);
        }

    }
    //printf("DEBUG : Current sample.mp3 file pointer position is %ld\n",ftell(s_fp));
    //printf("DEBUG : Current duplicate.mp3 file pointer position is %ld\n\n",ftell(d_fp));

    // Copy remaining data from sample.mp3 to duplicate.mp3
    unsigned int bytesRead;
    unsigned char buff[1024] = {0};
    
    // Read, then check if bytesRead is greater than 0
    while ( (bytesRead = fread(buff, 1, sizeof(buff), s_fp)) > 0 )
    {
        if (fwrite(buff , 1 , bytesRead , d_fp) != bytesRead) {
            perror("Error writing remaining data");
            break;
        }
    }
    
    //Remove the original
    //remove("sample.mp3");

    //rename the duplicate file as original file
    rename("duplicate.mp3" , "original.mp3");

    fclose(s_fp);
    fclose(d_fp);
    
    return mp3_success;
}


Status validate_edit(char *argv[]){
    if(argv[2] == NULL || argv[3] == NULL || argv[4] == NULL) {
        printf("Error : Invalid number of arguments\n");
        return mp3_failure;

    } else
        printf("INFO : Validation is success\n");
    return mp3_success;
}
