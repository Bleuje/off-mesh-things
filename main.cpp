/*****
Etienne JACOB, 25/07/2016
*****
Script maker to convert FAUST registrations from .ply to .off using MeshLab
*****/
#include <bits/stdc++.h>
using namespace std;

int main(){
    ofstream out ("faustGTconversion.bat");

    /// put your MeshLab path here
    out << "set PATH=%PATH%;C:\Program Files\VCG\MeshLab\n";

    for(int i=0;i<100;i++){
        out << "meshlabserver -i tr_reg_" << i/100 << (i/10)%10 << i%10 << ".ply -o gtfaustoff_all" << i << ".off\n";
    }

    /*** SECOND STYLE
    for(int i=0;i<100;i++){
        out << "meshlabserver -i tr_reg_" << i/100 << (i/10)%10 << i%10 << ".ply -o gtfaustoff_person" << i/10 << "_" << i%10 << ".off\n";
    }
    ***/

    out << "pause";

    return 0;
}
