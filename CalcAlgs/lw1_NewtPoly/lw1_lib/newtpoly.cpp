#include <KPEq/newtpoly.hpp>
#include <cmath>
#include <numeric>

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

KPEq::Interpoll::NewtPoly::NewtPoly(const SrcNodesType &nodesin, NewtCnfgEnumC logariphmic)
    : KPEq::Interpoll::NewtPoly::NewtPoly(nodesin)
{
    for(auto node : this->srcnodes){
        node.first 	= log(	node.first 	);
        node.second	= log(	node.second	);
    }
}

int KPEq::Interpoll::NewtPoly::setConfig(T value, std::size_t pow) { //here need special enum class with names of errors
    this->targetval = value;
    if(logariphmic){
        this->targetval = log(this->targetval);
    }
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
    if(worktable == nullptr)
        return std::nullopt;
    calcWorkTable(worktable, this->polypow); //here find NewtPol than you can use it forward
    this->uptr_worktable.reset(worktable); //here need for auto delete
    auto result = calcValueWithPoly(worktable, this->polypow);
    if(logariphmic)
        result = std::pow(std::numbers::e,result);
    return result;
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
    auto last_idx = nodes.size() - 1;
    return IndexWithOpt(last_idx, IntExtTypeDef::EXTRAPOLATION); // extrapolation with value more than max of nodes value
}

KPEq::Interpoll::T **KPEq::Interpoll::NewtPoly::createWorkTable(const SrcNodesType &srcnodes, std::size_t pow, size_t idx){
    //here need calculate how much nodes and which nodes include into worktable
    if(this->polypow+1 > srcnodes.size())
        return nullptr;

    //окрестность
    IndxRngType indxrange = findIndxRange(srcnodes.size(), this->polypow, idx);
    this->polypow = indxrange.second - indxrange.first;
    if(this->polypow < 1) // 2 nodes min require
        return nullptr;

    this->workTblRows = this->polypow+1;
    this->workTblCols = this->polypow+2;
    T** result = new T*[workTblRows];
    for(size_t i = 0; i < workTblRows; i++) {
        result[i] = new T[workTblCols]{}; // {} - zeroing out
        result[i][0] = srcnodes[indxrange.first + i].first;
        result[i][1] = srcnodes[indxrange.first + i].second;
    }
    return result;
}

KPEq::Interpoll::NewtPoly::IndxRngType KPEq::Interpoll::NewtPoly::findIndxRange(int size, int rangelen, int ref){
    int min = ref;
    int max = ref;

    int rest = rangelen % 2;
    int half = rangelen / 2;

    max = ref + half;
    min = ref - half;
    if( max > size - 1 ) {
        min -= max - (size - 1) + rest;
        max = size - 1;
    }
    else
    if( min < 0)
    {
        max -= min - rest;
        min = 0;
    }
    else
    {
        if(max == size - 1)
            min -= rest;
        else
            max += rest;
    }
    if( max > size - 1)
        throw std::runtime_error("Wrong range");

    //отслеживаем тут и монотонность по second координате (т.к. на первую возложили ответственность на сортировку,
    //но нужно сначала определить в какую сторону,
    //а также может быть ситуация с меняющимся знаком несколько раз
    //а также требуется хотя бы 2 точки и если меньше, то ошибка ведь (проверять во вне)

    bool hasIncreasing = true;
    T prevtemp = srcnodes[min].second;
    for(auto i = min + 1; i <= max; i++){
        T cur = srcnodes[i].second;
        bool isIncreasing = prevtemp < cur;
        if(hasIncreasing != isIncreasing){
            hasIncreasing = hasIncreasing == true ? false : true; // just toogle
            min = i-1; //now min after this
        }
        prevtemp = srcnodes[i].second;
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

