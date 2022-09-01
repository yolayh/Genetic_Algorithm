#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <math.h>
#include <cstdio>
#include <string.h>

using namespace std;

typedef vector<bool> vec;

//宣告變數
vec solution;
int result;
vector<vec> population;
vector<vec> selection;
vector<vec> children;
vector<int> random_num;


//宣告function
void print_vec(vec);
void print_vec_group(vector<vec>);
void initialization(int, int);
int evaluation(vec);
void determination(vector<vec>);
void selection_rw();
int random_generate();
bool check_repeat(int);
void selection_tour(int);
void crossover(vec, vec, double, double);
vec mutation(vec, double);
void execution(int, int, int, double, double, int, int);



//print individual vector
void print_vec(vec v){
    for(auto i: v){
        cout << i;
    }
    cout << endl;
}

//print group vector
void print_vec_group(vector<vec> v){
    for (int i = 0; i<v.size(); i++){
        for (int k = 0; k <v[i].size(); k++){
            cout << v[i][k];
        }
        cout << endl;
    }
}


//Initialize
void initialization(int bit, int pop){
    population.clear();
    for(int i=0; i<pop; i++){      
        vec individual;
        for (int k =0; k < bit; k++){
            individual.push_back(rand() % 2);   
        }
        population.push_back(individual);
    }
}


//Evaluation
int evaluation(vec v){
    int size = v.size();
    int vec_sum = 0;
    for (int i=0; i< size; i++){
        vec_sum += v[i];
    }
    return vec_sum;
}


//Determination
void determination(vector<vec> group){
    for (int i = 0; i<group.size(); i++){
        if(evaluation(group[i]) > result)
            solution = group[i];
            result = evaluation(group[i]);
    }
}


//Selection*1-Roulette Wheel
void selection_rw(){

    selection.clear();

    //計算各個parent的機率
    double total_score;
    vector<double> probability;
    for (int i = 0; i < population.size(); i++){
        total_score += evaluation(population[i]);
    }
    for (int i = 0; i < population.size(); i++){
        probability.push_back(evaluation(population[i])/total_score);
    }
    
    //隨機生成機率挑選parent，共挑population.size()次
    for (int i = 0; i < population.size(); i++){
        double val = (double)rand() / RAND_MAX;
        double temp = 0.0;

        for (int k = 0; k < probability.size(); k++){ 
            temp += probability[k];
            if(val<temp){
                selection.push_back(population[k]);
                k = probability.size();
            }
        } 
    }
}


//For Tournament
int random_generate(){
    int number = rand() % population.size();
    if (check_repeat(number)){
            random_generate();
    }
    return number;
}

//For Tournament
bool check_repeat(int n){
    bool check_set = false;
    for (int i = 0; i < random_num.size(); i++){
        if (random_num[i] == n){
            check_set = true;
        }
    }
    return check_set;
}

//Selection*2-Tournament; choose: 一次挑幾個出來比
void selection_tour(int choose){
    selection.clear();
    for (int k = 0; k < population.size(); k++){
        vec selected_parent;
        random_num.clear();
        for (int i = 0; i < choose; i++){
            random_num.push_back(random_generate());
        }
        selected_parent = population[random_num[0]];
        int selected_parent_value = evaluation(selected_parent);
        for(int j = 1; j <random_num.size(); j++){
            if(evaluation(population[j]) > selected_parent_value){
                selected_parent = population[j];
                selected_parent_value = evaluation(population[j]);
            }
        }
        selection.push_back(selected_parent);   
    }
}   


//Crossover
void crossover(vec a, vec b, double cross_rate, double mutate_rate){ 
    double cross_or_not = (double)rand() / RAND_MAX;
    int size = a.size();
    if (cross_or_not <= cross_rate){
        int random = (rand() % (size-1))+1; //假設size=6 random只取1~5  
        vec child_1;
        vec child_2;
        for (int i = 0; i < random; i++){
            child_1.push_back(a[i]);
            child_2.push_back(b[i]);
        }
        for (int k = random; k < size; k++){
            child_1.push_back(b[k]);
            child_2.push_back(a[k]);
        }

        children.push_back(mutation(child_1,  mutate_rate));
        children.push_back(mutation(child_2,  mutate_rate));
    }
    else{
        children.push_back(a);
        children.push_back(b);
    }
}


//Mutation
vec mutation(vec v, double mutate_rate){
    double mutate_or_not = (double)rand() / RAND_MAX;
    if (mutate_or_not < mutate_rate){
        int random = rand() % v.size();
        if (v[random] == 1)
            v[random] = 0;
        else
            v[random] = 1;
    }
    return v;
}

//(bit, population, evaluation, cross rate, mutation rate, choose = type 1(Roulette Wheel) or 2(Tournament), select_size:tournament方法下一次選取之數量) 
void execution(int bit, int pop, int evaluate, double cross_r,double mutate_r, int choose, int select_size){
    
    srand((unsigned)time(NULL));
    initialization(bit, pop);
    determination(population);

    print_vec_group(population);
    
    
    for(int i = 0; i < (evaluate/pop); i++){
        
        //選擇selection方式 
        if (choose == 1){
            selection_rw();
        }
        else if (choose == 2){
            selection_tour(select_size);
        }

        //crossover
        children.clear();
        if (pop % 2 == 0){
            for (int k = 0; k<selection.size(); k+=2){
                crossover(selection[k], selection[k+1], cross_r, mutate_r);
            }
        }
        else{
            for (int k = 0; k<selection.size()-1; k+=2){
                crossover(selection[k], selection[k+1], cross_r, mutate_r);
            }
            children.push_back(selection[selection.size()-1]);    //奇數個population下，最後一個挑選到的individual直接加入children
        }

        population = children;
        determination(population);

        print_vec_group(population);

    }

        
}