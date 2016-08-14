/*****
Etienne JACOB, 13/08/2016
*****
Off files and correspondences given.
Trying to find out bad correspondences.
And trying to correct them.
---
C++11 features are used
*****/

#include <bits/stdc++.h>
using namespace std;

const string path = "C:/Users/Etienne/Desktop/before_mincut/meshes/";
const string path2 = "C:/Users/Etienne/Desktop/before_mincut/data/";

const int MAX_SIZE_V = 7000;
const int MAX_SIZE_T = 15000;
const int SAMPLE_SIZE = 100;
const int SAMPLE_SIZE_LARGE = 1000;
const float INFINITE = 1e9;
const float VOTE_LIM = 0.02;
const int BFS_NB = 300;

float vert[MAX_SIZE_V][3][2];
int triangles[MAX_SIZE_T][3][2];
int n[2],p[2],m[2];
vector<int> graphMesh[MAX_SIZE_V][2];
float geoDistances[MAX_SIZE_V][MAX_SIZE_V][2];

vector<int> samplingVec;
vector<int> samplingVecLarge;
map<int,int> indexS;
map<int,int> indexSL;
string header;
int sampleEval[SAMPLE_SIZE];
vector<bool> isGood;

float shortDist(const int& i1,const int& i2,const int& mesh){
    return sqrt((vert[i1][0][mesh]-vert[i2][0][mesh])*(vert[i1][0][mesh]-vert[i2][0][mesh]) + (vert[i1][1][mesh]-vert[i2][1][mesh])*(vert[i1][1][mesh]-vert[i2][1][mesh]) + (vert[i1][2][mesh]-vert[i2][2][mesh])*(vert[i1][2][mesh]-vert[i2][2][mesh]));
}

bool isNew(const int& v,const int& mesh, const int& val){
    for(int i=0;i<int(graphMesh[v][mesh].size());i++){
        if(graphMesh[v][mesh][i]==val){
            return false;
        }
    }
    return true;
}

void loadMesh(const string& name,const int& mesh){
    string filepath = path + name + ".off";
    ifstream in (filepath);

    in>>header;

    in>>n[mesh]>>p[mesh]>>m[mesh];
    for(int i=0;i<n[mesh];i++){
        for(int j=0;j<3;j++){
            in>>vert[i][j][mesh];
        }
    }
    for(int i=0;i<p[mesh];i++){
        int waste;in>>waste;
        int v1,v2,v3;in>>v1>>v2>>v3;
        if(isNew(v1,mesh,v2)) graphMesh[v1][mesh].push_back(v2);
        if(isNew(v2,mesh,v1)) graphMesh[v2][mesh].push_back(v1);
        if(isNew(v3,mesh,v2)) graphMesh[v3][mesh].push_back(v2);
        if(isNew(v2,mesh,v3)) graphMesh[v2][mesh].push_back(v3);
        if(isNew(v1,mesh,v3)) graphMesh[v1][mesh].push_back(v3);
        if(isNew(v3,mesh,v1)) graphMesh[v3][mesh].push_back(v1);
        triangles[i][0][mesh]=v1;
        triangles[i][1][mesh]=v2;
        triangles[i][2][mesh]=v3;
    }

    if(int(samplingVec.size())<=0){
        vector<int> listIndex;
        for(int i=0;i<n[0];i++){
            listIndex.push_back(i);
        }

        for(int i=n[0]-1;i>=0;i--){
            int k = rand()%(i+1);
            int aux = listIndex[i];
            listIndex[i]=listIndex[k];
            listIndex[k]=aux;
        }

        for(int i=0;i<min(int(listIndex.size()),SAMPLE_SIZE);i++){
            samplingVec.push_back(listIndex[i]);
            indexS[listIndex[i]]=i;
        }
        sort(samplingVec.begin(),samplingVec.end());
    }

    if(int(samplingVecLarge.size())<=0){
        vector<int> listIndex;
        for(int i=0;i<n[0];i++){
            listIndex.push_back(i);
        }

        for(int i=n[0]-1;i>=0;i--){
            int k = rand()%(i+1);
            int aux = listIndex[i];
            listIndex[i]=listIndex[k];
            listIndex[k]=aux;
        }

        for(int i=0;i<min(int(listIndex.size()),SAMPLE_SIZE_LARGE);i++){
            samplingVecLarge.push_back(listIndex[i]);
            indexSL[listIndex[i]]=i;
        }
        sort(samplingVecLarge.begin(),samplingVecLarge.end());
    }
}

