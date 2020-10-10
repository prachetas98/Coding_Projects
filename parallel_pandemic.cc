//
// Pandemic.cc
//
// Author: Prachetas Deshpande
// Purpose: Compares two DNA sequences using a dynamic programming
// algorithm.Finds the longest identical subsequence between two
// sequences. This is done in parallel to make it more efficient
// Note: This program uses both openmp and threads and c++11 compiler

#include <iostream>
#include <omp.h>
#include <vector>
#include <set>
#include <fstream>
#include <future>
#include <thread>
#include <sstream>
#include <cassert>
#include <algorithm>

using namespace std;
vector<vector<int> > LSQ;
vector<vector<pair<int,int> > > from;     // making the vectors global
vector<vector<future<bool> > > ready;     // future vector to get the values
vector<vector<promise<bool> > > ready_p;  // promise to use for the future variables

void parallel_DNA(string DNA1, string DNA2, int k, int n);

// function to read the string
string read_string(istream &in) {
  string temp;
  string line;
  while (!in.eof()) {
    getline(in,line);
    if (!in.fail()) {
      istringstream in1(line);
      int l;
      in1 >> l;
      //cout << l << endl;
      while (!in1.eof()) {
	string t;
	in1 >> t;
	if (!in1.fail()) {
	  temp+=t;
	}
      }
    }
  }
  return temp;
}

// Compute the longest identical subsequence between DNA1 and DNA2

string LS(string &DNA1, string& DNA2) {

  cout << "DNA1 Length = " << DNA1.length() << endl;
  cout << "DNA2 Length = " << DNA2.length() << endl;

  LSQ.resize(DNA1.length()+1);
  from.resize(DNA1.length()+1);
  #pragma omp parallel for                // openmp to parallelize the loop
  for (int i=0;i<DNA1.length()+1;i++) {
    LSQ[i].resize(DNA2.length()+1,0);
    from[i].resize(DNA2.length()+1);
  }
  #pragma omp parallel for                // openmp to parallize this loop
  for (int i=0;i<DNA2.length()+1;i++) {
    LSQ[0][i] = 0;
    from[0][i] = make_pair(-1,-1);
  }
  #pragma omp parallel for                // openmp to parallize the loop
  for (int i=1;i<DNA1.length()+1;i++) {
    LSQ[i][0] = 0;
    from[i][0] = make_pair(-1,-1);
  }

  thread t1(parallel_DNA,DNA1,DNA2,0,4);    // threads to divide the work
  thread t2(parallel_DNA,DNA1,DNA2,1,4);
  thread t3(parallel_DNA,DNA1,DNA2,2,4);
  thread t4(parallel_DNA,DNA1,DNA2,3,4);

  t1.join();
  t2.join();
  t3.join();
  t4.join();


  cout << "LSQ length = " << LSQ[DNA1.length()][DNA2.length()] << endl;

  string return_it;
  // Construct the LIS.
  int l1 = DNA1.length();
  int l2 = DNA2.length();
  while ((l1!=0) && (l2!=0)) {
    pair<int,int> t;
    t=from[l1][l2];
    if ((t.first == l1-1)&&(t.second ==l2-1)) {
      assert(DNA1[l1-1]==DNA2[l2-1]);
      return_it.insert(0,1,DNA1[l1-1]);
    }
    l1=t.first;
    l2=t.second;
  }

  assert(return_it.length()==LSQ[DNA1.length()][DNA2.length()]);

  return return_it;
}

// parallel function to optimize the for loop and if-else statements
void parallel_DNA(string DNA1, string DNA2, int k, int n){
  int len = DNA2.length();
  int start = max(1,((k*len)/n));
  int end = min( ((k+1)*(DNA2.length()+1))/n, (DNA2.length()+1));
  for (int i=1;i<DNA1.length()+1;i++) {
    if(k!=0){
      bool go = ready[k-1][i].get();
    }
  for (int j=start;j<end;j++) {
    if (DNA1[i-1]==DNA2[j-1]) {
      if (LSQ[i-1][j-1]+1 > max(LSQ[i-1][j],LSQ[i][j-1])) {
        LSQ[i][j] = LSQ[i-1][j-1]+1;
        from[i][j] = make_pair(i-1,j-1);
      }
      else {
        if (LSQ[i-1][j] > LSQ[i][j-1]) {
          LSQ[i][j] = LSQ[i-1][j];
          from[i][j] = make_pair(i-1,j);
        }
        else {
          LSQ[i][j] = LSQ[i][j-1];
          from[i][j] = make_pair(i,j-1);
        }
      }
    }
    else {
      if (LSQ[i-1][j] > LSQ[i][j-1]) {
        LSQ[i][j]= LSQ[i-1][j];
        from[i][j] = make_pair(i-1,j);
      }
      else{
        LSQ[i][j]= LSQ[i][j-1];
        from[i][j] = make_pair(i,j-1);
      }
    }
  }
  if (k<n-1) {
      ready_p[k][i].set_value(true); // Get the next one moving
    }
}
}


int main(int argc,char *argv[]) {
  assert (argc==3); // Fail if this isn't true.

  ifstream in1;
  in1.open(argv[1]);
  if (in1.fail()) {
    cout << "Couldn't open " << argv[1] << endl;
    exit(-1);
  }
  ifstream in2;
  in2.open(argv[2]);
  if (in2.fail()) {
    cout << "Couldn't open " << argv[2] << endl;
    exit(-1);
  }

  string DNA1;
  DNA1 = read_string(in1);
  string DNA2;
  DNA2 = read_string(in2);

  ready.resize(3);
  ready_p.resize(3);
  for (int i=0;i<3;i++) {
    ready[i].resize(DNA1.length()+1);
    ready_p[i].resize(DNA1.length()+1);
    for (int j=0;j<DNA1.length()+1;j++) {
      ready[i][j] = ready_p[i][j].get_future();
    }
  }

  string LS1;
  LS1 = LS(DNA1,DNA2);

  cout << LS1 << endl;
  cout << "Similarity score 1 vs 2=" << LS1.length()/(DNA1.length()*1.0) << endl;
  cout << "Similarity score 2 vs 1=" << LS1.length()/(DNA2.length()*1.0) << endl;

}
