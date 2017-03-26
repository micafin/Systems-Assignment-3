#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

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

checkDirectory(char * input, int newFile){
    struct dirent * currEntry=NULL;
    DIR * directory=opendir(input);
    do{
      currEntry=readdir(directory);
      if(currEntry!=NULL){
        if(currEntry->d_type==DT_REG){
          indexFile(input,newFile);
        }
        else if(currEntry->d_type==DT_DIR){
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
  char *token;
  char storeChar[1]='\0';
  while(read(newFile,storeChar,1)!=0){
    
    if(!isalnum(storeChar[0])){

      new_node=createNode(start,iterator,user_inputted_String);
    
      if(start==0){
        head_of_list=new_node;
      }
      else{
        sort(new_node);
      }

      start=iterator+1;
      
    }

    memcpy(token,storeChar,1);
          
    // if(user_inputted_String[iterator]=='\0'){
      
    //   new_node=createNode(start,iterator,user_inputted_String);
      
    //   if(new_node!=NULL){
    //     sort(new_node);
    //   }
    //   break;
    // }             
    
  }

}
void indexFile(char * input, int newFile){


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
    checkDirectory(argv[2],newFile);

  }
  else{
    printf(stderr,"ERROR: Input is not a file or directory.\n");
    return -1;
  }

  return 0;
}