vector<int> loadCorr(const string& name){
    string filepath = path2 + name + ".txt";
    ifstream in (filepath);

    int sz;
    in>>sz;

    vector<int> corr;

    for(int i=0;i<sz;i++){
        int j;in>>j;
        corr.push_back(j-1);
    }

    return corr;
}

void saveCorr(const string& name,const vector<int>& corr){
    string filepath = path2 + name + ".txt";
    ofstream out (filepath);

    int sz=corr.size();
    out<<sz<<"\n";

    for(int i=0;i<sz;i++){
        out<<corr[i]+1<<" ";
    }
}

set<int> bfs(const int& start,const int& nb,const int& mesh){
    set<int> res;
    vector<bool> visited(n[mesh],0);

    queue<int> frontier;
    frontier.push(start);
    visited[start]=true;

    while(!frontier.empty() && int(res.size())<nb){
        int next = frontier.front();
        for(int i = 0;i < int(graphMesh[next][mesh].size());i++){
            int k = graphMesh[next][mesh][i];
            if(!visited[k]){
                frontier.push(k);
                visited[k]=true;
            }
        }
        res.insert(next);
        frontier.pop();
    }
    return res;
}

long long evaluate(const int& p,const vector<int>& corr){
    long long res = 0;
    //float dThreshold = VOTE_LIM;
    for(int i=0;i<SAMPLE_SIZE;i++){
        int k = samplingVec[i];
        res+=100000*min((float)0.10,(float)abs(geoDistances[k][p][0] - geoDistances[corr[k]][corr[p]][1]));
    }
    long long res2 = 0;
    int cnt = 0;
    set<int> here = bfs(p,50,0);
    set<int>::iterator it;
    for(it=here.begin();it!=here.end();it++){
        int k = *it;
        res2 += 100000*min((float)0.02,(float)abs(geoDistances[k][p][0] - geoDistances[corr[k]][corr[p]][1]));
        cnt++;
    }
    return res/SAMPLE_SIZE + 0.1*(cnt>0?res2/cnt:0);
}

long long evaluate2(const int& p,const vector<int>& corr,const set<int>& here){
    long long res = 0;
    //float dThreshold = VOTE_LIM;
    for(int i=0;i<SAMPLE_SIZE;i++){
        int k = samplingVec[i];
        res+=100000*min((float)0.10,(float)abs(geoDistances[k][p][0] - geoDistances[corr[k]][corr[p]][1]));
    }
    long long res2 = 0;
    int cnt = 0;
    set<int>::iterator it;
    for(it=here.begin();it!=here.end();it++){
        int k = *it;
        float difference = abs(geoDistances[k][p][0] - geoDistances[corr[k]][corr[p]][1]);
        res2 -= 0.1*100000*(difference<0.01)*(1+5*isGood[k]);
        cnt++;
    }
    return res/SAMPLE_SIZE + 0.2*(cnt>0?res2/cnt:0);
}

long long evaluate2Large(const int& p,const vector<int>& corr,const set<int>& here){
    long long res = 0;
    //float dThreshold = VOTE_LIM;
    for(int i=0;i<SAMPLE_SIZE;i++){
        int k = samplingVecLarge[i];
        res+=100000*min((float)0.10,(float)abs(geoDistances[k][p][0] - geoDistances[corr[k]][corr[p]][1]));
    }
    long long res2 = 0;
    int cnt = 0;
    set<int>::iterator it;
    for(it=here.begin();it!=here.end();it++){
        int k = *it;
        float difference = abs(geoDistances[k][p][0] - geoDistances[corr[k]][corr[p]][1]);
        res2 -= 0.1*100000*(difference<0.01)*(1+5*isGood[k]);
        cnt++;
    }
    return res/SAMPLE_SIZE + 0.1*(cnt>0?res2/cnt:0);
}

