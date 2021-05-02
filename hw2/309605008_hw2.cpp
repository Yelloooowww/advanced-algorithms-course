#include <iostream>
#include <string>
#include <sstream>
#include <vector>
// #include <stdlib.h>
#include <list>
#include <cstdlib>
#include <fstream>
using namespace std;

float MYabs(float a){
  if(a>=0) return a;
  else return -a;
}

class node;
class RBtree;
class ID_Value{
private:
  float value;
  int ID;
  friend class node;
  friend class RBtree;
public:
  ID_Value(){}
  ID_Value(int b,float a):value(a),ID(b){}

  float GetValue() const{return value;};
  int GetID() const{return ID;};

  friend bool operator< (const ID_Value& foo,const ID_Value& bar){
    if (foo.GetValue() != bar.GetValue() ){
      return (foo.GetValue() < bar.GetValue() );
    }else{
      return (foo.GetID() > bar.GetID() );
    }
  }

  friend bool operator> (const ID_Value& foo,const ID_Value& bar){
    if (foo.GetValue() != bar.GetValue() ){
      return (foo.GetValue() > bar.GetValue() );
    }else{
      return (foo.GetID() < bar.GetID() );
    }
  }

  friend bool operator!= (const ID_Value& foo,const ID_Value& bar){
    if (foo.GetValue() != bar.GetValue() ){
      return 1;
    }else{
      return ( foo.GetID() != bar.GetID() );
    }
  }

  friend bool operator== (const ID_Value& foo,const ID_Value& bar){
    if (foo.GetValue() == bar.GetValue() ){
      if (foo.GetID() == bar.GetID()){
        return 1;
      }else{
        return 0;
      }
    }
    return 0;
  }

  friend ostream& operator<< (ostream& out, const ID_Value& foo){
    if ((foo.GetID()== -1) & (foo.GetValue() == -1)){
      return out<<"";
    }
    return out<<foo.GetID()<<" "<<foo.GetValue();
  }
};

class node
{
public:
  ID_Value key;
  node *parent;
  char color;
  node *left;
  node *right;
  friend class RBtree; //使類別A可以存取類別B放在private區域的類別成員。
  ID_Value GetKey() const{return key;};
};

class RBtree
{
public :
  node *root;
  node *q;
  vector<ID_Value> sort_list;
  bool recursive_break_flag;
  int extend_lenght;
  ostringstream output_buffer;
  RBtree(){
    q=NULL;
    root=NULL;
    output_buffer.str("");
  }
  void insert(ID_Value z);
  void insertfix(node *);
  void leftrotate(node *);
  void rightrotate(node *);
  void del(ID_Value x);
  node* successor(node *);
  void delfix(node *);
  void disp();
  void display( node *);
  void search(ID_Value x);

  void Inorder_r(node *current, int rank);
  void Inorder_v(node *current, float value);
  void Inorder_K(node *current, float value, int num);
  void sort_r(int rank);
  void sort_v(float value);
  void sort_K(float value, int num);

  void r(int rank);
  void R(int rank);
  void v(float value);
  void V(float value);
  void K(float value, int num);
};

void RBtree::r(int rank){
  this->sort_r(rank);
  vector<ID_Value> ans_list;
  ID_Value tmpppp = this->sort_list[ this->sort_list.size() -1 ];
  ans_list.push_back(tmpppp);
  while (tmpppp.GetValue()==sort_list[ this->sort_list.size() -2 ].GetValue()){
    this->sort_list.pop_back();
    tmpppp = this->sort_list[ this->sort_list.size() -1 ];
    ans_list.push_back(tmpppp);
  }
  output_buffer<<"r "<<ans_list[ans_list.size()-1]<<endl;
}

