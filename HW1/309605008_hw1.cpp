#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
using namespace std;


void find_max(int arr[], int low, int high, int &max ,int &max_index){
  if(low == high){
    if(arr[low] > max){
      max = arr[low];
      max_index = low;
    }
    return;
  }
  if(high - low == 1){
    if(arr[low] > max){
      max = arr[low];
      max_index = low;
    }
    if(arr[high] > max){
      max = arr[high];
      max_index = high;
    }
    return;
  }
  int mid = (low + high) / 2;
  find_max(arr, low, mid, max, max_index);
  find_max(arr, mid+1, high, max, max_index);
}

const pair<int,int> Max = make_pair(2147483647,0);
void Merge(vector<pair<int,int> > &Array, int front, int mid, int end){
    vector< pair<int,int> > LeftSub(Array.begin()+front, Array.begin() + mid + 1);
    vector< pair<int,int> > RightSub(Array.begin()+mid+1, Array.begin() + end + 1);
    LeftSub.insert(LeftSub.end(), Max);
    RightSub.insert(RightSub.end(), Max);
    int idxLeft = 0;
    int idxRight = 0;
    for (int i = front; i <= end; i++) {
        if (LeftSub[idxLeft].first <= RightSub[idxRight].first ) {
            Array[i] = LeftSub[idxLeft];
            idxLeft++;
        }
        else{
            Array[i] = RightSub[idxRight];
            idxRight++;
        }
    }
}

void MergeSort(vector< pair<int,int> > &array, int front, int end){
    if (front < end) {
        int mid = (front + end) / 2;
        MergeSort(array, front, mid);
        MergeSort(array, mid + 1, end);
        Merge(array, front, mid, end);
    }
}

struct company{
  int id;
  int conduct_threshold;
  int grade_threshold;
  int salary;
};

class student{
public:
  int id;
  int conduct;
  int grade;
  vector<company> admission;
  int choose;

  void make_decision(){
    if(admission.size() == 0){
      // cout << "NO CHOOSE" << endl;
      choose = 0;
      return;
    }

    int max = -1;
    int max_index = -1;
    int N = admission.size();
    int salary_array[N];
    for(int i=0;i<N;i++) salary_array[i] = admission[i].salary;

    find_max(salary_array, 0, N-1, max, max_index);
    choose = admission[max_index].id;
    // cout << "choose=" << choose << " $$$=" << admission[max_index].salary << endl;
    return;
  }
};







int main(int argc, char *argv[]){
  string student_file = argv[1];
  string company_file = argv[2];
  string output_file = argv[3];

  // read company data
  vector<company> companys;
  ifstream infile0(company_file.c_str());
  while(!infile0.eof()){
    company tmp;
    infile0 >> tmp.id >> tmp.conduct_threshold >> tmp.grade_threshold >> tmp.salary;
    companys.push_back(tmp);
  }

  // read student data
  vector<student> students;
  ifstream infile1(student_file.c_str());
  while(!infile1.eof()){
    student tmp;
    infile1 >> tmp.id >> tmp.conduct >> tmp.grade;
    students.push_back(tmp);
  }


  for(int i=0;i<students.size();i++){
    for(int j=0;j<companys.size();j++){
      if(students[i].conduct>=companys[j].conduct_threshold && students[i].grade>=companys[j].grade_threshold){
        students[i].admission.push_back(companys[j]);
      }
    }
  }


  // for(int i=0;i<students.size();i++){
  //   cout << "student " << students[i].id << ": ";
  //   for(int j=0;j<students[i].admission.size();j++){
  //     cout << students[i].admission[j].id << ",";
  //   }
  //   if(students[i].admission.size()==0) cout << "No offer ";
  //   cout << endl;
  // }

  for(int i=0;i<students.size();i++){
    // cout << "student " << students[i].id << ": ";
    students[i].make_decision();
  }


  int N = students.size();
  pair<int,int>  student_array[N];
  for(int i=0;i<N;i++){
    student_array[i] = make_pair(students[i].id, students[i].choose);
  }

  vector< pair<int,int> > array(student_array, student_array+N);
  MergeSort(array, 0, N-1);

  ofstream outfile (output_file.c_str());
  for(int i=0;i<N;i++){
    // cout << array[i].first << ":" << array[i].second <<endl;
    outfile << array[i].first << ": " << array[i].second <<endl;
  }
  outfile.close();



  return 0;
}
