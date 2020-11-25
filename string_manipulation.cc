#include <iostream>
#include <string>
#include <vector>
using namespace std; 
  
// function to remove three consecutive 
// duplicates 
void remove3ConsecutiveDuplicates(string str, int k) 
{ 
    int l =1;
    vector<char> v; 
    for (int i = 0; i < str.size(); ++i) { 
        v.push_back(str[i]); 
  
        if (v.size() > k-1) { 
            int sz = v.size(); 
  
            // removing three consecutive duplicates 
            //if (v[sz - 1] == v[sz - 2] &&  
                //v[sz - 2] == v[sz - 3]) {
                for(int r=1;r<=k;r++){
                    //cout << "in the loop" << endl;
                    if(v[sz-1]==v[sz-1-r]){
                        l++;
                    }
                    else
                        break;
                }
                    if(l==k){
                    v.resize(sz - k);
                    }// Removing three characters 
                                 // from the string 
            //} 
        }
        //for (int u = 0; u < v.size(); ++u) 
            //cout << "This is the " << i << "th loop " << v[u] << endl; 
            //cout << "l is " << l << " and k is" << k << endl;
        l = 1;
    } 
  
    // printing the string final string 
    for (int i = 0; i < v.size(); ++i) 
        cout << v[i]; 
} 
  
// driver code 
int main() 
{ 
    string str = "abbcccb"; 
    int k =3;
    remove3ConsecutiveDuplicates(str,k); 
    return 0; 
} 
