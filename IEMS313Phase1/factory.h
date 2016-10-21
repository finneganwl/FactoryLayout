//
//  factory.h
//  IEMS313Phase1
//
//  Created by Will Finnegan on 10/19/16.
//  Copyright © 2016 Will Finnegan. All rights reserved.
//

#ifndef factory_h
#define factory_h

#include <stdio.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include "machine.h"

class Factory {
private:
    int height;
    int width;
    std::vector<Machine*> machines;
    std::vector< std::vector<Machine*> > regions;
    static bool more_expensive_flow(Flow* flow1, Flow* flow2);
    
public:
    Factory(int height_, int width_, std::vector<Machine*> machines_)
        :   regions(height_, std::vector<Machine*>(width_)),
            height(height_), width(width_),
            machines(machines_) {};
    ~Factory();
    void sort_flows();
    int get_region_num(int x, int y);
    void set_region(int x, int y, int machine_num);
    int set_region_at_dist(int center_x, int center_y, int dist, int num_set);
    int get_total_cost();
};

#endif /* factory_h */