int reconstructCorrectGeo(const string& name,const int& threshold,vector<int>& corr,const& opt){
    string filepath = path + name + "_cgeo.off";
    ofstream out (filepath);

    vector<bool> state(n[0],0);

    int cnt = 0;
    map<int,int> newindex;
    for(int i=0;i<n[0];i++){
        for(int l=0;l<SAMPLE_SIZE;l++){
            samplingVec[l]=rand()%n[0];
        }

        int before = evaluate(i,corr);
        if(before<threshold){
            state[i]=true;
            newindex[i]=cnt;
            cnt++;
        }
        else{
            int theMin = before;
            int jmin = corr[i];
            if(opt){
                for(int j=0;j<n[1];j++){
                    corr[i]=j;
                    int aux = evaluate(i,corr);
                    if(aux<theMin){
                        theMin = aux;
                        jmin=j;
                    }
                }
            }
            corr[i]=jmin;
            state[i]=(theMin<threshold);
            cnt+=state[i];
            if(i%25==0 && opt){cout << before - theMin <<" "<< before <<" -> "<< theMin <<"\n"<<flush;}
        }
    }
    //sort(pointsVec.begin(),pointsVec.end());

    out<<header<<"\n";

    int nn = cnt;
    int pp = 0;

    vector<bool> acceptedTriangle(p[0],0);
    for(int i=0;i<p[0];i++){
        if((state[triangles[i][0][0]])&&(state[triangles[i][1][0]])&&(state[triangles[i][2][0]])){
            acceptedTriangle[i]=true;
            pp++;
        }
    }

    out<<n[0]<<" "<<p[0]<<" "<<0<<"\n";
    for(int i=0;i<n[0];i++){
        if(true){
            out<<vert[i][0][0]<<" "<<vert[i][1][0]<<" "<<vert[i][2][0]<<"\n";
        }
    }
    for(int i=0;i<p[0];i++){
        if(acceptedTriangle[i]){
            out<<3<<" "<<triangles[i][0][0]<<" "<<triangles[i][1][0]<<" "<<triangles[i][2][0]<<" "<<"0.0"<<" "<<"0.0"<<" "<<"1.0"<<" "<<"1.0"<<"\n";
        }
        else{
            out<<3<<" "<<triangles[i][0][0]<<" "<<triangles[i][1][0]<<" "<<triangles[i][2][0]<<" "<<"1.0"<<" "<<"0.0"<<" "<<"0.0"<<" "<<"1.0"<<"\n";
        }
    }
    return nn;
}

void optimize(vector<int>& corr,const int& nSteps){
    for(int k=0;k<nSteps;k++){
        for(int l=0;l<SAMPLE_SIZE;l++){
            samplingVec[l]=rand()%n[0];
        }

        int i = rand()%(n[0]);
        int theMin = evaluate(i,corr);
        int jmin = corr[i];
        for(int j=0;j<n[1];j++){
            corr[i]=j;
            int aux = evaluate(i,corr);
            if(aux<theMin){
                theMin = aux;
                jmin=j;
            }
        }
        corr[i]=jmin;
    }
}

void optimize2(vector<int>& corr,const int& nSteps,const int& ts){
    for(int k=0;k<nSteps;k++){
        int i = rand()%(n[0]);

        if(evaluate(i,corr)>=ts){
            for(int l=0;l<SAMPLE_SIZE;l++){
                samplingVec[l]=rand()%n[0];
            }

            int theMin = evaluate(i,corr);
            int jmin = corr[i];
            for(int j=0;j<n[1];j++){
                corr[i]=j;
                int aux = evaluate(i,corr);
                if(aux<theMin){
                    theMin = aux;
                    jmin=j;
                }
            }
            corr[i]=jmin;
            isGood[i]=(theMin<ts);
        }
        else{
            isGood[i]=true;
        }
    }
}

void optimize3(vector<int>& corr,const int& nSteps){
    for(int k=0;k<nSteps;k++){
        int i = rand()%(n[0]);

        for(int l=0;l<SAMPLE_SIZE;l++){
            samplingVec[l]=rand()%n[0];
        }

        set<int> here = bfs(i,BFS_NB,0);
        int theMin = evaluate2(i,corr,here);
        int jmin = corr[i];
        for(int jj=0;jj<n[1]/50;jj++){
            int j = rand()%n[1];
            corr[i]=j;
            int aux = evaluate2(i,corr,here);
            if(aux<theMin){
                theMin = aux;
                jmin=j;
            }
        }
        corr[i]=jmin;

        set<int> there = bfs(corr[i],BFS_NB/3,1);
        set<int>::iterator it;
        for(it=there.begin();it!=there.end();it++){
            int j = *it;
            corr[i]=j;
            int aux = evaluate2(i,corr,here);
            if(aux<theMin){
                theMin = aux;
                jmin=j;
            }
        }
        corr[i]=jmin;

        set<int> here2;

        isGood[i]=(evaluate2Large(i,corr,here)<100000*0.01);
    }
}

