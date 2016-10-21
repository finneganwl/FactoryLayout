//
//  machine.h
//  IEMS313Phase1
//
//  Created by Will Finnegan on 10/7/16.
//  Copyright © 2016 Will Finnegan. All rights reserved.
//

#ifndef machine_h
#define machine_h

#include <stdio.h>
#include <vector>

class Machine;

class Flow
{
private:
    Machine* to_machine;
    int to_machine_num;
    int amount;
    float cost; // per unit distance
public:
    Flow(Machine* to_machine, int to_machine_num, int amount, float cost);
};

class Machine {
private:
    std::vector<int> set_up_costs;
    std::vector<Flow*> flows;
    //int region;
public:
    Machine(int num_regions) : set_up_costs(num_regions, 0) {}; // using vector constructor
    ~Machine();
    void add_flow(Machine* to_machine, int to_machine_num, int amount, float cost);
    void add_set_up_cost(int region, int cost);
    //void set_region(int region);
    //int get_region();
};

#endif /* machine_h */
