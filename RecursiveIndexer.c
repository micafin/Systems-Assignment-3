#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef struct _File{
    int occurrences;
    char * file;
    struct _File * next;
}File;

typedef struct _Node{
    char * word;
    File * fileNext;
    struct _Node * next;
    
}Node;

static Node * file[26];

int isFile(char * input){
    struct stat path_stat;
    stat(input, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int isDirectory(char * input){
    struct stat statbuf;
    if (stat(input, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
}

File * createFile(char * fileName){
    File * newNode=(File*)malloc(sizeof(File));
    newNode->file=(char*)malloc(strlen(fileName));
    newNode->file=fileName;
    newNode->next=(File*)malloc(sizeof(File));
    newNode->next=NULL;
    newNode->occurrences=1;
    return newNode;
}

Node * createNode(char * token){
    Node * newNode=(Node*)malloc(sizeof(Node));
    newNode->word=(char*)malloc(strlen(token));
    newNode->word=token;
    newNode->fileNext = (File*)malloc(sizeof(File));
    newNode->fileNext = NULL;
    newNode->next = (Node*) malloc(sizeof(Node));
    return newNode;
}

void sortFile(Node * pointer, char * fileName, int index){
    
    if(pointer->fileNext==NULL){
        pointer->fileNext=createFile(fileName);
        return;
    }

    File* prev = (File*)malloc(sizeof(File));
    prev = NULL;
    File* curr = (File*)malloc(sizeof(File));
    curr = pointer->fileNext;
    while(curr!=NULL){
        
        int comp=strcmp(fileName,curr->file);
        if(comp>0){
            prev=curr;
            curr=curr->next;
            if(curr == NULL){ // checks to see if end of the LL is reached
                File * newNode=createFile(fileName);
                newNode->next = curr;
                prev->next = newNode;
                return;
            }
        }
        else if(comp==0){
            curr->occurrences++;
            return;
        }else{
            
            if(prev==NULL){// if prev ==NULL, then we are at head of the list
                File * newNode=createFile(fileName);
                newNode->next=curr;
                pointer->fileNext=newNode;
                return;
            }
            else{
                File * newNode=createFile(fileName);
                newNode->next=curr;
                prev->next=newNode;
                return;
            }
        }
    }
}

Node * sortWord(char * token, char * fileName, int index){
    
    if(file[index]==NULL){
        file[index]=createNode(token);
        return file[index];
    }
    Node* prev = (Node*)malloc(sizeof(Node));
    prev = NULL;
    Node* curr = (Node*)malloc(sizeof(Node)); //curr = current node in the LL
    
    //curr intiialized to the head_of_list
    
    curr = file[index];
    
    //entering while loop to traverse the existing LL
    while(curr!=NULL){
        int comp=strcmp(token,curr->word);
        Node * newNode=createNode(token);
        if(comp>0){
            prev=curr;
            curr=curr->next;
            
            if(curr == NULL){ // checks to see if end of the LL is reached
                newNode->next = curr;
                prev->next = newNode;
                return newNode;
            }
        }
        else if(comp==0){
            return newNode;
            // }
        }else{
            
            if(prev==NULL){// if prev ==NULL, then we are at head of the list
                newNode->next=curr;
                file[index]=newNode;
                return newNode;
            }
            else{
                newNode->next=curr;
                prev->next=newNode;
                return newNode;
            }
        }
    }
}

void createToken(char * input, int newFile, char * fileName){
    //char *token=(char*)malloc(sizeof(char));
   FILE* file = fopen(fileName, "r");;
    fseek(file,0,SEEK_END);
    long filesz = ftell(file);
    fseek(file,0,SEEK_SET);

    char * token = (char*)malloc(sizeof(char)*filesz);
    token[0] = '\0';
    char curr[1]= "";
    int fd=open(input, O_RDONLY);
    int iterator = 0;
   // printf("hello\n");
    while(read(fd,curr,1)!=0){
        if(isdigit(curr[0])&&iterator == 0){
            continue;
        }
        if(!isalnum(curr[0])){
            if(iterator == 0){
                continue;
            }
            int index=token[0]-'a';
           // printf("%s\n",token);
            Node * curr=sortWord(token,fileName,index);
            sortFile(curr,fileName,index);
            token = memset(token, 0, strlen(token));
            iterator = 0;
        }else{
            token[iterator] = tolower(curr[0]);
            iterator++;
            //printf("%s\n",token);
        }
    }
    if(iterator!=0){
        int index=token[0]-'a';
        printf("%s\n",token);
        sortWord(token,fileName,index);
        sortFile(fileName,index);
        token = memset(token, 0, strlen(token));
        iterator = 0;
        
    }
}

void checkDirectory(int newFile, char * path){
    DIR * directory=opendir(path);
    struct dirent * currEntry;
    char * temp=malloc(sizeof(char)*256);//NEED TO MALLOC
    while((currEntry=readdir(directory))!=NULL){
        if(strcmp(currEntry->d_name,".DS_Store")==0|| strcmp(currEntry->d_name,".")==0 || strcmp(currEntry->d_name,"..")==0){
            continue;
        }
        if(currEntry->d_type==DT_REG){
            printf("hello\n");
            printf("%s\n",currEntry->d_name);
            strcat(temp,currEntry->d_name);
            createToken(path,newFile,temp);
        }
        else if(currEntry->d_type==DT_DIR){
            strcat(path,"/");
            strcat(path,currEntry->d_name);
            printf("%s\n",currEntry->d_name);
            checkDirectory(newFile, path);
        }
        else{
            fprintf(stderr,"ERROR: Input is not a file or a directoy.\n");
            exit(1);
        }
        closedir(directory);
    }

}

int main(int argc, char** argv){
    
    //int newFile=open(argv[1], O_RDWR | O_CREAT);
    int newFile=0;
    if(newFile==-1){
        //file was not created
    }
    //use errno
    
    if(isFile(argv[2])){
        //open file 
        createToken(argv[2], newFile, argv[2]);
    }
    else if(isDirectory(argv[2])){
        char path[260];
        strcat(path,"./");
        strcat(path,argv[2]);
        checkDirectory(newFile, path);
    }
    else{
        fprintf(stderr,"ERROR: Input is not a file or directory.\n");
        return -1;
    }
    return 0;
}
