#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>
#include <stdio.h>

using namespace std;

bool letsgo(string lets, char **argv){
  char tab2[1024];
  char** new = strcpy(tab2, lets.c_str());
  int pid = fork();
  if (pid== -1) {
    perror("fork fail");
    return false;
  }
  else if (execvp(new, argv)!=0) perror("error in execvp");
  else if(waitpid(pid, NULL, 0) == -1) return false;
  else return true;
}

int main(int argc, char **argv)
{
  cout << "$"; //print command prompt
  string cmd;
  cin >> cmd; //get user command
  while(cmd!="exit"){
  
    int lastc = -1; //records last command, -1 means no last command 0 means ; 1 means && 2 means ||
    int nextc = -1; //records next command, uses same notation as lastc
    bool lasts = false; //records if last command failed or not, true for succeeded false for failed
    
    while(cmd!=""){
      if(cmd.find('#')!=-1) cmd.erase(cmd.find('#'),-1); //anything after # is just a comment so you can just delete it
      int i=cmd.find_first_of(";|&");
      string cmd0=cmd;
      if(i!=-1){ //if there's another operator in the line
        if(cmd.at(i)==';' || (cmd.at(i)=='&' && cmd.at(i+1)=='&') || (cmd.at(i)=='|' && cmd.at(i+1)=='|'){
          cmd0 = cmd.substr(0,i); //cmd0 is the next command
          if(cmd.at(i)==';') {
            cmd.erase(0,i+1); //trim cmd if the operator was a ;
            nextc=0;
          }
          else if(cmd.at(i)=='&'){
            cmd.erase(0,i+2); //trim if operator was &&
            nextc=1;
          }
          else if(cmd.at(i)=='|'){
            cmd.erase(0,i+2); //trim if operator was ||
            nextc=2;
          }
        }
      }
      else nextc=-1; //since there's no operator on the line
      
      //now we have our cmd0 which is the command we're going to use for now
      
      if(cmd0.find("exit")!=-1) break;
      
      if(lastc==-1||lastc==0) letsgo(cmd0, argv); //last command was a semicolon or no previous command so just run
      else if(lastc==1){ //last command was &&
        if(lasts==true) { //and the last command succeeded so run
          lasts = letsgo(cmd0, argv);
        } 
      }
      else if(lastc==2){ //last command was ||
        if(lasts==false) { //and the last command failed so run
          lasts = letsgo(cmd0, argv);
        }
      }
      lastc=nextc;
    }
    if(cmd0.find("exit")!=-1) break;
    cout << "$";
    cin >> cmd;
  }
  
  return 0;
}

