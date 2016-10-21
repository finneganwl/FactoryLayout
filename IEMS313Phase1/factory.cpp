//
//  factory.cpp
//  IEMS313Phase1
//
//  Created by Will Finnegan on 10/19/16.
//  Copyright © 2016 Will Finnegan. All rights reserved.
//

#include "factory.h"

Factory::~Factory() {
    for (int i = 0; i < machines.size(); i++)
    {
        delete machines[i];
    }
}

/*
 * Takes 2 flows and returns a boolean representing
 * if the cost of the first one is greater than that of the second.
 * Used with sort function
 */
bool Factory::more_expensive_flow(Flow* flow1, Flow* flow2) {
    float cost1 = flow1->cost * flow1->amount;
    float cost2 = flow2->cost * flow2->amount;
    return cost1 > cost2;
}

/*
 * Sorts list of flows for each machines so that the first flow in each list
 * is the flow with the highest product of flow amount and flow cost
 */
void Factory::sort_flows() {
    for (int i = 0; i < machines.size(); i++) {
        Machine* machine = machines[i];
        std::sort(machine->flows.begin(), machine->flows.end(), more_expensive_flow);
    }
}

/*
 * Converts zero indexed x y coordinates to region number
 * region numbers start at one in top left corner and increase
 * left to right, then top to bottom
 */
int Factory::get_region_num(int x, int y) {
    return y * this->width + x + 1;
}

void Factory::set_region(int x, int y, int machine_num) {
    Machine* machine = this->machines[machine_num-1];
    machine->set_region(this->get_region_num(x,y));
    this->regions[y][x] = machine;
}

int Factory::set_region_at_dist(int center_x, int center_y, int dist, int num_set) {
    Machine* center_machine = regions[center_y][center_x];
    int max_num_set = center_machine->flows.size();
    for (int x = center_x - dist; x <= center_x + dist; x++) {
        if (x >= this->width || x < 0) {
            continue;
        }
        int y_dist = dist - std::fabs(center_x-x);
        int upper_y = center_y - y_dist;
        int lower_y = center_y + y_dist;
        if (lower_y >= this->height || upper_y < 0) {
            continue;
        }
        if (num_set >= max_num_set) {
            break;
        }
        if (this->regions[upper_y][x] == nullptr) {
            set_region(x, upper_y, center_machine->flows[num_set]->to_machine_num);
            num_set++;
        }
        if (num_set >= max_num_set) {
            break;
        }
        if (this->regions[lower_y][x] == nullptr && y_dist != 0) { //because if y_dist == 0 , lower_y and upper_y are same
            set_region(x, upper_y, center_machine->flows[num_set]->to_machine_num);
            num_set++;
        }
    }
    return num_set;
}

