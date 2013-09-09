#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

char *progname;

int main(int argc, char *argv[]){
    progname = (char *)getprogname();    
    char *filename;
    int r_ok, w_ok, x_ok;
    char perm[] = "---\x0";

    if(argc<2){
	printf("usage: tp file\n");
        return 1;
    } else if(argc>2){
        fprintf(stderr, "%s: Too many arguments\n", progname);
    }
    
    filename = argv[1];

    //Check for file existance
    if(faccessat(AT_FDCWD, filename, F_OK, 0)!=0){
        fprintf(stderr, "%s: %s: No such file or directory\n", progname, filename);
        return 1;
    }
    
    r_ok = faccessat(AT_FDCWD, filename, R_OK, 0);
    w_ok = faccessat(AT_FDCWD, filename, W_OK, 0);
    x_ok = faccessat(AT_FDCWD, filename, X_OK, 0);
    if(r_ok==0){
         perm[0] = 'r';
    }
    if(w_ok==0){
         perm[1] = 'w';
    }
    if(x_ok==0){
         perm[2] = 'x';
    }
    printf("%s\n", perm);
    return 0;
}
