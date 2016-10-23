//
//  factory.h
//  IEMS313Phase1
//
//  Created by Will Finnegan on 10/19/16.
//  Copyright © 2016 Will Finnegan. All rights reserved.
//

#ifndef factory_h
#define factory_h

//#include <stdio.h>

#include <vector>
#include <cstdlib> // for abs
#include <algorithm> // for sort
#include <iostream>
#include <iomanip> // for print_factory
#include "machine.h"

struct Position{
    int x;
    int y;
};

class Factory {
private:
    int height;
    int width;
    std::vector<Machine*> machines;
    std::vector< std::vector<Machine*> > regions;
    bool PRINT;
    
    // helper functions
    static bool more_expensive_flow(Flow* flow1, Flow* flow2);
    void set_region(int x, int y, int machine_num);
    int find_machine_to_set(Machine* center_machine, int num_set, int max_num_set);
    int set_region_at_dist(int center_x, int center_y, Machine* center_machine, int dist, int num_set, int max_num_set);
    void set_regions_flows_to(int center_x, int center_y);
    int manh_dist(int from_region_num, int to_region_num);
    void switch_machines(Machine* machine1, Machine* machine2);
    void backup_assignment_plan();
    
public:
    Factory(int height_, int width_, std::vector<Machine*> machines_)
        :   regions(height_, std::vector<Machine*>(width_)),
            height(height_), width(width_),
            machines(machines_)
            {if (machines_.size() > 10) {
                PRINT = false;
            }
            else {
                PRINT = true;
                }
            };
    
    /**
     * Sorts list of flows for each machines so that the first flow in each list
     * is the flow with the largest product of flow amount and flow cost
     */
    void sort_flows();
    
    /**
     * Converts zero indexed x y coordinates to region number
     * region numbers start at one in top left corner and increase
     * left to right, then top to bottom
     */
    int get_region_num(int x, int y);
    
    /**
     * Reverse of get_region_num
     */
    Position get_region_pos(int region_num);
    
    /**
     * Sets the machine with number machine_num to the region in the middle of the factory
     * or closest to it if dimensions are even
     */
    void set_first_region(int machine_num);
    
    /**
     * Sets regions for all other machines assuming one machine has already been placed/
     * Sets the machines that are flowed to by the already set machines quasi recursively,
     * If can't set them in this way, defaults by setting them close to the middle
     */
    void set_all_other_regions();
    
    /**
     * Returns sum of set up and flow costs for all machines
     */
    float get_total_cost();
    
    void print_factory();
    void print_total_cost();
    
    /**
     * For all pairs of machines, checks if swapping them would decrease total cost.
     * If so makes the switch. Continues until no switches result in decrease or 
     * algorithm times out
     */
    void switch_all_machines();
    void reset_regions();
    int get_num_machines();
};

#endif /* factory_h */