void RBtree::R(int rank){
  this->sort_r(rank);
  vector<ID_Value> ans_list;
  ID_Value tmpppp = this->sort_list[ this->sort_list.size() -1 ];
  ans_list.push_back(tmpppp);
  while (tmpppp.GetValue()==sort_list[ this->sort_list.size() -2 ].GetValue()){
    this->sort_list.pop_back();
    tmpppp = this->sort_list[ this->sort_list.size() -1 ];
    ans_list.push_back(tmpppp);
  }
  output_buffer<<"R "<<ans_list[0]<<endl;
}
void RBtree::v(float value){
  this->sort_v(value);
  output_buffer<<"v "<<this->sort_list.size()<<endl;
}
void RBtree::V(float value){
  this->sort_v(value);
  int last = this->sort_list.size();
  int first = last;
  while (this->sort_list[first-1].GetValue() == value) {
    first --;
  }
  first ++;
  output_buffer<<"V "<<first<<" "<<last<<endl;
}
void RBtree::K(float value, int num){
  this->sort_K(value,num);
  int ans_list[num];
  int count = 0;
  int left = this->sort_list.size()-1-num;
  int right = this->sort_list.size()-num;

  while(count<num){
    float tmp1 = this->sort_list[left].GetValue();
    float tmp2 = this->sort_list[right].GetValue();
    if(MYabs(tmp1-value) < MYabs(tmp2-value)){
      while(this->sort_list[left].GetValue() == tmp1) left --;
      ans_list[count] = this->sort_list[left+1].GetID();
    }else{
      ans_list[count] = this->sort_list[right].GetID();
      right ++;
    }
    count ++;
  }
  output_buffer<<"K ";
  for(int i=0;i<count;i++) output_buffer<<ans_list[i]<<" ";
}
void RBtree::sort_v(float value){
  (this->sort_list).clear();
  this -> recursive_break_flag = 0;
  this -> Inorder_v(this->root, value);
}
void RBtree::Inorder_v(node *current, float value){
  if(this -> recursive_break_flag) return;
  if (current) {// if current != NULL
    Inorder_v(current->right, value);


    if (current->key.GetValue()>=value){
      this -> sort_list.push_back(current->key);
    }else{
      this -> recursive_break_flag = 1;
    }

    Inorder_v(current->left, value);
  }
}
void RBtree::sort_K(float value, int num){
  (this->sort_list).clear();
  this -> recursive_break_flag = 0;
  this -> extend_lenght = 0;
  this -> Inorder_K(this->root, value, num);
}
void RBtree::Inorder_K(node *current, float value, int num){
  if(this -> recursive_break_flag) return;
  if (current) {// if current != NULL
    Inorder_K(current->right, value, num);


    if (current->key.GetValue()>=value){
      this -> sort_list.push_back(current->key);
    }else{
      this -> sort_list.push_back(current->key);
      this -> extend_lenght ++;
      if(this -> extend_lenght > num) this -> recursive_break_flag = 1;
    }

    Inorder_K(current->left, value, num);
  }
}
void RBtree::sort_r(int rank){
  (this->sort_list).clear();
  this -> recursive_break_flag = 0;
  this -> Inorder_r(this->root, rank);
}
void RBtree::Inorder_r(node *current, int rank){
  if(this -> recursive_break_flag) return;
  if (current) {// if current != NULL
    Inorder_r(current->right, rank);

    if ((this->sort_list).size()< rank ) {
      this -> sort_list.push_back(current->key);
    }else{
      if( current->key.GetValue() == this->sort_list[this->sort_list.size()-1].GetValue() ){
        this -> sort_list.push_back(current->key);
      }else this -> recursive_break_flag = 1;

    }

    Inorder_r(current->left, rank);
  }
}
void RBtree::insert(ID_Value z){
  int i=0;
  node *p,*q;
  node *t = new node;
  t->key=z;
  t->left=NULL;
  t->right=NULL;
  t->color='r';
  p=root;
  q=NULL;
  if(root==NULL){
    root=t;
    t->parent=NULL;
  }
  else{
    while(p!=NULL){
      q=p;
      if(p->key<t->key) p=p->right;
      else p=p->left;
    }
    t->parent=q;
    if(q->key<t->key) q->right=t;
    else q->left=t;
  }
  // insertfix(t);
}
void RBtree::insertfix(node *t){
  node *u;
  if(root==t){
    t->color='b';
    return;
  }
  while(t->parent!=NULL&&t->parent->color=='r'){
    node *g=t->parent->parent;
    if(g->left==t->parent){
      if(g->right!=NULL){
        u=g->right;
        if(u->color=='r'){
          t->parent->color='b';
          u->color='b';
          g->color='r';
          t=g;
        }
      }
      else{
        if(t->parent->right==t){
          t=t->parent;
          leftrotate(t);
        }
        t->parent->color='b';
        g->color='r';
        rightrotate(g);
      }
    }
    else{
      if(g->left!=NULL){
        u=g->left;
        if(u->color=='r'){
            t->parent->color='b';
            u->color='b';
            g->color='r';
            t=g;
        }
      }
      else{
        if(t->parent->left==t){
          t=t->parent;
          rightrotate(t);
        }
        t->parent->color='b';
        g->color='r';
        leftrotate(g);
      }
    }
    root->color='b';
  }
}

