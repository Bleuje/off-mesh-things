/*****
Etienne JACOB, 26/07/2016
*****
Off file rescaling (polygons can only be triangles here),
used to have similar scales in TOSCA shapes compared to FAUST shapes
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

bool transformationIsChosen = false;
float A,B;

void load(const string& name,const int& id,float& minSoFar,float& maxSoFar){
    string filepath = path + name + to_string(id) + ".off";
    ifstream in (filepath);

    string s;
    in>>s;

    in>>n>>p>>m;
    for(int i=0;i<n;i++){
        for(int j=0;j<3;j++){
            in>>vert[i][j];
            minSoFar=min(minSoFar,vert[i][j]);
            maxSoFar=max(maxSoFar,vert[i][j]);
        }
    }
    for(int i=0;i<p;i++){
        int waste;in>>waste;
        for(int j=0;j<3;j++){
            in>>tri[i][j];
        }
    }
}

void write(const string& name,const int& id,float& minSoFar,float& maxSoFar){
    string filepath = path + name + "Scaled" + to_string(id) + ".off";
    ofstream out (filepath);

    out<<"OFF\n";

    out<<n<<" "<<p<<" "<<m<<"\n";
    for(int i=0;i<n;i++){
        for(int j=0;j<3;j++){
            float f = vert[i][j];
            if(!transformationIsChosen){
                A = (maxTarget-minTarget)/(maxSoFar-minSoFar);
                B = minTarget - minSoFar*A;
            }
            out<<A*f+B<<" ";
        }
        out<<"\n";
    }
    for(int i=0;i<p;i++){
        out<<3;
        for(int j=0;j<3;j++){
            out<<" ";
            out<<tri[i][j];
        }
        out<<"\n";
    }
}

void convert(const string& name,const int& idMin,const int& idMax){
    for(int i=idMin;i<idMax;i++){
        cout<<"Treating "<<name<<" "<<i<<" ...";
        float minSoFar=1e6,maxSoFar=-1e6;
        load(name,i,minSoFar,maxSoFar);
        write(name,i,minSoFar,maxSoFar);
        cout<<"done\n";
    }
}

int main(){
    convert("cat",0,11);

    return 0;
}
