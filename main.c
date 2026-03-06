#include "view.h"
#include "edit.h"


int main(int argc, char*argv[]) {

    if(argc < 2){
        printf("---------------------------------------------------------------------------------------\n");
        printf("INVALID ARGUMENTS\nUSAGE\n");
        printf("To view please pass like : ./a.out -v file_name.mp3\n");
        printf("To edit please pass like : ./a.out -e -t/-a/-A/-y/-m/-c changing_text file_name.mp3\n");
        printf("To get help pass like : ./a.out --help\n");
        printf("---------------------------------------------------------------------------------------\n");
        return -1;
    }
    
    int res = check_operation_type(argv);

    if(res == 0) {
        printf("---------------------------------------------------------------------------------------\n");
        printf("1. -v -> to view mp3 file contents\n2. -e -> to edit mp3 file contents\n");
        printf("\t2.1. -t -> to edit song title\n");
        printf("\t2.2. -a -> to edit artist name\n");
        printf("\t2.3. -A -> to edit album name\n");
        printf("\t2.4. -y -> to edit year\n");
        printf("\t2.5. -m -> to edit content\n");
        printf("\t2.6. -c -> to edit comment\n");
    }

    else if (res == 1) {
        printf("-------------------------------VIEW SELECTED-------------------------------------------\n");

        //function call for MP3_VIEW function
        if (MP3_view(argv) == mp3_success) {

            printf("Success : MP3_view operation is successful\n");

        } else {
            printf("ERROR : Failed MP3_view operation\n");
            printf("---------------------------------------------------------------------------------------\n");
            return mp3_failure;
        }
        
    }

    else if(res == 2){
        printf("-------------------------------EDIT SELECTED-------------------------------------------\n");

        //function call for edit function
        if (MP3_edit (argv) == mp3_success) {

            printf("Success : MP3_edit operation is successful\n");
            
        } else {
            printf("ERROR : Failed MP3_edit operation\n");
            printf("---------------------------------------------------------------------------------------\n");
            return mp3_failure;
        }
    }
     
    else {
        printf("Invalid operation type\n");
        printf("---------------------------------------------------------------------------------------\n");
        return -1;
    }

    printf("---------------------------------------------------------------------------------------\n");
    return 0;
}


operationType check_operation_type(char *argv[]){

    if (strcmp (argv[1] , "--help") == 0)
        return mp3_help;

    else if(strcmp (argv[1],"-v")== 0)
        return mp3_view;

    else if(strcmp (argv[1], "-e")== 0)
        return mp3_edit;

    else 
        return mp3_unsupported;
}