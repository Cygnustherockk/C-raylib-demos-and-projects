#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define SPACE " "

bool auto_run = false;
char name[32] = "main";

FilePathList file_path;

#define GET_TIME(time) (float)(clock()-time)/CLOCKS_PER_SEC

int main(int argc, char* argv[])
{
  clock_t time = clock();

  if(argc > 1)
  {
    for(int i = 1; i < argc; i++)
    {
      if(!strcmp(argv[i], "-auto-run"))
      {
        auto_run = true;
        printf("%.3f [INFO]: auto-run enabled! (Note, custom exe commands cannot be used)\n", GET_TIME(time));
      }
      else if(!strcmp(argv[i], "-name"))
      {
        i++;
        strcpy(name, argv[i]);
        printf("%.3f [INFO]: Executable now named '%s'\n", GET_TIME(time), argv[i]);
      }
      else printf("%.3f [WARNING]: argument '%s' not recognized\n",GET_TIME(time), argv[i]);
    }
  }
  printf("%.3f [INFO]: Checking for 'main.c' file...\n", GET_TIME(time));
  if(!FileExists("main.c")){ printf("%.3f [FATAL]: NO main.c SCRIPT FOUND, CANNOT COMPILE, ABORTING\n", GET_TIME(time)); return 1;}
  printf("%.3f [INFO]: Found 'main.c' file!\n", GET_TIME(time));

  char command[256] = "gcc main.c ";
  
  printf("%.3f [INFO]: checking for 'src', directory...\n", GET_TIME(time));
  if(!DirectoryExists("src")) printf("%.3f [WARNING]: No 'src' directory found, no additional scripts will be compiled, if this is not intended, please check your filepath\n", GET_TIME(time));

  else
  { 
    printf("%.3f [INFO]: Found 'src' directory!\n", GET_TIME(time));
    strcat(command, "-I src ");
    file_path = LoadDirectoryFiles("src");
    for(int i = 0; i < file_path.count; i++)
    {
      if(IsFileExtension(file_path.paths[i], ".c"))
      {
        strcat(command, file_path.paths[i]);
        strcat(command, SPACE);
      }
    }
  }


  printf("%.3f [INFO]: Checking for 'lib', directory...\n", GET_TIME(time));
  if(!DirectoryExists("lib")) printf("%.3f [WARNING]: No 'lib' directory found, this will be needed to compile raylib, if this is not intended, please check your filepath\n", GET_TIME(time));
  else
  {
    printf("%.3f [INFO]: 'lib' directory found!\n", GET_TIME(time));
    strcat(command, "-L lib -lraylib -lgdi32 -lwinmm ");
  }

  strcat(command, "-Wall -Wextra -Wpedantic -o ");
  
  strcat(command, name);

  printf("%.3f [INFO]: Compiling main.c with command: %s\n", GET_TIME(time), command);
  clock_t c_time = clock();
  int success = system(command);

  if(success == 0)printf("%.3f [INFO]: Build compiled succesfully after %.3f seconds!\n",GET_TIME(time), GET_TIME(c_time));
  
  else{ printf("%.3f [FATAL]: Compilation failed! Please check Compile error log\n", GET_TIME(time)); return 1;}
  

  if(auto_run == 1)
  {
    printf("%.3f [INFO]: Auto-running running %s\n", GET_TIME(time), name);
    system(name);
  }
  else printf("%.3f [INFO]: Build complete, exiting\n", GET_TIME(time));

  return 0;
}