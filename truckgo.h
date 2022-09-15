#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <dirent.h> 
#include <sys/stat.h> 
#include <unistd.h>
#include <errno.h>

#define VERSION "1.0.0 (Release)"
/* NOTE: this has to be manually configured */
#define CONFIG "~/.truckgo"

/* Declarations */
void help (void);
int new (char * name);
void build (int release);
/* void run (int release); */
char * getConf (void);
char * getFiles (void);

/* Globals */
char * cc;
char * debug;
char * release;
char * dir;

/* Implementations */
void help (void) {
  printf("Version: %s\n", VERSION);
  puts("Usage: truckgo [options]");
  puts("Options: help, new, build");
  puts("help: print this message");
  puts("new [name]: initializes a new truckgo build directory");
  puts("build [debug | release]: builds a debug or release version, and stores the result in gen/");
}

/* Dies on failure */
int new (char * name) {
  int ret = mkdir(name);
  if (ret == -1) {
    switch (errno) {
    case EACCES:
      fputs("Could not create base directory", stderr);
      exit(EXIT_FAILURE);
    case EEXIST:
      fputs("Directory with same name exists", stderr);
      exit(EXIT_FAILURE);
    case ENAMETOOLONG:
      fputs("Directory name is too long", stderr);
      exit(EXIT_FAILURE);
    default:
      perror("Directory creation");
      exit(EXIT_FAILURE);
    }
  }
  puts("Base directory created");
  
  chdir(name);
  /* This has errno stuff, but if we could create the first, we can create the rest */
  mkdir("src");
  mkdir("gen");
  puts("src and gen directories created");
  chdir("gen");
  mkdir("debug");
  mkdir("release");
  puts("debug and release directories created");
  chdir("../../");
  return 0;
}

void build (int releaseflag) {
  char * syscall;
  char * files = getFiles();
  chdir("src");
  if (releaseflag == 1) {
    syscall = malloc(strlen(cc) + strlen(release) + strlen(files) + strlen(dir) + 20);
    strcpy(syscall, cc);
    /* This may be inefficient */
    strcat(syscall, " ");
    strcat(syscall, release);
    strcat(syscall, " ");
    strcat(syscall, files);
    strcat(syscall, "-o ../gen/release/");
    strcat(syscall, dir);
    puts(syscall);
    system(syscall);
  } else {
    syscall = malloc(strlen(cc) + strlen(debug) + strlen(files) + strlen(dir) + 18);
    strcpy(syscall, cc);
    /* This may be inefficient */
    strcat(syscall, " ");
    strcat(syscall, debug);
    strcat(syscall, " ");
    strcat(syscall, files);
    strcat(syscall, "-o ../gen/debug/");
    strcat(syscall, dir);
    puts(syscall);
    system(syscall);
  }
  free(syscall);
  free(files);
}

/* void run (int releaseflag) { */
/*   //read in contents of src/ and compile 'em, then run */
/*   char * directive; */
/*   directive = malloc(strlen(dir) + 20); */
/*   if (releaseflag == 1) { */
/*     build(1); */
/*     strcpy(directive, "./gen/release/"); */
/*     strcat(directive, dir); */

/*     puts(directive); */
/*     system(directive); */
/*   } else { */
/*     build(0); */
/*     strcpy(directive, "./gen/debug/"); */
/*     strcat(directive, dir); */
/*     puts(directive); */
/*     system(directive); */
/*   } */
/*   releaseflag++; */
/* } */

/*
  Returns memory region with options. Each option is separated with a comma
  If the file fails to open, the program will error and exit, dumping an example of the
  file contents
*/
char * getConf (void) {
  int regionsize = 300;
  char * retregion = malloc(regionsize);
  FILE * config = fopen(CONFIG, "r");
  if (errno != 0) {
    perror("Error opening configuration file");
    exit(1);
  }
  int c, counter, line = 0;
  while ((c = fgetc(config)) != EOF) {
    if (counter == 300) {
      regionsize += 50;
      retregion = realloc(retregion, regionsize);
    }
    if (c == '\n') {
      if (line < 2) {
	line++;
	c = ',';
      } else {
	c = '\0';
      }
    }
    *(retregion + counter) = c;
    counter++;
  }
  *(retregion + (regionsize - 1)) = '\0';
  return retregion;
}

char * getFiles (void) {
  struct dirent *dp;
  DIR * dfd = opendir("src");
  int tracker = 0;
  /* 5 kib worth of names should be enough for anyone! */
  char * filelist = malloc(1024 * 5);
  *(filelist + 1023) = '\0';
  if (dfd != NULL) {
    while ((dp = readdir(dfd)) != NULL) {
      if ((strcmp(dp->d_name, ".") ==  0) || (strcmp(dp->d_name, "..") == 0)) {
	continue;
      }

      /* Only get the C files */
      if (*(dp->d_name + strlen(dp->d_name) - 1) == 'c') {
	if (tracker == 0) {
	  strcpy(filelist, dp->d_name);
	  strcat(filelist, " ");
	  tracker++;
	} else {
	  strcat(filelist, dp->d_name);
	  strcat(filelist, " ");
	}
      }
    }
  }
  closedir(dfd);
  return filelist;
}
