#include "GA_onemax.hpp"

int main(int argc, char *argv[]){

    const int bit = atoi(argv[1]);
    const int population = atoi(argv[2]);
    const int evaluation = atoi(argv[3]);
    const double cross_rate = atof(argv[4]);
    const double mutate_rate = atof(argv[5]);
    const int selection_way = atoi(argv[6]);          //1:Roulette Wheel; 2:Tournamnet
    const int tournament_quantity = atoi(argv[7]);
    
    vector<int> all_result;
    
    //run 30 times
    for (int i = 0; i < 30; i++){
        cout << "run-" << i+1 << endl;
        execution(bit, population, evaluation, cross_rate, 
        mutate_rate, selection_way, tournament_quantity);
        all_result.push_back(result);
        cout << endl;
    }
    
    cout << "All result:" << endl;
    
    double average = 0.0;
    for (auto i : all_result){
        average += i;
        cout << i << " ";
    }
    cout << endl;
    average = average/30.0;
    cout << "Average: " << average << endl;


    return 0;

}