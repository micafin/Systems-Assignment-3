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

Node* head_of_list= NULL;

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
    strcpy(newNode->file,fileName);
    newNode->next=(File*)malloc(sizeof(File));
    newNode->next->file=NULL;
    newNode->occurrences=1;
    return newNode;
}

Node * createNode(char * token){
    Node * newNode=(Node*)malloc(sizeof(Node));
    newNode->word=(char*)malloc(strlen(token));
    strcpy(newNode->word,token);
    newNode->fileNext = (File*)malloc(sizeof(File));
    newNode->fileNext = NULL;
    newNode->next = (Node*) malloc(sizeof(Node));
    newNode->next->word=NULL;
    return newNode;
}

int newStrCmp (const char *s1, const char *s2) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    while (*p1 != '\0') {
        if(*p1>=48&&*p1<=57){
            if(isalpha(*p2)){
                return 1;
            }
        }
        if(*p2>=48&&*p2<=57){
            if(isalpha(*p1)){
                return -1;
            }
        }

        if (*p2 == '\0') return  1;
        if (*p2 > *p1)   return -1;
        if (*p1 > *p2)   return  1;

        p1++;
        p2++;
    }

    if (*p2 != '\0') return -1;

    return 0;
}

void sortFileAlpha(char*filename, int occur, Node* word){
    File* insertNode = createFile(filename);
    insertNode->occurrences = occur;

    File* prev = (File*)malloc(sizeof(File));
    prev = NULL;
    File* ptr = (File*)malloc(sizeof(File)); 
    ptr = word->fileNext;
    File*temp = (File*)malloc(sizeof(File));
    temp = NULL;
    while(ptr->occurrences!= occur){
        if(ptr->next && ptr->next->occurrences ==occur){
            temp = prev;
        }
        prev->next = ptr;
        ptr = ptr->next;
    }

    while(ptr->occurrences== occur){
     
        int comp_length = 0; 
    
        int curr_length = strlen(ptr->file);
        int insert_node_len = strlen(filename);
      
        if(curr_length<insert_node_len){
          comp_length = insert_node_len;
        }
        else{
          comp_length = curr_length;
        }
                   
        int comp=strncmp(insertNode->file,ptr->file, comp_length);
        
        if(comp>0){ 
          prev=ptr;
          ptr=ptr->next;
          
          if(ptr == NULL){ // checks to see if end of the LL is reached
            insertNode->next = ptr;
            prev->next = insertNode;
            return;
          } 
        }
        else if(comp==0){
          if(curr_length>insert_node_len){
            insertNode->next = ptr;
            
            if(prev!=NULL){//checks to see where in the LL we are
              prev->next =insertNode;
            }
            else{//at head of list
              temp->next=insertNode;
            }
            
            return;
          }
          else{
            insertNode->next= ptr-> next;
            ptr->next = insertNode;
            return;
          }
        }else{
             
          if(prev==NULL){// if prev ==NULL, then we are at head of the list
            insertNode->next=ptr;
            temp->next =insertNode;
            return;
          }
          
          else{
            insertNode->next=ptr;
            prev->next=insertNode; 
            return;
          } 
        }
      }
    }

void sortFileNum(Node* word, char * fileName){
    printf("word-> word : %s\n",word->word);
    if(word->fileNext == NULL){
        word->fileNext=createFile(fileName);
        return;
    }
    Node*ptr = (Node*)malloc(sizeof(Node));
    ptr = word;
    File* prev = (File*)malloc(sizeof(File));
    prev = NULL;
    File* curr = (File*)malloc(sizeof(File));
    curr = ptr->fileNext;
    
    while(curr->file!=NULL){
        int comp=newStrCmp(fileName,curr->file);
        //if our file is found, then call sortFileAlpha
        if(comp==0){
            curr->occurrences++;
            sortFileAlpha(fileName, curr->occurrences, word);
            if(prev){
                if(curr->next){
                    prev->next = curr->next;
                }else{
                    prev->next = NULL;
                }
            }else{
                curr = curr->next;
                word->fileNext = curr;
            }
            return;
        }else{
            if(prev==NULL){// if prev ==NULL, then we are at head of the list
                //File * newNode=createFile(fileName);
                sortFileAlpha(fileName,1, word);
                return;
            }
            else{
                //File * newNode=createFile(fileName);
                sortFileAlpha(fileName,1, word);
                return;
            }
        }
    }
    // if there exists no file node such that the filename = the filename we are looking for
    if(curr == NULL){
        prev->next = createFile(fileName);
        if(prev->occurrences !=1){
            return;
        }
        return;
    }
}

