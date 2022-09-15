#include "truckgo.h"


/* Displacements */
#define CCDISPLACE 3
#define DEBUGDISPLACE 6
#define RELEASEDISPLACE 8

int main (int argc, char * argv[]) {
  if (argc < 2) {
    help();
    return 1;
  }
  if (strcmp(argv[1], "help") == 0) {
    help();
    return 0;
  }

  char * options = getConf();
  int track = 0;
  
  while (*(options + track) != ',') {
    track++;
  }
  
  *(options + track) = '\0';
  cc = options + CCDISPLACE;

  debug = options + track + 1;
  track = 0;
  while (*(debug + track) != ',') {
    track++;
  }
  *(debug + track) = '\0';
  debug = debug + DEBUGDISPLACE;

  release = debug + track + 1;
  release = release + RELEASEDISPLACE;

  /* Get the name of the project */
  dir = malloc(1024);
  dir = getenv("PWD");
  int end = strlen(dir);
  while (*(dir + end) != '/') {
    end--;
  }
  dir = dir + end + 1;
  
  errno = 0;
  
  if (strcmp(argv[1], "new") == 0) {
    if (argc < 3) {
      fprintf(stderr, "Error: no new filename provided\n");
      help();
    }
    new(argv[2]);
  }
  
  if (strcmp(argv[1], "build") == 0) {
    if (argc == 3) {
      if (strcmp(argv[2], "release") == 0) {
	build(1);
	puts("Finished building release");
      } else {
	build(0);
	puts("Finished building debug");
      }
    } else {
      build(0);
      puts("Finished building debug");
    }
  } else {
    /* Last call for correct arguments! */
    fprintf(stderr, "Error: incorrect argument(s) given\n");
    help();
    return 1;
  }


  /* if (strcmp(argv[1], "run") == 0) { */
  /*   if (argc == 3) { */
  /*     if (strcmp(argv[2], "release") == 0) { */
  /* 	run(1); */
  /*     } else {  */
  /* 	run(0); */
  /*     } */
  /*   } else { */
  /*     run(0); */
  /*   } */
  /*   return 0; */
  free(options);
  return 0;
}
