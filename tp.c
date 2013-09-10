#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <pwd.h>
#include <string.h>

char *progname;
char str_usage[] = "usage: tp [-u username] filename\x0";

int main(int argc, char *argv[]){
    progname = (char *)getprogname();    
    char *filename, *username;
    int r_ok, w_ok, x_ok;
    int uflag = 0;
    uid_t uid;
    gid_t gid; 
    char perm[] = "---\x0";
    char ch;
    struct passwd *pwd;

    //Search for the -u option and save its argument
    while((ch = getopt(argc, argv, "u:"))!=-1){
        switch(ch){
            case 'u':
                uflag = 1;
		username = optarg;
                break;
            default:
                fprintf(stderr, "%s: illegal option -- %c\n%s\n", progname, str_usage);
                exit(1);   
        }
    }
    
    //Process user flag if specified
    if(uflag){
        //Try to parse username as UID
        if((uid = (uid_t)strtol(username, NULL, 10))==0 && (errno==EINVAL || errno==ERANGE)) {
	    //Try to use username parameter as username
            if((pwd = getpwnam(username)) == NULL){
                fprintf(stderr, "%s: %s: user not found\n", progname, username);
			exit(1);
            }
            uid = pwd->pw_uid;
            gid = pwd->pw_gid;
        } else {
            //Check if user with such UID exists in the system
            if((pwd = getpwuid(uid)) == NULL){
                fprintf(stderr, "%s: %s: user not found\n", progname, username);
                exit(1);
            }
            gid = pwd->pw_gid;
            username = pwd->pw_name;
        }
        //Try to set uid and update the list of groups
        initgroups(username, gid);
        if(setgid(gid)||setuid(uid)){
            fprintf(stderr, "%s: you don't have permission\n", progname);
            exit(1);
        }
    }

    filename = argv[optind];
    //Check for file existance
    if(faccessat(AT_FDCWD, filename, F_OK, 0)!=0){
        fprintf(stderr, "%s: %s: No such file or directory\n", progname, filename);
        exit(1);
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
