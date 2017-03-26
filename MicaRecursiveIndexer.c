#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

typedef struct _Node{
  char * word=NULL;
  int occurences=0;
  struct _Node * next=NULL;

}Node;

static Node file[26];

int isFile(char * input){
    struct stat inputCheck;
    stat(input, &inputCheck);
    return S_ISREG(inputCheck.st_mode);
}

int isDirectory(char * input){
    struct stat inputCheck;
    if (stat(input, &inputCheck) != 0)
       return 0;
    return S_ISDIR(inputCheck.st_mode);
}

void sort(Node * passed_in_node){

  //going to have to check how strcmp compares numbers
  
  //dynamic allocation of memory for prev and curr pointers to be used during traversal of the LL

  Node* prev = (Node*)malloc(sizeof(Node));
  prev = NULL;
  Node* curr = (Node*)malloc(sizeof(Node)); //curr = current node in the LL
  
  //curr intiialized to the head_of_list

  curr = head_of_list;
  if(head_of_list==NULL){
        head_of_list=passed_in_node;
        return;
    }

  //entering while loop to traverse the existing LL
  while(curr!=NULL){
    
    int comp_length = 0; 
    
    //following code checks to see which word length is smaller between passed_in_node and current node in LL in order to set as "size" in strncmp in order to avoid going out of bounds 
    int curr_length = strlen(curr->word);
    int passed_in_node_length = strlen(passed_in_node->word);
  
    if(curr_length<passed_in_node_length){
      comp_length = passed_in_node_length;
    }

    else{
      comp_length = curr_length;
    }
               
    int comp=strncmp(passed_in_node->word,curr->word, comp_length);
    
    /*following lines of code check to see in what order are the words given in: 
        1. if comp>0, then that means that the passed_in_node->word comes AFTER the curr->word 
            -If this is the case, then the traversal of the LL continues

        2. if comp==0, then that means that both of the words are equal UP TO comp_length
            -further checks are made to see order of the words based on word length;

        3. if comp <0, then that means that passed_in_node->word is supposed to come BEFORE curr->word


    */
    if(comp>0){ 
      prev=curr;
      curr=curr->next;
      
      if(curr == NULL){ // checks to see if end of the LL is reached
        passed_in_node->next = curr;
        prev->next = passed_in_node;
        return;
      } 
    }
    else if(comp==0){
      if(curr_length>passed_in_node_length){
        passed_in_node->next = curr;
        
        if(prev!=NULL){//checks to see where in the LL we are
          prev->next =passed_in_node;
        }
        else{//at head of list
          head_of_list=passed_in_node;
        }
        
        return;
      }
      else{
        passed_in_node->next= curr-> next;
        curr->next = passed_in_node;
        return;
      }
    }else{
         
      if(prev==NULL){// if prev ==NULL, then we are at head of the list
        passed_in_node->next=curr;
        head_of_list =passed_in_node;
        return;
      }
      
      else{
        passed_in_node->next=curr;
        prev->next=passed_in_node; 
        return;
      } 
    }
  }

}


checkDirectory(char * input, int newFile, char * path){
    struct dirent * currEntry=NULL;
    DIR * directory=opendir(input);
    do{
      currEntry=readdir(directory);
      if(currEntry!=NULL){
        if(currEntry->d_type==DT_REG){
          indexFile(input,newFile);
        }
        else if(currEntry->d_type==DT_DIR){
          strcat(path,currEntry->d_name);
          strcat(path,'/');
          checkDirectory(input,newFile);
        }
        else{
          printf(stderr,"ERROR: Input is not a file or a directoy.");
          exit();
        }
      }
    }while(currEntry!=NULL);
    closedir(directory);
}

void createToken(char * input, int newFile){
  char *token=(char*)malloc(sizeof(char));
  char storeChar[1]='\0';
  while(read(newFile,storeChar,1)!=0){
    if(isdigit(storeChar[0])){
      if(token==NULL){
        continue;
      }
    }
    
    if(!isalnum(storeChar[0])){
      indexFile(token,newFile);
      token=NULL;
    }

    memcpy(token,storeChar,1);  
    char * temp=(char*)realloc(token,sizeof(char));
    token=temp;         
    
  }

}

void indexFile(char * token,int newFile){

    //find first letter of file name and go to that index
    //create new node then sort
    Node * newNode=malloc(sizeof(Node));
    newNode->word=tolower(token);
    newNode->occurrences++;
    sort(newNode);
}


int main(int argc, char** argv){

  int newFile=open(argv[1],O_RDWR | O_CREAT);
  if(newFile==-1){
    //file was not created
  }
  //use errno

  if(isFile(argv[2])){
    //open file 
    indexFile(argv[2], newFile);
  }
  else if(isDirectory(argv[2])){
    char path[260];
    strcat(path,"./");
    checkDirectory(argv[2],newFile,path);

  }
  else{
    printf(stderr,"ERROR: Input is not a file or directory.\n");
    return -1;
  }

  return 0;
}