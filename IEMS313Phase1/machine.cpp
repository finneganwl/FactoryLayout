//
//  machine.cpp
//  IEMS313Phase1
//
//  Created by Will Finnegan on 10/7/16.
//  Copyright © 2016 Will Finnegan. All rights reserved.
//

#include "machine.h"

Machine::~Machine()
{
    for (int i = 0; i < flows.size(); i++)
    {
        delete flows[i];
    }
}

void Machine::add_flow(Machine *to_machine, int to_machine_num, int amount, float cost)
{
    Flow* flw = new Flow;
    flw->to_machine = to_machine;
    flw->to_machine_num = to_machine_num;
    flw->amount = amount;
    flw->cost = cost;
    flows.push_back(flw);
}

void Machine::add_set_up_cost(int reg, int cost)
{
    set_up_costs[reg - 1] = cost;
}


void Machine::set_region(int reg)
{
    region = reg;
}

int Machine::get_region()
{
    return region;
}

bool Machine::region_is_set()
{
    return region != -1;
}

std::vector<Flow*> Machine::get_flows()
{
    return flows;
}

int Machine::get_machine_num()
{
    return machine_num;
}

float Machine::get_set_up_cost()
{
    return set_up_costs[region-1];
}
