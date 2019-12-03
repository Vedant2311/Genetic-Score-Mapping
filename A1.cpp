//KEY
//## update Change
//$$ Debug change revert after debugging
// #include<iostream>
// #include<string>
// #include<fstream>
// #include<map>
// #include<cmath>
// #include<vector>
#include<bits/stdc++.h>
using namespace std;
//CONSTANTS
const int MAX_VSIZE = 10000;
const int MAX_K = 10000;
//Variables
float tim;
int vsize,cc;
char v[MAX_VSIZE];
map<char,int> vmap;
int mc[MAX_VSIZE+1][MAX_VSIZE+1];


/*
  Thought Process
  1.a funciton to give a starting State i.e. a solution wiht all strings equal
  for now we can take as fill with dash
  2.Change equal lenth strings to get better strings
  NEW THOUGHT think as k bit numbers for one move then move till reach corner
  assign weight to each path alternative then compute shortest path using IDDFS
  for local search change positions of dash and occasionally increase length of all strings by adding a dash
  problem when going out of index
*/




void makeMap(){
  int n=vsize;
  for(int i=0;i<n;i++)
    vmap[v[i]]=i;
  vmap['-']=n;
}

// int getVIndex(char c){
//   for(int i=0;i<vsize;i++)
//     if(v[i]==c)
//       return i;
//   if(c=='-')
//     return vsize;
//   else{
//     throw "Invalid Vocabulary";
//   }
// }

class State{
public:
  vector<string> x;
  int k;
  int index=0;  //curr working index
  int dash=0;   //string at which dash shoud be put
  int maxlength;
  vector<string> y;


  int maxLen(vector<string> &v){
    int max=v[0].length();
    for(int i=1;i<v.size();i++)
      if(v[i].length()>max)
        max=v[i].length();
    return max;
  }

  void padDash(vector<string> &v){
    int max=maxLen(v);

    for(int i=0;i<v.size();i++){
      int diff=max-v[i].length();
      for(int j=0;j<diff;j++){
        v[i].insert(v[i].length(),"-");
      }
    }
    // cout<<"========"<<endl;
    // for(int i=0;i<v.size();i++)
    //   cout<<v[i]<<endl;
    // cout<<"********* "<<getIndexCC(v,v[0].length())+getIndexMC(v,v[0].length())<<endl;
  }

  // //constructor
  // State(int k){
  //   int
  //   int max=x[0].length();
  //   for(int i=1;i<x.size();i++)
  // }

  void setMaxLength(){
      int max=x[0].length();
      for(int i=1;i<x.size();i++)
        if(x[i].length()>max)
          max=x[i].length();
      maxlength=max;
  }

  int getTC(){
    return getCC()+getMC();
  }

  int getCC(){
    int cost=0;
    for(int i=0;i<k;i++)
      for(int j=0;j<x[i].length();j++)
        if(x[i][j]=='-')
          cost+=cc;
    return cost;
  }

  int get2MC(string x1,string x2){
    int n =x1.length();
    int m=x2.length();
    if(n!=m){
      throw "Strings not equal";
    }
    int cost=0;
    for(int i=0;(i<n)&&(i<m);i++){
      cost+=mc[vmap.find(x1[i])->second][vmap.find(x2[i])->second];
    }
    // if(m<n){  //assumig dashes
    //   cost+=cc*(n-m);
    // }
    // else{   //assuming dashes
    //   cost+=cc*(m-n);
    // }
    return cost;
  }

  int getMC(){
    int cost=0;
    for(int i=0;i<x.size()-1;i++){
      for(int j=i+1;j<x.size();j++){
        cost+=get2MC(x[i],x[j]);
      }
    }
    return cost;
  }

  int getIndex2MC(string x1,string x2,int index){  //get upto cost upto length l
    if(x2.length()<index||x1.length()<index){
      throw "String(s) less than length";
    }
    int cost=0;
    for(int i=0;i<index;i++)
      cost+=mc[vmap.find(x1[i])->second][vmap.find(x2[i])->second];
    return cost;
  }

  int getIndexCC(vector<string> &x,int index){
    int cost=0;
    for(int i=0;i<k;i++)
      for(int j=0;j<index;j++)
        if(x[i][j]=='-')
          cost+=cc;
    return cost;
  }

