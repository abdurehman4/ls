#include <stdio.h>    // For printing text
#include <dirent.h>   // For getting list of members of a directory
#include <string.h>   // For using functions like strcmp
#include <sys/stat.h> // For differentiating between directories and files
#include <stdlib.h>   // For using malloc()


int get_no_of_files(DIR ** directory,struct dirent ** dir_entry){
  int count = 0;

  while ((*dir_entry = readdir(*directory)) != NULL)
    ++count;
  return count-2; // Excluding "." and ".."
}


void get_files_list(DIR ** directory, struct dirent ** dir_entry, char * files[])
{
  rewinddir(*directory);

  int i = 0; 
  while ((*dir_entry = readdir(*directory)) != NULL)
  {
    if (strcmp((*dir_entry)->d_name,"..") == 0 || strcmp((*dir_entry)->d_name,".") == 0){
      continue;
    }
    else{
    files[i++] = (*dir_entry)->d_name;
  }}
}

int get_no_of_non_hidden_files(int no_of_files,char *files[]){

  int num_of_non_hidden_files = 0;

  for (int i=0; i<no_of_files; i++){
    if (files[i][0] != '.'){
      num_of_non_hidden_files++;
    }
  }
  return num_of_non_hidden_files; 
}

char * get_non_hidden_files(int no_of_files, char * files[], char * non_hidden_files[])
{
  int j=0;
  for (int i=0; i<no_of_files; i++){
    if (files[i][0] != '.'){
      non_hidden_files[j++] = files[i];
    }
  }
}



int get_no_of_hidden_files(int no_of_files,char *files[]){

  int num_of_hidden_files = 0;

  for (int i=0; i<no_of_files; i++){
    if (files[i][0] == '.'){
      num_of_hidden_files++;
    }
  }
  return num_of_hidden_files; 
}

char * get_hidden_files(int no_of_files, char * files[], char * hidden_files[])
{
  int j=0;
  for (int i=0; i<no_of_files; i++){
    if (files[i][0] == '.'){
      hidden_files[j++] = files[i];
    }
  }
}



void printFiles(int no_of_files,char * files[]){
  for (int i=0; i<no_of_files; i++){
    printf("%s\t", files[i]);
  }
}

int main(int argc,char* argv[])
{
  struct dirent *dir_entry;
  char *dir_path; 

  // Mode int determines what should be printed
  int mode = 0;
  // Dealing with args
  for (int i=1; i<argc; i++)
  {
    const char* arg = argv[i];
    if (arg[0] != '-') {
      dir_path = (char*)malloc(sizeof(arg)); // Allocating some memory to the pointer
      strcpy(dir_path, arg);
    }
    else if (arg[0] == '-') {
      if ( (strcmp("-a",arg) == 0) || (strcmp("--all", arg) == 0) ){
        mode = 1;
      } else if ( (strcmp("-h", arg) == 0) || (strcmp("--hidden", arg) == 0) )
      {
        mode =2;
      }
    }
  }

  // Opening a directory
  DIR * directory = opendir(dir_path);

  if (directory==NULL){
    printf("Error while opening the given directory!");
    return 1;
  }

  int total_no_of_files = get_no_of_files(&directory,&dir_entry);

  char * names[total_no_of_files];
  get_files_list(&directory, &dir_entry, names);
  
  int no_of_non_hidden_files = get_no_of_non_hidden_files(total_no_of_files, names);
  char* non_hidden_files[no_of_non_hidden_files];
  get_non_hidden_files(total_no_of_files, names, non_hidden_files);

  int num_of_hidden_files = get_no_of_hidden_files(total_no_of_files,names);

  char * hidden_files[num_of_hidden_files];
  get_hidden_files(total_no_of_files, names, hidden_files);
  // puts("Switch");
  switch (mode) {
    case 0:
      printFiles(no_of_non_hidden_files, non_hidden_files);
      break;
    case 1:
      printFiles(total_no_of_files, names);
      break;
    case 2:
      printFiles(num_of_hidden_files, hidden_files);
      break;
     
  }
  puts(""); // Adds a new line!

  //Closing the opened directory
  closedir(directory);
  return 0;
}