void optimize4(vector<int>& corr,const int& nSteps){
    for(int k=0;k<nSteps;k++){
        int i = rand()%(n[0]);

        for(int l=0;l<SAMPLE_SIZE;l++){
            samplingVecLarge[l]=rand()%n[0];
        }

        set<int> emp;

        int theMin = evaluate2Large(i,corr,emp);/*
        int jmin = corr[i];
        for(int jj=0;jj<n[1]/25;jj++){
            int j = rand()%n[1];
            corr[i]=j;
            int aux = evaluate2(i,corr,emp);
            if(aux<theMin){
                theMin = aux;
                jmin=j;
            }
        }
        if((theMin<100000*0.03)||!isGood[i]){
            corr[i]=jmin;
        }*/
        isGood[i]=(theMin<100000*0.01);
    }
}

string strOfFloat(const float& f){
    std::stringstream ss;
    ss << std::fixed << std::setprecision(4) << f;
    std::string mystring = ss.str();
    return mystring;
}

void reconstructError(const string& name,vector<int>& corr){
    string filepath = path + name + "_error.off";
    ofstream out (filepath);

    vector<bool> state(n[0],0);

    vector<float> error(n[0],0);

    map<int,int> newindex;
    for(int i=0;i<n[0];i++){
        error[i]=geoDistances[i][corr[i]][1];
    }
    //sort(pointsVec.begin(),pointsVec.end());

    out<<header<<"\n";

    vector<float> colorTriangle(p[0],0);
    for(int i=0;i<p[0];i++){
        colorTriangle[i]=max(error[triangles[i][0][0]],max(error[triangles[i][1][0]],error[triangles[i][2][0]]))/0.2;
        colorTriangle[i]=min((float)1.0,colorTriangle[i]);
    }

    out<<n[0]<<" "<<p[0]<<" "<<0<<"\n";
    for(int i=0;i<n[0];i++){
        if(true){
            out<<vert[i][0][0]<<" "<<vert[i][1][0]<<" "<<vert[i][2][0]<<"\n";
        }
    }
    for(int i=0;i<p[0];i++){
        float r = colorTriangle[i];
        float g = 0.0;
        float b = 1.0 - colorTriangle[i];
        out<<3<<" "<<triangles[i][0][0]<<" "<<triangles[i][1][0]<<" "<<triangles[i][2][0]<<" "<<strOfFloat(r)<<" "<<strOfFloat(g)<<" "<<strOfFloat(b)<<" "<<"1.0"<<"\n";
    }
}

void reconstructError2(const string& name,vector<int>& corr){
    string filepath = path + name + "_error2.off";
    ofstream out (filepath);

    vector<bool> state(n[0],0);

    vector<float> error(n[0],0);

    map<int,int> newindex;
    for(int i=0;i<n[0];i++){
        error[i]=evaluate(i,corr);
    }
    //sort(pointsVec.begin(),pointsVec.end());

    out<<header<<"\n";

    vector<float> colorTriangle(p[0],0);
    for(int i=0;i<p[0];i++){
        colorTriangle[i]=max(error[triangles[i][0][0]],max(error[triangles[i][1][0]],error[triangles[i][2][0]]))/(0.1*100000);
        colorTriangle[i]=min((float)1.0,colorTriangle[i]);
    }

    out<<n[0]<<" "<<p[0]<<" "<<0<<"\n";
    for(int i=0;i<n[0];i++){
        if(true){
            out<<vert[i][0][0]<<" "<<vert[i][1][0]<<" "<<vert[i][2][0]<<"\n";
        }
    }
    for(int i=0;i<p[0];i++){
        float r = colorTriangle[i];
        float g = 0.0;
        float b = 1.0 - colorTriangle[i];
        out<<3<<" "<<triangles[i][0][0]<<" "<<triangles[i][1][0]<<" "<<triangles[i][2][0]<<" "<<strOfFloat(r)<<" "<<strOfFloat(g)<<" "<<strOfFloat(b)<<" "<<"1.0"<<"\n";
    }
}

