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
    newNode->occurrences=1;
    return newNode;
}

Node * createNode(char * token){
    Node * newNode=(Node*)malloc(sizeof(Node));
    newNode->word=(char*)malloc(strlen(token));
    newNode->word=token;
    return newNode;
}

void sortFile(char * fileName, int index){
    
    if(file[index]->fileNext==NULL){
        file[index]->fileNext=createFile(fileName);
        return;
    }
    File* prev = (File*)malloc(sizeof(File));
    prev = NULL;
    File* curr = (File*)malloc(sizeof(File));
    curr = file[index]->fileNext;
    //curr = current node in the LL
    
    //entering while loop to traverse the existing LL
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
                file[index]->fileNext=newNode;
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

void sortWord(char * token, char * fileName, int index){
    if(file[index]==NULL){
        file[index]=createNode(token);
        return;
    }
    Node* prev = (Node*)malloc(sizeof(Node));
    prev = NULL;
    Node* curr = (Node*)malloc(sizeof(Node)); //curr = current node in the LL
    
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
                return;
            }
        }
        else if(comp==0){
            return;
            // }
        }else{
            
            if(prev==NULL){// if prev ==NULL, then we are at head of the list
                newNode->next=curr;
                file[index]=newNode;
                return;
            }
            else{
                newNode->next=curr;
                prev->next=newNode;
                return;
            }
        }
    }
}

void createToken(char * input, int newFile, char * fileName){
    char *token=(char*)malloc(sizeof(char));
    char storeChar[1]="";
    int fd=open(input, O_RDONLY);
    while(read(fd,storeChar,1)!=0){
        if(isdigit(storeChar[0])){
            if(token==NULL){
                continue;
            }
        }
        if(!isalnum(storeChar[0])){
            if(token==NULL){
                continue;
            }
            int index=token[0]-'a';
            sortWord(token,fileName,index);
            sortFile(fileName,index);
            token=NULL;
        }
        storeChar[0]=tolower(storeChar[0]);
        memcpy(token,storeChar,1);
        char * temp=(char*)realloc(token,sizeof(char)*strlen(token)+sizeof(char));
        token=temp;
    }
}

void checkDirectory(int newFile, char * path){
    struct dirent * currEntry=NULL;
    DIR * directory=opendir(path);
    char * temp;//NEED TO MALLOC
    do{
        currEntry=readdir(directory);
        if(currEntry!=NULL){
            strcat(path,"/");
            strcat(path,currEntry->d_name);
            strcat(temp,currEntry->d_name);
            if(currEntry->d_type==DT_REG){
                createToken(path,newFile,temp);
            }
            else if(currEntry->d_type==DT_DIR){
                checkDirectory(newFile, path);
            }
            else{
                fprintf(stderr,"ERROR: Input is not a file or a directoy.");
                exit(1);
            }
        }
    }while(currEntry!=NULL);
    closedir(directory);
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
        strcat(path,".");
        checkDirectory(newFile, path);
        
    }
    else{
        fprintf(stderr,"ERROR: Input is not a file or directory.\n");
        return -1;
    }
    return 0;
}
