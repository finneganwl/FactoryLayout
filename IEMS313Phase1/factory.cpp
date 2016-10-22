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

/**
 * calculates manhattan distance from start to end in a rectangular region
 *
 * width is width of region
 * from_region and to region refer to region numbers starting at 1 in top left corner
 * increasing left to right, then top to bottom
 */
int Factory::manh_dist(int from_region_num, int to_region_num)
{
    Position from_pos = get_region_pos(from_region_num);
    Position to_pos = get_region_pos(to_region_num);
    int x_dist = std::abs(from_pos.x - to_pos.x);
    int y_dist = std::abs(from_pos.y - to_pos.y);
    return x_dist + y_dist;
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
        std::vector<Flow*> flows = machine->get_flows();
        std::sort(flows.begin(), flows.end(), more_expensive_flow);
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

Position Factory::get_region_pos(int region_num) {
    int x = (region_num-1) % this->width;
    int y = (region_num-1) / this->width;
    Position pos;
    pos.x = x;
    pos.y = y;
    return pos;
}

void Factory::set_region(int x, int y, int machine_num) {
    Machine* machine = this->machines[machine_num-1];
    machine->set_region(this->get_region_num(x,y));
    this->regions[y][x] = machine;
}

void Factory::set_first_region(int machine_num) {
    int x = width / 2;
    int y = height / 2;
    //int machine_num = 1;
    set_region(x, y, machine_num);
}

int Factory::find_machine_to_set(Machine* center_machine, int num_set, int max_num_set) {
    // find a machine that the center one flows to that hasn't been set yet
    Machine* to_machine;
    while (num_set < max_num_set) {
        to_machine = center_machine->get_flows()[num_set]->to_machine;
        if (to_machine->region_is_set()) {
            num_set++;
        }
        else {
            // found one
            break;
        }
    }
    return num_set;
}

/*
 * Sets the regions for as many of the machines that center_machine flows to as it can fit
 * within the given distance dist. Returns the number of machines set or already set in the
 * flow array for the center_machine so that the function can be called again with a higher
 * dist and starting num_set;
 */
int Factory::set_region_at_dist(int center_x, int center_y, Machine* center_machine, int dist, int num_set, int max_num_set) {
    // go through all points of manhattan distance dist from center
    for (int x = center_x - dist; x <= center_x + dist; x++) {
        if (x >= this->width || x < 0) {
            continue;
        }
        int y_dist = dist - std::abs(center_x-x);
        int upper_y = center_y - y_dist;
        int lower_y = center_y + y_dist;
        
        // find a machine that the center one flows to that hasn't been set yet
        num_set = find_machine_to_set(center_machine, num_set, max_num_set);
        if (num_set >= max_num_set) {
            break;
        }
        Machine* to_machine = center_machine->get_flows()[num_set]->to_machine;
        
        if (upper_y >= 0 && this->regions[upper_y][x] == nullptr) {
            set_region(x, upper_y, to_machine->get_machine_num());
            num_set++;
        }
        
        // repeat for bottom part of manhattan circle (if exists)
        num_set = find_machine_to_set(center_machine, num_set, max_num_set);
        if (num_set >= max_num_set) {
            break;
        }
        to_machine = center_machine->get_flows()[num_set]->to_machine;
        // third condition because if y_dist == 0 , lower_y and upper_y are same
        if (lower_y < this->height && this->regions[lower_y][x] == nullptr && y_dist != 0) {
            set_region(x, lower_y, to_machine->get_machine_num());
            num_set++;
        }
    }
    return num_set;
}

/*
 * Sets regions for machines to which the machine at center_x, center_y flows.
 * Puts the machines with the highest cost*amount of flow closest to the center machine
 */
void Factory::set_regions_flows_to(int center_x, int center_y) {
    Machine* center_machine = regions[center_y][center_x];
    int num_out_flows = center_machine->get_flows().size();
    int num_set = 0;
    int dist = 1;
    while (num_set < num_out_flows) {
        int num_new = set_region_at_dist(center_x, center_y, center_machine, dist, num_set, num_out_flows);
        //std::cout << center_machine->get_machine_num() << std::endl;
        //print_factory();
        //std::cout << "---------------------------" << std::endl;
        num_set += num_new;
        dist++;
    }
}

/*
 * Sets regions for all other machines assuming one machine has already been placed
 */
void Factory::set_all_other_regions() {
    int num_machines = machines.size();
    std::vector<bool> done_machines(num_machines, false);
    
    bool done_with_all = false;
    int num_iterations = 0; // logic below assumes all machines are connected eventually
                            // may loop infinitely if this is not the case
                            // or if we start on machine with no out flows.
                            // backup plan is to assign another one and then try again
    while (done_with_all == false) {
        if (num_iterations > num_machines) {
            // if in backup plan, assign first unassigned machine to first unavailable region
            for (int i = 0; i < num_machines; i++) {
                if (!machines[i] -> region_is_set()) {
                    for (int x = 0; x < this->width; x++) {
                        for (int y = 0; y < this->height; y++) {
                            if (this->regions[y][x] == nullptr) {
                                set_region(x, y, machines[i]->get_machine_num());
                                num_iterations = 0;
                                std::cout << "THAT WEIRD CASE" << std::endl;
                                goto finish;
                            }
                        }
                    }
                }
            }
        }
        finish:
        // only set surrounding machines of a machine if it's already been placed
        for (int i = 0; i < num_machines; i++) {
            if (!done_machines[i] && machines[i]->region_is_set()) {
                Position center_position = get_region_pos(machines[i]->get_region());
                set_regions_flows_to(center_position.x, center_position.y);
                done_machines[i] = true;
            }
        }
        // check if all machines have placed their surrounding machines
        done_with_all = true;
        for (int i = 0; i < num_machines; i++) {
            if (done_machines[i] == false) {
                done_with_all = false;
                break;
            }
        }
        num_iterations++;
    }
}

float Factory::get_total_cost() {
    float total_cost = 0;
    // for each machine
    for (int i = 0; i < machines.size(); i++) {
        // add outgoing flow costs
        std::vector<Flow*> flows = machines[i]->get_flows();
        for (int j = 0; j < flows.size(); j++) {
            float unit_cost = flows[j]->amount * flows[j]->cost;
            int dist = manh_dist(machines[i]->get_region(), flows[j]->to_machine->get_region());
            total_cost += unit_cost * dist;
        }
        // add setup cost
        total_cost += machines[i]->get_set_up_cost();
    }
    return total_cost;
}

void Factory::print_factory() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (regions[y][x] == nullptr) {
                std::cout << "  .";
            }
            else {
                std::cout << std::setw(3) << regions[y][x]->get_machine_num();
            }
        }
        std::cout << std::endl;
    }
}

