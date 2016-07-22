/************
Etienne JACOB 22/07/2016
---
Convert a list of triangles and a list of vertices (.tri and .vert files) to OFF file format.
It should work on TOSCA datasets given there : http://tosca.cs.technion.ac.il/book/resources_data.html
---
this code uses C++11 features
*************/

#include <bits/stdc++.h>
using namespace std;

///maximum number of vertices or faces (I was too lazy to use dynamic array size)
const int MAX_TAB = 20000;
int tabTri[MAX_TAB][4];
float tabVert[MAX_TAB][3];
map<pair<int,int>,int > edges;
int nbTri,nbVert,nbEdges;

///PATH where the files will be loaded and created
string path = "C:/Users/New user/Downloads/nonrigid3d/";

string name = "david";
///Here david0.tri, david0.vert, david1.tri, david1.vert, ... will be loaded.

int readTri(string filename){
    filename = path + filename;
    ifstream in (filename.c_str());
    int k = 0;
    int cur;
    while(in>>cur){
        tabTri[k/3][1 + (k%3)]=cur;
        k++;
    }
    int nbTri = k/3;
    for(int i=0;i<nbTri;i++){
        tabTri[i][0]=3;
    }
    return nbTri;
}

int readVert(string filename){
    filename = path + filename;
    ifstream in (filename.c_str());
    int k = 0;
    float cur;
    while(in>>cur){
        tabVert[k/3][k%3]=cur;
        k++;
    }
    int nbVert = k/3;
    return nbVert;
}

int countEdges(){
    for(int i=0;i<nbTri;i++){
        for(int j=0;j<3;j++){
            int i1 = tabTri[i][1 + j%3];
            int i2 = tabTri[i][1 + (j+1)%3];
            edges[make_pair(min(i1,i2),max(i1,i2))]++;
        }
    }
    return edges.size();
}

void write(string filename){
    filename = path + filename;
    ofstream out (filename);
    out<<"OFF\n";
    out<<nbVert<<" "<<nbTri<<" "<<nbEdges<<"\n";

    for(int i=0;i<nbVert;i++){
        out<<tabVert[i][0]<<" ";
        out<<tabVert[i][1]<<" ";
        out<<tabVert[i][2]<<"\n";
    }

    for(int i=0;i<nbTri;i++){
        out<<tabTri[i][0]<<" ";
        out<<tabTri[i][1]-1<<" ";
        out<<tabTri[i][2]-1<<" ";
        out<<tabTri[i][3]-1<<"\n";
    }
}

void convertOne(string name,int num){
    cout<<"Converting "<<num<<"...";
    string filenameTri = name+to_string(num)+".tri";
    string filenameVert = name+to_string(num)+".vert";

    nbVert = readVert(filenameVert);
    nbTri = readTri(filenameTri);
    nbEdges = countEdges();

    string filenameOff = name+to_string(num)+".off";
    write(filenameOff);
    cout <<"done\n";
}

void convert(string name,int numMax){
    for(int num=0;num<numMax;num++){
        convertOne(name,num);
    }
}

int main(){
    /// convert mesh david4
    convertOne(name,4);

    /// convert the "david" meshes from 0 to 14
    convert(name,15);

    return 0;
}
