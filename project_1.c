#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>

int count_lines(char* filename) {
    FILE* fp;
    int count = 0;
    char ch;

    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Could not open file %s\n", filename);
        return -1;
    }

    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }

    fclose(fp);
    return count;
}

void create_txt_file_in_directory(char *filename) {
    char file_path[256];
    sprintf(file_path, "%s/%s_file.txt", filename, filename);
    int fd = open(file_path, O_WRONLY | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    close(fd);
}

int compute_score(char* filename) {
    char command[256];
    sprintf(command, "./script.sh %s", filename);
    FILE* fp = popen(command, "r");
    if (!fp) {
        printf("Error executing script\n");
        return -1;
    }
    char output[1024];
    int num_errors = 0, num_warnings = 0;
    while (fgets(output, 1024, fp) != NULL) {
        if (strstr(output, "error")) {
            num_errors++;
        } else if (strstr(output, "warning")) {
            num_warnings++;
        }
    }
    pclose(fp);
    int score = 0;
    if (num_errors == 0 && num_warnings == 0) {
        score = 10;
    } else if (num_errors >= 1) {
        score = 1;
    } else if (num_warnings > 10) {
        score = 2;
    } else {
        score = 2 + 8*(10 - num_warnings)/10;
    }
    return score;
}


void print_access_rights(mode_t mode){

	printf("User:\n");
	printf("\tRead - %s\n", (mode & S_IRUSR) ? "yes" : "no");
    printf("\tWrite - %s\n", (mode & S_IWUSR) ? "yes" : "no");
    printf("\tExec - %s\n", (mode & S_IXUSR) ? "yes" : "no");
    printf("Group:\n");
    printf("\tRead - %s\n", (mode & S_IRGRP) ? "yes" : "no");
    printf("\tWrite - %s\n", (mode & S_IWGRP) ? "yes" : "no");
    printf("\tExec - %s\n", (mode & S_IXGRP) ? "yes" : "no");
    printf("Others:\n");
    printf("\tRead - %s\n", (mode & S_IROTH) ? "yes" : "no");
    printf("\tWrite - %s\n", (mode & S_IWOTH) ? "yes" : "no");
    printf("\tExec - %s\n", (mode & S_IXOTH) ? "yes" : "no");
}

void print_file_info(const char *filename) {
    
	struct stat sb;
	if(lstat(filename, &sb)== -1)
	{
		printf("Error: %s\n", strerror(errno));
		return;
	};
	

    printf("File: %s\n", filename); 
    printf("Type: "); 

    if (S_ISREG(sb.st_mode)) {
     printf("Regular file\n");
     printf("Options:\n");
     printf("\t-n: name\n");
     printf("\t-d: size\n");
     printf("\t-h: hard link count\n");
     printf("\t-m: time of last modification\n");
     printf("\t-a: access rights\n");
     printf("\t-l: create simbolink link\n");
     }
    else if (S_ISLNK(sb.st_mode)) {
	printf("Directory\n");
	printf("Options:\n");
	printf("\t-n: name\n");
	printf("\t-l: delete symbolic link\n"); //if this option is given the other options will no longer be performed
	printf("\t-d: size of symbolic link\n");
	printf("\t-t: size of target file\n");
	printf("\t-a: access rights\n");
	printf("\t-l: create simbolink link\n");

     }
    else if (S_ISDIR(sb.st_mode)) {
	printf("Symbolic link\n");
	printf("Options:\n");
	printf("\t-n: name\n");
	printf("\t-d: size\n");
	printf("\t-a: access rights\n");
	printf("\t-c: total number of files with .c extension\n");

     } 
    else {
	 printf("Unknown\n"); 
	 return;
	 }

    
    char options[100];
    printf("Enter options: ");
    fgets(options, 100, stdin);
    int running =1;
    if(S_ISREG(sb.st_mode)){
	char* p = getchar();
	while(*p !=EOF && running){
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
				printf("Hard link count: %ld links\n", sb.st_nlink);
			}
			else if(*p=='m'){
				
				printf("Time of last modification: %s\n", ctime(&sb.st_mtime));
			}
			else if(*p=='a'){
				//add function to print access rights
				printf("Access rights: \n");
				print_access_rights(sb.st_mode);
			}
			else if(*p=='l'){
				char linkname[100];
				printf("Enter symbolic link name: ");
				fgets(linkname,100,stdin);
				linkname[strcspn(linkname, "\n")]=0;
				
				if(symlink(filename,linkname)== -1){ 
				printf("Error: couldn't create simbolic link.\n");
				}else{
				printf("Symbolic link created successfully.\n");
				}
			}
			else{
				printf("Error:Invalid option -%c\n", *p);
				break;
			}
			p++;
			}
	    }
	}
    }
    else if(S_ISLNK(sb.st_mode)){
	char* p=getchar();
	while(*p !=EOF && running){
	    if(*p=='-'){
		p++;

		    if(*p=='n'){
			printf("Name: %s\n", filename);
		    }
		    else if(*p=='l'){
			//add function to delete symbolic link
			if(unlink(filename)==-1){ // 
			    printf("Error: Couldn't delete symbolic link.\n");
			}
			else{
			    printf("Symbolic link deleted successfully.\n");
			}
		    }
		    else if(*p=='d'){
			printf("Size of symbolic link: %ld\n", sb.st_size);
		    }
		    else if(*p=='t'){
				struct stat target_sb;
				if(stat(filename, &target_sb)==-1){
					printf("Error: %s\n", strerror(errno));
				}else{
					printf("Size of target file: %ld\n", sb.st_size);
				}
		    }
		    else if(*p=='a'){
			//add function to print access rights
			printf("Access rights: \n");
			print_access_rights(sb.st_mode);
		    }
			else{
				printf("Error: Invalid option -%c\n", *p);
			}
		}
		else{
			printf("Error: Invalid option string.\n");
	    }
	}
    }
    else if(S_ISDIR(sb.st_mode)){
	char* p=getchar();
	while(*p !=EOF && running){
	    if(*p){
		p++;
		
		while(*p){
		    if(*p=='n'){
			printf("Name: %s\n", filename);
		    }
		    else if(*p=='d'){
			printf("Size of directory: %ld\n", sb.st_size);
		    }
		    else if(*p=='a'){
			printf("Access rights: \n");
			print_access_rights(sb.st_mode);
		    }
		    else if(*p=='c'){
				int count=0;
				DIR* dirp =opendir(filename);
				if(dirp ==NULL){
					printf("Error couldn't open directory or directory is empty");
				}
				else{
					struct dirent* entry;
					while((entry=readdir(dirp))!=NULL){
						if(entry->d_type == DT_REG && strstr(entry->d_name, ".c")!=NULL){
							count++;
						}
					}
					closedir(dirp);
					printf("TOtal number of .c files: %d\n", count);
				}
		    }
			else{
				printf("Error: Invalid option: -%c\n", *p);
				break;
			}
			p++;
		}
	    }
		else{
			printf("Error: Invalid options string.\n");
		}
	}
	}
}

