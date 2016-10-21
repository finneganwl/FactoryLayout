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
#include "machine.h"

class Factory {
private:
    int height;
    int width;
    std::vector<Machine*> machines;
    std::vector< std::vector<Machine*> > regions;
    
public:
    Factory(int height_, int width_, std::vector<Machine*> machines_)
        : regions(height_, std::vector<Machine*>(width_)),
        height(height_), width(width_),
        machines(machines_){};
    ~Factory();
    int get_total_cost();
};

#endif /* factory_h */