Node * sortWord(char * token){
   
   //printf("token: %s\n",token);
    
    Node* prev = (Node*)malloc(sizeof(Node));
    prev = NULL;
    Node* curr = (Node*)malloc(sizeof(Node)); //curr = current node in the LL
    
    //curr intiialized to the head_of_list
    curr = head_of_list;
    if(head_of_list==NULL){
        head_of_list=createNode(token);
         //printf("head %s\n",head_of_list->word);
        return head_of_list;
    }
   
    //entering while loop to traverse the existing LL
    while(curr!=NULL){
        int comp=newStrCmp(token,curr->word);
        Node * newNode=createNode(token);
        if(comp>0){
            //printf("curr word: %s  token: %s\n",curr->word,token);
            prev=curr;
            curr=curr->next;
           // printf("%p\n",curr);
             if(curr->word== NULL){ // checks to see if end of the LL is reached
                 //printf("howdie\n");
                newNode->next = curr;
                prev->next = newNode;
                return newNode;
            }
        }
        else if(comp==0){
            return newNode;
        }
        else if(comp<0){
            if(prev==NULL){// if prev ==NULL, then we are at head of the list
                newNode->next=curr;
                head_of_list=newNode;
                return newNode;
            }
            else{
                newNode->next=curr;
                prev->next=newNode;
                return newNode;
            }
        }
    }

    return NULL;
}
 
void createToken(char * input, char * fileName){
    
    FILE*file = fopen(fileName, "r");
    //printf("dis da input: %s\n", input);
    //printf("entered\n");
    fseek(file,0,SEEK_END);
    long filesz = ftell(file);
    fseek(file,0,SEEK_SET);
    
    fclose(file);
    char * token = (char*)malloc(filesz);
    
    int bytesz = (int) filesz;
    
    //fclose(file);
    token[0] = '\0';
    char curr[1]= "";
    int fd=open(input, O_RDONLY);
    int iterator = 0;
  
    while(read(fd,curr,1)!=0){
        if(isdigit(curr[0])&&iterator == 0){
            iterator++;
            continue;
        }
        if(!isalnum(curr[0])){
            if(iterator == 0){
                continue;
            }
            Node * curr=sortWord(token);
            if(curr==NULL){
              fprintf(stderr,"ERROR: Node is null.\n");
            }
            //sortFileNum(curr,fileName,index);
            //printf("hello\n");
            token = memset(token, 0, strlen(token));
            iterator = 0;
        }else{
            token[iterator] = curr[0];
            if(iterator==bytesz-2){
                token[iterator+1]= '\0';
            }else if(iterator == bytesz-1){
                break;
            }
            iterator++;
        }
    }
    if(iterator!=0){
        Node * curr=sortWord(token);
        sortFileNum(curr,fileName);
        token = memset(token, 0, strlen(token));
        iterator = 0;
    }
       // Node * start=head_of_list;
       //          while(start->word!=NULL){
       //            printf("%s\n",start->word);
       //            start=start->next;
       //          }
    free(token);
    return;
}

void checkDirectory(char * path){
    DIR * directory=opendir(path);
    struct dirent * currEntry=readdir(directory);
    if(directory==NULL){
      return;
    }
    //NEED TO MALLOC
    while((currEntry=readdir(directory))!=NULL){

       printf("IN THIS %s\n",currEntry->d_name);
       //printf("path %s\n",path);
        if(newStrCmp(currEntry->d_name,".DS_Store")==0|| newStrCmp(currEntry->d_name,".")==0 || newStrCmp(currEntry->d_name,"..")==0){
            continue;
        }
        if(currEntry->d_type==DT_REG){
            char * temp=malloc(sizeof(char)*256);
            strcpy(temp,path);
            strcat(temp,"/");
            strcat(temp,currEntry->d_name);
            createToken(path,currEntry->d_name);
            free(temp);
        }
        else if(currEntry->d_type==DT_DIR){
            char * temp2=malloc(sizeof(char)*256);
            strcpy(temp2,path);
            strcat(temp2,"/");
            strcat(temp2,currEntry->d_name);
            //printf("directory: %s\n",path);
            
            checkDirectory(temp2);
            free(temp2);

        }
        else{
            fprintf(stderr,"ERROR: Input is not a file or a directoy.\n");
            exit(1);
        } 
    }

    closedir(directory);
    return;

}

void printList(char* name){
    printf("entered\n");
    FILE * outputFile = fopen(name, "a");
    fprintf(outputFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(outputFile, "<fileIndex>\n");
        Node * ptr = (Node*)malloc(sizeof(Node));
        ptr = head_of_list;
        while(ptr){
            fprintf(outputFile, "\t<word text = \"%s\">\n", ptr->word);
            File* filetrav = (File*)malloc(sizeof(File));
            filetrav = ptr->fileNext;
            if(filetrav){
                while(filetrav){
                    fprintf(outputFile, "\t\t<file name=\"%s\">%d</file>\n", filetrav->file, filetrav->occurrences);
                    filetrav = filetrav->next;
                }
            }
            fprintf(outputFile, "\t</word>\n");
            ptr = ptr->next;
        }
    fprintf(outputFile, "</fileIndex>");
    return;
}

int main(int argc, char** argv){
    
    //use errno
    
    if(isFile(argv[2])){
        //open file 
      createToken(argv[2], argv[2]);
        //createToken(argv[2], newFile, argv[2]);
    }
    else if(isDirectory(argv[2])){
        char path[260];
        //strcat(path,"/");
        strcpy(path,argv[2]);
        checkDirectory(path);
    }
    else{
        fprintf(stderr,"ERROR: Input is not a file or directory.\n");
        return -1;
    }
    return 0;
}
