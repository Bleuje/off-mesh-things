/*****
Etienne JACOB, 26/07/2016
*****
Off file shuffling (polygons can only be triangles here),
---
C++11 features are used
*****/
#include <bits/stdc++.h>
using namespace std;

const float minTarget = -0.75;
const float maxTarget = 0.75;

const string path = "C:/Users/New user/Downloads/fmaps_siggraph2012_code/shapes/tosca-blended/";

const int MAX_SIZE = 1e6;
float vert[MAX_SIZE][3];
int tri[MAX_SIZE][3];
int n,p,m;

vector<int> randomPermutation(const int& sz){
    vector<int> res(sz);
    for(int i=0;i<sz;i++){
        res[i]=i;
    }
    for(int i=sz;i>=1;i--){
        int k = rand()%i;
        int temp = res[k];
        res[k] = res[i-1];
        res[i-1] = temp;
    }
    return res;
}

vector<int> inversePermutation(const vector<int>& v){
    vector<int> res(int(v.size()));
    for(int i=0;i<n;i++){
        res[v[i]] = i;
    }
    return res;
}

void load(const string& name,const int& id){
    string filepath = path + name + to_string(id) + ".off";
    ifstream in (filepath);

    string s;
    in>>s;

    in>>n>>p>>m;
    for(int i=0;i<n;i++){
        for(int j=0;j<3;j++){
            in>>vert[i][j];
        }
    }
    for(int i=0;i<p;i++){
        int waste;in>>waste;
        for(int j=0;j<3;j++){
            in>>tri[i][j];
        }
    }
    cout<<"loading done...";
}

void write(const string& name,const int& id){
    string filepath = path + name + "Shuffled" + to_string(id) + ".off";
    ofstream out (filepath);

    vector<int> permut = randomPermutation(n);
    vector<int> inv = inversePermutation(permut);

    out<<"OFF\n";

    out<<n<<" "<<p<<" "<<m<<"\n";
    for(int i=0;i<n;i++){
        for(int j=0;j<3;j++){
            float f = vert[inv[i]][j];
            out<<f<<" ";
        }
        out<<"\n";
    }
    for(int i=0;i<p;i++){
        out<<3;
        for(int j=0;j<3;j++){
            out<<" ";
            out<<permut[tri[i][j]];
        }
        out<<"\n";
    }
}

void convert(const string& name,const int& idMin,const int& idMax){
    for(int i=idMin;i<idMax;i++){
        cout<<"Treating "<<name<<" "<<i<<" ...";
        load(name,i);
        write(name,i);
        cout<<"done\n";
    }
}

int main(){
    convert("catScaled",0,11);

    return 0;
}
