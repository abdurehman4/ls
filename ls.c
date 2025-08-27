#include <stdio.h>    // For printing text
#include <dirent.h>   // For getting list of members of a directory
#include <string.h>   // For using functions like strcmp
#include <sys/stat.h> // For differentiating between directories and files
#include <stdlib.h>


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
    // printf("%s is : %d\n",(*dir_entry)->d_name, isDirectory("~/C/"));
    files[i++] = (*dir_entry)->d_name;
  }}
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

int isDirectory(char * path)
{
  struct stat statbuf;
  if (stat(path, &statbuf) != 0) { //The stat() function obtains information about the named file and writes it to the area pointed to by the buf argument. 
        // (Failure Case) Path does not exist or an error occurred
        puts("Error while accessing the file/directory.");
        return 1;
    }
  return S_ISDIR(statbuf.st_mode);
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
  int mode = 0;
  // Dealing with args
  for (int i=1; i<argc; i++)
  {
    const char* arg = argv[i];
    if (arg[0] != '-') {
      dir_path = (char*)malloc(sizeof(arg)); // Allocating some memory to the pointer
      strcpy(dir_path, arg);
    }
    else if (arg[0] == '-'){
      if (strcmp("-a",arg) == 0){
        mode = 1;
      }
    }
  }

  // Opening a directory
  DIR * directory = opendir(dir_path);

  if (directory==NULL){
    printf("Error while opening the given directory!");
    return 1;
  }

  int count = get_no_of_files(&directory,&dir_entry);

  char * names[count];
  get_files_list(&directory, &dir_entry, names);
  
  int num_of_hidden_files = get_no_of_hidden_files(count,names);

  char * hidden_files[num_of_hidden_files];
  get_hidden_files(count, names, hidden_files);
  // puts("Switch");
  switch (mode) {
    case 0:
      printFiles(count, names);
      break;
    case 1:
      printFiles(num_of_hidden_files, hidden_files);
      break;
     
  }
  puts(""); // Adds a new line!

  //Closing the opened directory
  closedir(directory);
  return 0;
}
