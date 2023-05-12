#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int args, char* argv[]){

    if(args<2){
        printf("Not enough arguments\n");
    }
    struct stat info;
    int stat=lstat(argv[1],&info);
    printf("%d\n", stat){
    int regFile=S_ISREG(info.st_mode);
    int directory=S_ISDIR(info.st_mode);
    int simbLink=S_ISLNK(info.st_mode);
    printf("Type of file: %d\n", regFile);
    return 0;

    if(regFile){
        printf("Select what option do you want: \n");
        scanf("%s")
    }
}