int main(int argc, char* argv[]){

	if (argc<2){
		printf("Not enough arguments");
		exit(1);
	}

	int num_files=argc-1;
	int i, score;
	char file_type,filename[256];

	for(i=1; i<=num_files; i++){
		struct stat status;

		pid_t menu=fork();

		if(menu==0){
			strcpy(filename, argv[i]);
			print_file_info(filename);
			exit(0);
		}
		else if(menu>0){
			int pfd[2]; // pfd[0]- read descriptor, pfd[1]-write descriptor
			int newfd;
			if(pipe(pfd)<0){
				printf("Pipe couldn't be created");
				exit(0);
			}
			int child_status;
			waitpid(menu, &child_status, 0);
			pid_t s_child =fork();
			if(s_child<0){
					printf("Error couldn't create second child process.\n");
			}else if(s_child==0){
				close(pfd[0]);
				if((newfd =  dup2(pfd[1],1))<0)
                    {
                        perror("Error couldn not duplicate\n");
                        exit(1);
                    }
				if(S_ISREG(status.st_mode)==1 && strstr(argv[i], ".c")){
					if(S_ISREG(status.st_mode)==1){
						execlp("bash", "bash", "srcipt.sh", argv[i], NULL);
					}
				}else if(S_ISREG(status.st_mode)==1){
					int numOfLines=count_lines(argv[i]);
					printf("The number of lines is: %d\n", numOfLines);
				}else if(S_ISDIR(status.st_mode)==1){
					create_txt_file_in_directory(argv[i]);
					printf("%s_file.txt created\n", argv[i]);
				}else if(S_ISLNK(status.st_mode)==1){
					if(execlp("chmod", "chmod", "u=rwx,g=rw,o=---",argv[i], NULL)== -1){
						perror("Failed to execute chmod command");
					}
				}
				close(pfd[1]);
				exit(1);
				}
				close(pfd[1]);
				FILE *stream = fdopen(pfd[0], "r");
				if(S_ISREG(status.st_mode)==1 && strstr(argv[i], ".c")){
					char errors[100], warnings[100];
					fscanf(stream, "%s", errors);
					fscanf(stream, "%s", warnings);
					FILE *file=fopen("grades.txt", "a");

					if(file == NULL ){
						perror("Failed to open file");
						return 1;
					}

					fprintf(file, "%s : %.1f\n", argv[i], compute_score(argv[i]));
					fclose(file);
				}

				char buffer[1024];

				while(fgets(buffer, sizeof(buffer),stream)!=NULL){
					printf("%s", buffer);
				}
				close(pfd[0]);
				int status;
				pid_t childPid=waitpid(s_child,&status,0);
				if(WIFEXITED(status)){
					int exitCode =WEXITSTATUS(status);
					printf("The process with PID %d has ended with exit code %d\n", childPid, exitCode);
				}
				else{
					printf("The process with PID %d has terminated abnormally\n", childPid);
				}

				childPid=waitpid(s_child, &status, 0);

				if(WIFEXITED(status)){
					int exitCode=WEXITSTATUS(status);
					printf("The process with PID %d has ended with exit code %d\n", childPid, exitCode);
				} else{
					printf("The process with PID %d has terminated abnormally\n", childPid);
				}
				
		}else{
                perror("Error with menu process");
                exit(-2);
            } 
	}
    printf("Hello world!\n");
	return 0;
}
