#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_file_info(const char *filename, const struct stat *st) {
    
    printf("File: %s\n", filename); 
    printf("Type: "); 

    if (S_ISREG(st->st_mode)) {
     printf("Regular file\n");
     printf("Options:\n");
     printf("\t-n: name\n");
     printf("\t-d: size\n");
     printf("\t-h: hard link count\n");
     printf("\t-m: time of last modification\n");
     printf("\t-a: access rights\n");
     printf("\t-l: create simbolink link\n");
     }
    else if (S_ISLINK(st->st_mode)) {
	printf("Directory\n");
	printf("Options:\n");
	printf("\t-n: name\n");
	printf("\t-l: delete symbolic link\n"); //if this option is given the other options will no longer be performed
	printf("\t-d: size of symbolic link\n");
	printf("\t-t: size of target file\n");
	printf("\t-a: access rights\n");
	printf("\t-l: create simbolink link\n");

     }
    else if (S_ISDIR(st->st_mode)) {
	printf("Symbolic link\n");
	printf("Options:\n");
	printf("\t-n: name\n");
	printf("\t-d: size\n");
	printf("\t-a: access rights\n");
	printf("\t-c: total number of files with .c extension\n");

     } 
    else {
	 printf("Unknown\n"); }
    }
    
    char options[100];
    printf("Enter options: ");
    fgets(options, 100, stdin);
    
    if(S_ISREG(sb.st_mode)){
	char* p =options;
	while(*p){
	    if(*p=='-'){
	     p++;
	    
	    while(*p){
		if(*p=='n'){
		    printf("Name: %s\n", filename);
		}
		else if(*p=='d'){
		    printf("Size: %ld bytes\n", sb.st_size);
		}
		else if(*p=='h'){
		    //add hard link count to display
		    printf("Hard link count: %d links\n",);
		}
		else if(*p=='m'){
		    //add time variable to display
		    printf("Time of last modification: %_\n",);
		}
		else if(*p=='a'){
		    //add function to print access rights
		    printf("Access rights: \n");
		}
		else if(*p=='l'){
		    //create simbolic link
		    if{ //simbolic link was created
			printf("simbolic link created successfully\n");
		    }
		    else{
			printf("Error: couldn't create link\n");
		    }
		}
	    }
	    }
	}
    }
    else if(S_ISLINK(sb.st_mode)){
	char* p=options;
	while(*p){
	    if(*p=='-'){
		p++;

		while(*p){
		    if(*p=='n'){
			printf("Name: %s\n", filename);
		    }
		    else if(*p=='l'){
			//add function to delete symbolic link
			if{ // 
			    printf("Symbolic link deleted successfully \n");
			}
			else{
			    printf("Error: Couldn't delete symbolic link \n");
			}
		    }
		    else if(*p=='d'){
			printf("Size of symbolic link: %ld\n", sb.st_size);
		    }
		    else if(*p=='t'){
			//complete with the correct way to put the size oft the target file
			printf("Size of target file: %ld\n", sb.st_size);
		    }
		    else if(*p=='a'){
			//add function to print access rights
			printf("Access rights: \n");
		    }
		}
	    }
	}
    }
    else if(S_ISDIR(sb.st_mode)){
	char* p=options;
	while(*p){
	    if(*p){
		p++;
		
		while(*p){
		    if(*p=='n'){
			printf("Name: %s\n", filename);
		    }
		    else if(*p=='d'){
			printf("Size of directory: %lb\n", sb.st_size);
		    }
		    else if(*p=='a'){
			//add function to print access rights
			printf("Access rights: \n");
		    }
		    else if(*p=='c'){
			//add function to count the number of files with .c extension
			printf("Number of files: %d\n", );
		    }
		}
	    }
	}
    }
}

int main(){
    
}