#include <iostream>
#include <vector>
#include "modules/newtpoly.hpp"
#include <iomanip>
#include <optional>

#define STUD_EXAMPLE

using namespace std;
int main(int, char**){
    std::cout << "Hello, from Newton'sPolynomial!\n";
    std::vector<std::pair<int,int>> pairvect;
    std::pair<int,int> pair;
    
    //namespace kpeq::interpoll :D
    //below using need in namespace
    using ValT = double;
    using NodesXY = std::pair<double,double>;
#ifdef STUD_EXAMPLE
    vector<NodesXY> srcnodes = {
        NodesXY{-0.5,   0.707},
        NodesXY{-0.25,  0.924},
        NodesXY{0,      1.0},
        NodesXY{0.25,   0.924},
        NodesXY{0.5,    0.707},
        NodesXY{0.75,   0.383},
        NodesXY{1.0,    0},
    };
#else
    vector<NodesXY> srcnodes = {
        NodesXY{0.0,   0.0},
        NodesXY{3,  3},
    };
#endif
    for(auto i : srcnodes){
        std::cout << setw(10) << i.first << "\t" << i.second << std::endl;
    }

#ifdef WITH_THROW
    try{
        NewtPoly newtpolthrow(srcnodes, 2.5, 5);//with throw
        cout << newtpolthrow.getDesValue() << endl;
        cout << newtpolthrow.getLastWorkTable() << endl;
        //but here this will be work only how local because 'try{HERE}'
    }
    catch(const std::exception & ex){
        cout << "something wrong" << endl;
    };
#endif

    using namespace KPEq;
    NewtPoly newtpol(srcnodes);
    //how check errors with this? only use after this flags
    if(bool wrong = newtpol.isWrong(); wrong == true)
        cout << "something wrong with table" << endl; //maybe not unique
    
#ifdef STUD_EXAMPLE
    auto targetval = 0.6; 
    auto polypow = 4;
#else
    auto targetval = 1.5; 
    auto polypow = 1;
#endif
    if(newtpol.setConfig(targetval, polypow) < 0)
    {
        cout << "wrong config" << endl; //here maybe power more than count of 
        //here may be extrapollation too than need return enum or something else
        return -1;
    }
    //but here maybe extrapollation and this need know when user create this!!!
    std::optional<ValT> desvalue = newtpol.calc(); //if return nullopt - that mean the config is not set
    cout << "The answer is: " << desvalue.value() << endl; //here may be next failures: div to zero if x-x0 = 0 => need x only unique! this failures check when create NewtPoly
    //this value saved into object and may be request again
    // optional<ValT> desvalueagain = newtpol.getDesValue(); //if return nullopt - that mean the config is not set
    // cout << desvalueagain << endl; //this option too

    //this option too and maybe return nullopt
    auto worktable = newtpol.getLastWorkTable();//maybe need get last table for print it
    cout << "Worktable is: " << endl;
    for (size_t i = 0; i < polypow + 1; i++) {
        for (size_t j = 0; j < polypow + 2; j++) {
            cout << setw(10) << worktable[i][j] << "\t";
        }
        cout << endl;
    }
    
    //here need std::cout overload ! than need return getLastTable() special wrapper (clear wrapper) for cout

    //maybe use unique_ptr for save lastTable and auto delete this when newtpol is no more need 
    //we can use throw exception when create object and we can create ctor with exception and without exception with keyword 'noexept' => this object will be more faster than with exceptions

    // newtpol.
    // newtpol.calcWorkTable();
    // int waitkey;
    // cin >> waitkey;
}