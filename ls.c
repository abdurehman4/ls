#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int get_no_of_files(DIR ** directory,struct dirent ** dir_entry){
  int count = 0;

  while ((*dir_entry = readdir(*directory)) != NULL)
    ++count;
  return count-2; // Excluding "." and ".."
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


void get_hidden_files(int no_of_files,char * files[])
{
  int num_of_hidden_files = 0;
  for (int i=0; i<no_of_files; i++){
    if (files[i][0] == '.'){
      num_of_hidden_files++;
    }
  }
  printf("No of hidden files is : %d", num_of_hidden_files);
}

int main(int argc,char* argv[])
{
  struct dirent *dir_entry;
  const char *dir_path =  (char *)argv[1];

  // Opening a directory
  DIR * directory = opendir(dir_path);

  if (directory==NULL){
    return 1;
  }

  int count = get_no_of_files(&directory,&dir_entry);

  char * names[count];
  get_files_list(&directory, &dir_entry, names);
  

  get_hidden_files(count,names);
  /*
  for (int i=0; i<count; i++)
  {
    printf("%-10s",names[i]);
  }
 */

  
  // filter_hidden_files(names);
  puts("");

  //Closing the opened directory
  closedir(directory);
  return 0;
}