void dijkstraFill(const int& start,const int& mesh){
    for(int i=0;i<n[mesh];i++){
        geoDistances[start][i][mesh] = INFINITE;
    }
    geoDistances[start][start][mesh] = 0;

    vector<bool> alreadyComputed(n[mesh],0);

    priority_queue<pair<float,int> > frontier;
    frontier.push(make_pair(0,start));

    while(!frontier.empty()){
        float nextVal = -frontier.top().first;
        int nextInd = frontier.top().second;
        frontier.pop();
        if(!alreadyComputed[nextInd]){
            for(int i = 0;i < int(graphMesh[nextInd][mesh].size());i++){
                int k = graphMesh[nextInd][mesh][i];
                float newDist =  nextVal + shortDist(nextInd,k,mesh);
                if(newDist<geoDistances[start][k][mesh]){
                    geoDistances[start][k][mesh] = newDist;
                    frontier.push(make_pair(-newDist,k));
                }
            }
        }
        alreadyComputed[nextInd]=true;
    }
}

float simpleGlobalEval(const vector<int>& corr){
    double res = 0;
    for(int i=0;i<n[0];i++){
        res += shortDist(i,corr[i],1)/n[0];
    }
    return res;
}

float globalEval(const vector<int>& corr){
    double res = 0;
    for(int i=0;i<n[0];i++){
        res += geoDistances[i][corr[i]][1]/n[0];
    }
    return res;
}

int defineTs(const vector<int>& corr){
    vector<int> aux;
    int SZ = 100;
    for(int i=0;i<SZ;i++){
        int k = rand()%(n[0]);
        //set<int> here = bfs(i,10,0);
        aux.push_back(evaluate(k,corr));
    }
    sort(aux.begin(),aux.end());
    return aux[1*SZ/4];
}

void verifySymettry(const int& mesh){
    float maxDif = 0.0;
    for(int i=0;i<n[mesh];i++){
        for(int j=0;j<n[mesh];j++){
            maxDif = max(maxDif,abs(geoDistances[i][j][mesh]-geoDistances[j][i][mesh]));
        }
    }
    cout << "difference on mesh " << mesh << " is " << maxDif << "\n";
}

int main(){
    srand(time(0));
    ios_base::sync_with_stdio(0);

    vector<int> corr = loadCorr("corr68_60");
    loadMesh("gtfaustoff_all68",0);
    loadMesh("gtfaustoff_all60",1);
    cout << "loading ok ... \n\n";

/*
    for(int i=n[0]-1;i>=0;i--){
        int k = rand()%(i+1);
        int aux = corr[i];
        corr[i]=corr[k];
        corr[k]=aux;
    }
*/


    for(int i=0;i<n[0];i++){
        dijkstraFill(i,0);
        if(i%100==99) cout << "Dijkstra on mesh " << 1 << " ... " << i+1 << "/" << n[0] << " done.\n" << flush;
    }

    for(int i=0;i<n[1];i++){
        dijkstraFill(i,1);
        if(i%100==99) cout << "Dijkstra on mesh " << 2 << " ... " << i+1 << "/" << n[1] << " done.\n" << flush;
    }

    cout << "\n";

    verifySymettry(0);
    verifySymettry(1);

    vector<bool> test(n[0],0);
    isGood = test;

    cout << endl << globalEval(corr) << endl << endl;
    reconstructError("gtfaustoff_all68_before",corr);

    for(int k=1;k<=300;k++){
        cout << "1000 steps No." << k << "\n"<<flush;
        //optimize(corr,500);
        //cout << globalEval(corr) << endl;
        //int ts = defineTs(corr);
        optimize4(corr,15000);
        cout << globalEval(corr) << endl;
        optimize3(corr,1000);
        cout << globalEval(corr) << endl;
    }

    int ts = defineTs(corr);

    int nbCorrect = reconstructCorrectGeo("gtfaustoff_all68_2",-1,corr,0);
    reconstructError("gtfaustoff_all68",corr);
    reconstructError2("gtfaustoff_all68",corr);
    cout << "Correct : " << nbCorrect << " / " << n[0] << endl;

    cout << endl << globalEval(corr) << endl << endl;

    saveCorr("corr68_60__2",corr);

    return 0;
}
