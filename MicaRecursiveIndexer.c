#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

indexFile(){
	while(user_inputted_String[iterator]!='\0'){
    
    if(!isalpha(user_inputted_String[iterator])){

      new_node=createNode(start,iterator,user_inputted_String);
    
      if(start==0){
        head_of_list=new_node;
      }
      else{
        sort(new_node);
      }

      start=iterator+1;
      
    }
    iterator++;
          
    if(user_inputted_String[iterator]=='\0'){
      
      new_node=createNode(start,iterator,user_inputted_String);
      
      if(new_node!=NULL){
        sort(new_node);
      }
      break;
    }
                  
    
  }

}


int main(int argc, char** argv){
	if(file){
		//open file 
		indexFile();
	}
}