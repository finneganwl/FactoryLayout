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

struct Flow
{
    Machine* to_machine;
    int to_machine_num;
    int amount;
    float cost; // per unit distance
};

class Machine {
private:
    int machine_num;
    std::vector<int> set_up_costs;
    std::vector<Flow*> flows;
    int region;
public:
    Machine(int num_regions, int machine_num_)
        :   machine_num(machine_num_),
            set_up_costs(num_regions, 0), // using vector constructor
            region(-1) // -1 will represent unset region
            {};
    ~Machine();
    void add_flow(Machine* to_machine, int to_machine_num, int amount, float cost);
    void add_set_up_cost(int reg, int cost);
    void set_region(int reg);
    int get_region();
    bool region_is_set();
    std::vector<Flow*> get_flows();
    int get_machine_num();
    float get_set_up_cost();
    //friend class Factory;
};

#endif /* machine_h */
