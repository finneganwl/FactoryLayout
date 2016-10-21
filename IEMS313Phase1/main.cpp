//
//  main.cpp
//  IEMS313Phase1
//
//  Created by Will Finnegan on 10/7/16.
//  Copyright © 2016 Will Finnegan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include "machine.h"
#include "factory.h"

using namespace std;

/**
 * calculates manhattan distance from start to end in a rectangular region
 *
 * width is width of region
 * from_region and to region refer to region numbers starting at 1 in top left corner
 * increasing left to right, then top to bottom
 */
int manh_dist(int from_region, int to_region, int width)
{
    int x_dist = abs(from_region - to_region) % width;
    int y_dist = abs(from_region - to_region) / width; // floor division
    return x_dist + y_dist;
}

Factory* file_io(ifstream* inptr)
{
    // read in first block of data
    int num_machines;
    int height;
    int width;
    *inptr >> num_machines;
    *inptr >> height;
    *inptr >> width;
    int num_regions = height * width;
    
    // skip newlines between data blocks
    char ch;
    inptr->get(ch);
    inptr->get(ch);
    
    // initialize vector of machines
    // machine 1 is stored in machines[0] !!!
    vector<Machine*> machines(num_machines);
    for (int i = 0; i < num_machines; i++)
    {
        machines[i] = new Machine(num_regions, i + 1);
    }
    
    
    // read in second block of data
    int from_machine_num;
    int to_machine_num;
    int flow_amount;
    float flow_cost;
    inptr->get(ch);
    while (ch != '\n')
    {
        inptr->unget();
        
        *inptr >> from_machine_num;
        *inptr >> to_machine_num;
        *inptr >> flow_amount;
        *inptr >> flow_cost;
        
        machines[from_machine_num-1]->add_flow(machines[to_machine_num - 1], to_machine_num,flow_amount, flow_cost);
        
        inptr->get(ch); // block ends in 2 newlines
        inptr->get(ch);
    }
    
    // read in third block of data
    int set_up_cost;
    for (int i = 1; i <= num_regions; i++) // regions start at 1, not zero
    {
        for (int j = 0; j < num_machines; j++)
        {
            *inptr >> set_up_cost;
            machines[j]->add_set_up_cost(i, set_up_cost);
        }
    }
    
    // initialize factory
    Factory* myfactory = new Factory(height, width, machines);
    
    return myfactory;
}


int main(int argc, const char * argv[]) {
    // open file
    string fname = "Data/DataSet4.txt";
    ifstream* inptr = new ifstream;
    inptr->open(fname);
    if (inptr->fail())
    {
        cout << "Couldn't open file" << endl;
        return 1;
    }
    
    // read in file and assign data to machines
    Factory* myfactory;
    myfactory = file_io(inptr);
    
    // for each machine, sort flows by unit cost * amount of flow
    myfactory->sort_flows();
    
    myfactory->set_region(1, 1, 1);
    myfactory->set_region_at_dist(1, 1, 1, 0);
    

    // start with machine has the most outgoing flows, place it the middle
    // sort list of outgoing machines by flow amount * cost (tiebreaker num outgoing flows)
    // assign the first of the list to the feasible
    
    
    
    // deallocate memory
    delete myfactory;
    
    inptr->close();
    delete inptr;
    return 0;
}