void RBtree::del(ID_Value x)
{
  if(root==NULL) return ;

  node *p;
  p=root;
  node *y=NULL;
  node *q=NULL;
  int found=0;
  while(p!=NULL&&found==0){
    if(p->key == x) found=1;
    if(found==0){
      if(p->key<x) p=p->right;
      else p=p->left;
    }
  }
  if(found==0) return ;
  else{
    if(p->left==NULL||p->right==NULL) y=p;
    else y=successor(p);
    if(y->left!=NULL) q=y->left;
    else{
      if(y->right!=NULL) q=y->right;
      else q=NULL;
    }
    if(q!=NULL) q->parent=y->parent;
    if(y->parent==NULL) root=q;
    else{
      if(y==y->parent->left) y->parent->left=q;
      else y->parent->right=q;
    }
    if(y!=p){
      p->color=y->color;
      p->key=y->key;
    }
    // if(y->color=='b') delfix(q);
  }
}

void RBtree::delfix(node *p){
  node *s;
  while(p!=root&&p->color=='b'){
    if(p->parent->left==p){
      s=p->parent->right;
      if(s->color=='r'){
        s->color='b';
        p->parent->color='r';
        leftrotate(p->parent);
        s=p->parent->right;
      }
      if(s->right->color=='b'&&s->left->color=='b'){
        s->color='r';
        p=p->parent;
      }
      else{
        if(s->right->color=='b'){
          s->left->color=='b';
          s->color='r';
          rightrotate(s);
          s=p->parent->right;
        }
        s->color=p->parent->color;
        p->parent->color='b';
        s->right->color='b';
        leftrotate(p->parent);
        p=root;
      }
    }
    else{
      s=p->parent->left;
      if(s->color=='r'){
        s->color='b';
        p->parent->color='r';
        rightrotate(p->parent);
        s=p->parent->left;
      }
      if(s->left->color=='b'&&s->right->color=='b'){
        s->color='r';
        p=p->parent;
      }
      else{
        if(s->left->color=='b'){
          s->right->color='b';
          s->color='r';
          leftrotate(s);
          s=p->parent->left;
        }
        s->color=p->parent->color;
        p->parent->color='b';
        s->left->color='b';
        rightrotate(p->parent);
        p=root;
      }
    }
  p->color='b';
  root->color='b';
  }
}


void RBtree::leftrotate(node *p){
  if(p->right==NULL) return ;
  else{
    node *y=p->right;
    if(y->left!=NULL){
      p->right=y->left;
      y->left->parent=p;
    }
    else p->right=NULL;
    if(p->parent!=NULL) y->parent=p->parent;
    if(p->parent==NULL) root=y;
    else{
    if(p==p->parent->left) p->parent->left=y;
    else p->parent->right=y;
    }
    y->left=p;
    p->parent=y;
  }
}
void RBtree::rightrotate(node *p){
  if(p->left==NULL) return ;
  else{
    node *y=p->left;
    if(y->right!=NULL){
      p->left=y->right;
      y->right->parent=p;
    }
    else p->left=NULL;
    if(p->parent!=NULL) y->parent=p->parent;
    if(p->parent==NULL) root=y;
    else{
      if(p==p->parent->left) p->parent->left=y;
      else p->parent->right=y;
    }
    y->right=p;
    p->parent=y;
  }
}

node* RBtree::successor(node *p){
  node *y=NULL;
  if(p->left!=NULL){
     y=p->left;
     while(y->right!=NULL) y=y->right;
  }
  else{
    y=p->right;
    while(y->left!=NULL) y=y->left;
  }
  return y;
}

void RBtree::search(ID_Value x){
  if(root==NULL)return  ;

  node *p=root;
  int found=0;
  while(p!=NULL&& found==0){
    if(p->key==x) found=1;
    if(found==0){
    if(p->key<x) p=p->right;
    else p=p->left;
    }
  }
}

int main(int argc, char *argv[]){
  RBtree obj;
  string input_file = argv[1];
  string output_file = argv[2];
  ifstream infile(input_file.c_str());
  ofstream outfile(output_file.c_str());
  while(!infile.eof()){
    string action;
    infile >> action;
    if(action=="I"){
      int id;
      float value;
      infile >> id >> value;
      obj.insert(ID_Value(id, value));
    }
    if(action=="D"){
      int id;
      float value;
      infile >> id >> value;
      obj.del(ID_Value(id, value));
    }
    if(action=="r"){
      int rank;
      infile >> rank ;
      obj.r(rank);
    }
    if(action=="R"){
      int rank;
      infile >> rank ;
      obj.R(rank);
    }
    if(action=="v"){
      float value;
      infile >> value ;
      obj.v(value);
    }
    if(action=="V"){
      float value;
      infile >> value ;
      obj.V(value);
    }
    if(action=="K"){
      float value;
      int num;
      infile >> value >> num;
      obj.K(value, num);
    }
    outfile << obj.output_buffer.str();
    // cout<< obj.output_buffer.str();
    obj.output_buffer.str("");
  }

  outfile.close();
  return 0;
}