  int getIndexMC(vector<string> &x, int index){
    int cost=0;
    for(int i=0;i<x.size()-1;i++){
      for(int j=i+1;j<x.size();j++){
        cost+=getIndex2MC(x[i],x[j],index);
      }
    }
    return cost;
  }

  int getStepMC(vector<char> c){
    int cost=0;
    for(int i=0;i<c.size()-1;i++){
      for(int j=i+1;j<c.size();j++){
        cost+=mc[vmap.find(c[i])->second][vmap.find(c[j])->second];
      }
    }
    return cost;
  }
//Trying to deploy proper A*

  int h(){//no of steps to make all strings equal to maximum length string
    int heu=0;
    int max=x[0].length();
    for(int i=1;i<k;i++){
      int len=x[i].length();
      if(len<=max)
        heu+=cc*(max-len);
      else{
        heu+=cc*i*(len-max);
        max=len;
      }
    }
    return heu;
  }

  int g(){  //step cost upto the current step
    return getIndexMC(x,index+1)+getIndexCC(x,index+1);//#Can change accordingly
  }

  int f(){
    return h();
  }

  // int h(int dash){//no of steps to make all strings equal to maximum length string
  //   int heu=0;
  //   int max=x[0].length()+dash&1;
  //   for(int i=1;i<k;i++){
  //     int len=x[i].length()+((dash>>i)&1);
  //     if(len<=max)
  //       heu+=cc*(max-len);
  //     else{
  //       heu+=cc*i*(len-max);
  //       max=len;
  //     }
  //   }
  //   return heu;
  // }

  int h(int dash){//no of steps to make all strings equal to maximum length string
    updateY(dash);
    padDash(y);
    return getIndexCC(y,y[0].length())+getIndexMC(y,y[0].length());
  }

  int g(vector<char> c ,int dash){  //step cost upto the current step
    int tcc=0;
    for(int i=0;i<k;i++)
      tcc+=cc*((dash>>i)&1);
    // if(tcc>(k-1)*cc/2)
    //   tcc=INT8_MAX; //Never put dash at all places
    return tcc+getStepMC(c);//#Can change accordingly tcc is cc and rest is mc
  }

  vector<State*> getChildren(){
    int noc=(k*2)-1; //excluding all dashesh
    vector<State*> v(noc);  //vector of size 2^k
    for(int i=0;i<noc;i++){
      v[i] = new State();
      v[i]->k=k;
      v[i]->index=index;
      v[i]->dash=i;
      v[i]->x=x;
      v[i]->update(); //updates according to dash
    }
    return v;
  }

  void bestChild(){
    //int dash=0;
    int min=INT8_MAX;
    int min_dash=0;
    int score=0;
    bool invalid =false;
    vector<char> curr(k);//for dash< something update dash then calculate cost interate over all dash values then find minimum cost
    for(int dash=0;dash<(k*2)-1;dash++){
      for(int i=0;i<k;i++){
        if((dash>>i)&1)
          curr[i]='-';
        else if(index<x[i].length()){  //discard dash if string end reached
          curr[i]=x[i][index];
        }
        else{
          invalid=true;
          break;
        }
      }
      if(invalid){
        invalid =false;
        continue;
      }
      //get score of curr
      score=h(dash);//+g(curr,dash);
      if(score<=min){
        min=score;
        min_dash=dash;  //loal dash
      }
    }
    //update   global dash
    dash=min_dash;  //best transformation for next child
  }
//bit representaion of dash is which palces to insert dash upto k
  void updateY(int dash){
    y=x;
    if(dash>=(k*2))
      return;
    for(int i=0;i<k;i++)
      if((dash>>i)&1){
        if(y[i][index]=='-')
          continue;
        else
          y[i].insert(index,"-");
      }
  }
  void update(){
    if(dash>=(k*2)-1){
      index++;
      cout<<"Returning"<<endl;
      return;
    }
    for(int i=0;i<k;i++)
      if((dash>>i)&1){
        if(x[i][index]=='-')
          continue;
        else
          x[i].insert(index,"-");
      }
    index++;
  }


  bool goalTest(){  //if all strings are of same length
    return index>=maxLen(x);
  }


 };

