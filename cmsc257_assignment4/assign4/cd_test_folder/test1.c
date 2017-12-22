#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<stdio.h>
#include <sys/select.h>
#include <signal.h>

//pid_t pid, wpid;

int cd(char **words);

char*builtin_str[] ={
        "cd"
};

int (*func[]) (char**) = {
        &cd
};

int num_choices(){
        return sizeof(builtin_str)/ sizeof(char*);
}

int cd(char **words) {

}

int cd(char **words)
{char cwd[512];
        if (chdir(words[1]) !=0 ){
                        perror("not dir");
                 }
        return 1;
}



char** parseString(char* input){
        char **tokens = malloc(50*sizeof(char*));
        char *token;
        int pos=0;
        token = strtok(input," \t\r\n\a");
        while(token!=NULL){
                tokens[pos] = token;
                pos++;
                token = strtok(NULL," \t\r\n\a");
        }
        tokens[pos] = NULL;
        return tokens;
}

int forks(char ** words){
        pid_t pid, wpid;
        int status;

        pid = fork();

        if (pid == 0){
                execvp(words[0], words);
        exit(0);
        }
        else if (pid<0){
                perror("forks error");
        }
        else{
                //printf("child %d, parent is %d\n",pid, getppid());
                do{
                        wpid = waitpid(pid, &status, WUNTRACED);
                } while (!WIFEXITED(status) && !WIFEXITED(status));
        }
        return 1;
}

int run(char ** words){
 int i;
        if(words[0]==NULL){
                return 1;
        }

        for (i = 0; i<num_choices(); i++) {
                if(strcmp(words[0],builtin_str[i]) == 0){
                        return(*func[i])(words);
                }
        }

        return forks(words);
}


int main(int argc, char **argv){
        char*bash; //input[1000];
        char * input = malloc(50*sizeof(char));
        char ** words = malloc(10*sizeof(char*));
        struct sigaction new_action, old_action;


        //signal(SIGINT,sig_func);
        if (argc >= 2 && strcmp(argv[1],"-p")== 0){// check for commandline inputs
                bash = argv[2];
                if (bash == NULL)
                        bash = "my257sh";
                while(strcmp(input,"exit")!=0){
                        printf("%s> ",bash);
                        fgets(input, 64 ,stdin);
                        input[strcspn(input,"\n")]= '\0';
                        words = parseString(input);
                        run(words);
                }
        }
        else {
                printf("Command line inputs must be in: excutable -p <Name>\n");
        }


}
