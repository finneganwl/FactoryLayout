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
    
    // helper functions
    static bool more_expensive_flow(Flow* flow1, Flow* flow2);
    void set_region(int x, int y, int machine_num);
    int find_machine_to_set(Machine* center_machine, int num_set, int max_num_set);
    int set_region_at_dist(int center_x, int center_y, Machine* center_machine, int dist, int num_set, int max_num_set);
    void set_regions_flows_to(int center_x, int center_y);
    int manh_dist(int from_region_num, int to_region_num);
    
public:
    Factory(int height_, int width_, std::vector<Machine*> machines_)
        :   regions(height_, std::vector<Machine*>(width_)),
            height(height_), width(width_),
            machines(machines_) {};
    ~Factory();
    void sort_flows();
    int get_region_num(int x, int y);
    Position get_region_pos(int region_num);
    void set_first_region(int machine_num);
    void set_all_other_regions();
    float get_total_cost();
    void print_factory();
};

#endif /* factory_h */
