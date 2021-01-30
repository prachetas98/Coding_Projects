// ConsoleApplication3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// function to remove three consecutive 
// duplicates 

void minimumBribes(vector<int> q) {
    int swap = 0;
    int bribes;
    int pos = 0;
    for (int i = q.size() - 1; i >= 0; i--) {
        int j = 0;
        bribes = q[pos] - (pos + 1);
        if (bribes > 2) {
            cout << "Too chaotic" << endl;
            return;
        }
        if (q[i] - 2 > 0) {
            j = q[i] - 2;
        }

        while (j <= i) {
            if (q[j] > q[i]) {
                swap++;
            }
            j++;
        }
        pos++;
    }
    cout << swap << endl;

}

long largestRectangle(vector<int> h) {
    vector <long> area;
    long great = 0;
    int num_build = 1;
    //int sz = h.size();
    for (int i = 0; i < h.size(); i++) {
        if (i == 0) {
            for (int j = 1; j < h.size(); j++) {

                if (h[0] <= h[j]) {
                    num_build++;
                }
                else {
                    //num_build++;
                    break;
                }
            }
            //cout << "num build is " << num_build << endl; 
            area.push_back(num_build * h[i]);
        }
        else if (i == h.size() - 1) {
            for (int j = h.size() - 2; j >= 0; j--) {
                //cout << h[j] << endl;
                if (h[i] <= h[j]) {
                    num_build++;
                }
                else {
                    break;
                }
            }
            area.push_back(num_build * h[i]);
        }
        else {
            //cout << "num build was " << num_build << endl;
            for (int j = i; j < h.size(); j++) {
                //cout << h[j] << endl;
                if (h[i] <= h[j]) {
                    num_build++;
                }
                else {
                    break;
                }
            }
            cout << "else num_build is" << num_build << endl;
            for (int y = i - 1; y >= 0; y--) {
                if (h[i] <= h[y]) {
                    num_build++;
                }
                else {
                    break;
                }
            }
            //cout << "num build is " << num_build << endl;
            //num_build++;
            area.push_back((num_build - 1) * h[i]);
        }
        num_build = 1;
    }

    for (int g = 0; g < area.size(); g++) {
        cout << area[g] << " ";
    }

    return *max_element(area.begin(), area.end());
}

int commonChild(string s1, string s2) {
    static int T[5001][5001];
    int m = s1.size();
    int n = s2.size();

    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                T[i][j] = 0;
            }
            else if (s1[i - 1] == s2[j - 1]) {
                T[i][j] = 1 + T[i - 1][j - 1];
            }
            else {
                T[i][j] = max(T[i][j - 1], T[i - 1][j]);
            }
        }
    }
    return T[m][n];
}

void remove3ConsecutiveDuplicates(string str, int k)
{
    int l = 1;
    vector<char> v;
    for (int i = 0; i < str.size(); ++i) {
        v.push_back(str[i]);

        if (v.size() >= k) {
            int sz = v.size();

            // removing three consecutive duplicates 
            for (int r = 1; r < k; r++) {
                if (v[sz - 1] == v[sz - 1 - r]) {
                    l++;
                }
                else
                    break;
            }
            if (l == k) {
                v.resize(sz - k);
            }
            // Removing three characters from the string 
        }
        l = 1;
    }

    // printing the string final string 
    for (int i = 0; i < v.size(); ++i)
        cout << v[i];
}

vector<int> rotateLeft(int d, vector<int> arr) {
    int diff = 0;
    int y = 0;
    //int y=arr[arr.size()-1];
    for (int i = 0; i < d; i++) {
        y = arr[0];
        arr.erase(arr.begin());
        arr.push_back(y);
    }
    return arr;
}

int main()
{
    string str = "kklbbjjaalg";
    int k = 2;
    remove3ConsecutiveDuplicates(str, k);
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
