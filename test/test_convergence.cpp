#include "approxChol.hpp"
#include "test.h"
#include "matrix.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <random>

float get_random()
{
    static std::default_random_engine e;
    static std::normal_distribution<> dis(0, 1); 
    return dis(e);
}
int main(int argc, char **argv){

    int v = atoi(argv[1]);
    int e = atoi(argv[2]);

    SparseMatrix A(v, e);
    SparseMatrix lap_A;
    laplacian(A, lap_A);
    std::vector<std::vector<Tval>> sol(10, std::vector<Tval>(v, 0.0));

    SolverParameter para;
    para.maxits = 1000;
    para.maxtime = 10000;
    para.verbose = true;
    para.tolerance = 1e-15;

    std::cout << v << " " << e << " ";
    std::vector<Tval> b(v, 0.0);
    for (auto& el: b)
        el = get_random();
    Tval m = mean(b);
    std::cout << "random b:\n";
    for (auto& el : b)
        el -= m;
    
    LLMatOrd_vector2 llmat2 = LLMatOrd_vector2(A);
    std::vector<LLMatOrd_vector2> llmats2(4, llmat2);

    std::cout << "VecMg: ";
    LDLinv ldli1 = approxChol_vector2_merge(llmats2[0]);
    sol[1] = LDLsolver(ldli1, b);
    sol[1] = pcg(lap_A, b, LDLsolver, sol[1], ldli1, para);

    std::cout << "VecMgRand: ";
    LDLinv ldli2 = approxChol_vector2_mergerand(llmats2[1]);
    sol[2] = LDLsolver(ldli2, b);
    sol[2] = pcg(lap_A, b, LDLsolver, sol[2], ldli2, para);

    std::cout << "VecMgSearch:"; 
    LDLinv ldli3 = approxChol_vector2_merge_search(llmats2[2]);
    sol[3] = LDLsolver(ldli3, b);
    sol[3] = pcg(lap_A, b, LDLsolver, sol[3], ldli3, para);

    std::cout << "approxChol_vector2_opt4:"; 
    LDLinv ldli7 = approxChol_vector2_opt4(llmats2[3]);
    sol[7] = LDLsolver(ldli7, b);
    sol[7] = pcg(lap_A, b, LDLsolver, sol[7], ldli7, para);


    LLMatOrd llmat = LLMatOrd(A);
    std::vector<LLMatOrd> llmats(3, llmat);
    std::cout << "Baseline: ";
    LDLinv ldli0 = approxChol(llmats[0]);
    sol[0] = LDLsolver(ldli0, b);
    sol[0] = pcg(lap_A, b, LDLsolver, sol[0], ldli0, para);

    // std::cout << "Vec: ";
    // LDLinv ldli6 = approxChol_opt2(llmats[1]);
    // sol[6] = LDLsolver(ldli6, b);
    // sol[6] = pcg(lap_A, b, LDLsolver, sol[6], ldli6, para);


    
    

    LLMatOrd_vector2_struct llmat3 = LLMatOrd_vector2_struct(A);
    std::vector<LLMatOrd_vector2_struct> llmats3(2, llmat3);

    std::cout << "VecStructMg:"; 
    LDLinv ldli4 = approxChol_vector2_struct_merge(llmats3[0]);
    sol[4] = LDLsolver(ldli4, b);
    sol[4] = pcg(lap_A, b, LDLsolver, sol[4], ldli4, para);

    std::cout << "VecStructMgSearch:"; 
    LDLinv ldli5 = approxChol_vector2_struct_merge_search(llmats3[1]);
    sol[5] = LDLsolver(ldli5, b);
    sol[5] = pcg(lap_A, b, LDLsolver, sol[5], ldli5, para);

    

    std::cout << "\n";

    return 0;

}