//  class myComparator
//  {
//  public:
//      int operator() (State*& s1,State*& s2)
//      {
//          return s1->f() > s2->f();
//      }
//  };
//
// priority_queue <State*, vector<State*>, myComparator > pq;
//
// void AStar(State *st){
//   int noc=(1<<(st->k))-1;
//   vector<State*> children(noc);
//   pq.push(st);
//   while(!pq.top()->goalTest()){//add time termination condition
//     //$$ to check how proceeding
//     children=pq.top()->getChildren();
//     pq.pop();
//     for(int i=0;i<noc;i++)
//       pq.push(children[i]);
//   }
//   st=pq.top();//make st as the final state
// }

void AStar1(State *st, clock_t startt, float tim){
    
    clock_t endt;
    int flag = 0;
    int diff = 0;
  for(int i=0;i<10;i++){
      
      if (flag==1) break;
      
    while(!st->goalTest()){//add time termination condition
      //$$ to check how proceeding
        endt = clock();
        if ((((endt - startt)/1000)/1000) >= ((tim * 60) -1)){
        
            flag=1;
            break;
        } 
      st->padDash(st->x);
      st->bestChild();
      st->update(); //index++incorportd in this
    //  cout<<"NEXT INDEX"<<endl;
      // st->index++;
    }
  }

  //cout<<"Over";
}


// void localSearch(State *st){
//   State *n1 = new
// }


int main(int argc, char *argv[]){
  clock_t startt,endt;
  startt=clock();
  //Argument check
  if(argc<3){//##Make .sh files
    cout<<"Argumnents Missing"<<endl;
    return 0;
  }
  /*---------Vars--------*///##Make Gobal if needed
  string ifilename = argv[1];
  string ofilename = argv[2];
  /*----------Input---------*/
  ifstream fin;
  fin.open(ifilename);
  if(!fin.is_open()){
    cout<<"Unable to open input file"<<ifilename<<endl;
    return 0;
  }
  fin>>tim;  //Reading Time
  //Reading Vocabulary size and char array
  fin>>vsize;
  for(int i=0;i<vsize;i++){
      fin>>v[i];
      if (fin.peek() == ','||fin.peek() == ' ')
           fin.ignore();
  }
  //Making map of vocab and index
  makeMap();
  //Reading number of string,and strings
  State* st = new State();
  fin>>st->k;
  st->x.resize(st->k);
  for(int i=0;i<st->k;i++)
    fin>>st->x[i];
  //st->setMaxLength(); //for goal test

  fin>>cc;  //Reading Conversion Cost
  // Reading Matching Cost
  for(int i=0;i<=vsize;i++)
    for(int j=0;j<=vsize;j++)
      fin>>mc[i][j];

  fin.close();
  /*----------------Input END------------------*/
  //Debug
  //cout<<tim;
  /*----------------Main Code------------------*/
  try{
    AStar1(st,startt,tim);
    //localSearch();
  }
  catch(const char* msg){
    cout<<msg<<endl;
  }
  //
  //
  //

  /*----------------Main END-------------------*/
  // //Debug
  //   cout<<time<<endl;
  //   cout<<vsize<<endl;
  //   cout<<v[0]<<" "<<v[1]<<endl;
  //   cout<<cc<<"sdf"<<k;
  //$$
  // for(int i=0;i<st->k;i++){
  //   for(int j=0;j<st->x[i].length();j++)
  //     cout<<st->x[i][j]<<" ";//##Replace with modified strings of saem lengths
  //   cout<<endl;
  // }
  //   cout<<"Matching Cost: "<<st->getMC()<<endl<<"Conversion Cost: "<<st->getCC()<<endl<<"Total cost: "<<st->getCC()+st->getMC()<<endl;

  /*-----------------Output--------------------*/
  ofstream fout;
  fout.open(ofilename);
  if(!fout.is_open()){
    cout<<"Unable to open output file"<<ofilename<<endl;
    return 0;
  }
  for(int i=0;i<st->k;i++)
    fout<<st->x[i]<<endl;//##Replace with modified strings of saem lengths
  fout.close();
  /*---------------Output END------------------*/
  endt = clock();
   //cout<<"Time Lapsed: "<<(endt-startt)/1000<<"ms"<<endl;

}
