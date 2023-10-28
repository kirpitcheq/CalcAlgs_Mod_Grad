#include <KPEq/newtpoly.hpp>

KPEq::Interpoll::NewtPoly::NewtPoly(const SrcNodesType &nodesin)
    : srcnodes(nodesin)
{
    // if(!validNodes(nodesin)) //check nodesin with unique
    // break;
    std::sort(begin(srcnodes), end(srcnodes), [](auto A, auto B){return A.first <= B.first;});//need sort by x nodesin
    //validation: need upper values and unique values and write values (on 1 value x only 1 value y)
    auto end_of_unique = std::unique(begin(srcnodes),end(srcnodes), [](auto A, auto B){return A.first == B.first;});
    if(std::distance(begin(srcnodes), end_of_unique) != srcnodes.size()) {
        err_code = -1; //wrong input table
    }
    //if end_of_unique distance diff of srcnodes.size() => not unique x! must be unique!

}

int KPEq::Interpoll::NewtPoly::setConfig(T value, std::size_t pow) { //here need special enum class with names of errors
    this->targetval = value;
    this->polypow = pow;
    if(err_code != 0)
        return -1;
    if(pow >= srcnodes.size())
        return -2;
    if(value < srcnodes[0].first || value > srcnodes[srcnodes.size()-1].first) // must be sort by ascending before
        return 2; //EXTRAPOLLATION
    return 1; //here fine
}

std::optional<KPEq::Interpoll::T> KPEq::Interpoll::NewtPoly::calc() { //here must be calc work table before
    if(err_code != 0){
        return std::nullopt;
    }
    auto idx_wopt = findIndexFrom(srcnodes, this->targetval);

    if(idx_wopt.second == IntExtTypeDef::EXTRAPOLATION)
        std::cout << "Extrapollation!" << std::endl;
    T ** worktable = createWorkTable(this->srcnodes, this->polypow, idx_wopt.first);
    calcWorkTable(worktable, this->polypow); //here find NewtPol than you can use it forward
    this->uptr_worktable.reset(worktable); //here need for auto delete
    return calcValueWithPoly(worktable, this->polypow);
}

KPEq::Interpoll::T KPEq::Interpoll::NewtPoly::calcValueWithPoly(T **worktable, size_t pow)
{
    T result = 0;
    for (size_t i = 0; i < pow; i++) {
        T temp = 1;
        for (size_t j = 0; j <= i; j++) {
            T mul = this->targetval - worktable[j][X_COL];
            temp *= mul;
        }
        T difval= worktable[i+1][i+2];
        temp *= difval;
        result += temp;
    }
    result += worktable[0][1];
//    for (size_t i = 0; i < pow; i++) {
//        delete [] worktable[i];
//    }
    return result;
}

KPEq::Interpoll::NewtPoly::IndexWithOpt KPEq::Interpoll::NewtPoly::findIndexFrom(const std::vector<std::pair<T, T> > &nodes, T value) {
    if(value < nodes[0].first)
        return IndexWithOpt(0, IntExtTypeDef::EXTRAPOLATION); //extrapolation with value less than min of nodes value
    for(auto it = nodes.begin(); it != nodes.end(); it++){
        if(value < it->first) {
            return IndexWithOpt(std::distance(begin(nodes), it),IntExtTypeDef::INTERPOLATION);
        }
    }
    return IndexWithOpt(0, IntExtTypeDef::EXTRAPOLATION); // extrapolation with value more than max of nodes value
}

KPEq::Interpoll::T **KPEq::Interpoll::NewtPoly::createWorkTable(const SrcNodesType &srcnodes, std::size_t pow, size_t idx){
    //here need calculate how much nodes and which nodes include into worktable
    if(pow+1 > srcnodes.size())
        return nullptr;
    this->workTblRows = pow+1;
    this->workTblCols = pow+2;
    T** result = new T*[pow+1];

    //окрестность
    IndxRngType indxrange = findIndxRange(srcnodes.size(), pow, idx);
    for(size_t i = 0; i < pow+1; i++) {
        result[i] = new T[pow+2]{}; // {} - zeroing out
        result[i][0] = srcnodes[indxrange.first+i].first;
        result[i][1] = srcnodes[indxrange.first+i].second;
    }
    return result;
}

KPEq::Interpoll::NewtPoly::IndxRngType KPEq::Interpoll::NewtPoly::findIndxRange(int range, int len, int ref){
    int min,max;
    min = max = ref;
    bool toogle = false;
    while(len > 0) {
        if(toogle){
            if(min == 0)
                max++;
            else
                min--;
        }
        if(!toogle){
            if(max == range - 1)
                min--;
            else
                max++;
        }
        toogle = toogle == false ? true : false;
        len--;
    }
    return IndxRngType(min, max);
        // int isEven = 0;
    // if(surround % 2 == 0)
    //     isEven = 1;
    // int half = surround / 2;
    // if(ref-half < 0)
    //     return IndxRngType(0, ref+half+isEven);
    // if(ref+half >= range)
    //     return IndxRngType(ref-half+isEven, range-1);
}

void KPEq::Interpoll::NewtPoly::calcWorkTable(T **&worktable, size_t tablesize){
    /*
         * here need
         * - choose nodes from srcnodes table
         * - create table and fill this nodes with it
         * - calc this table
         * - return this table
         */
    size_t x_plus = 1;
    size_t y_plus = 1;
    for (size_t col = 1; col <= tablesize; col++)
    {
        for (size_t row = 0; row <= tablesize-x_plus; row++)
        {
            worktable[row+x_plus][col+1] =
                (worktable[row+x_plus-1][col] - worktable[row+x_plus][col]) /
                                               (worktable[row][X_COL] - worktable[row+x_plus][X_COL]) ;
        }
        x_plus++;
    }
}

KPEq::Interpoll::T **KPEq::Interpoll::NewtPoly::getLastWorkTable() const{
    T** result = new T*[polypow+1];
    for (size_t i = 0; i < polypow+1; i++) {
        result[i] = new T[polypow+2];
        std::copy(uptr_worktable.get()[i], uptr_worktable.get()[i] + polypow + 2, result[i]);
    }
    return result;
}

