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
    
    // skip newlines between data blocks
    char ch;
    inptr->get(ch);
    while (ch == '\n' || ch == '\r') {
        inptr->get(ch);
    }
    
    while (ch != '\n' && ch != '\r')
    {
        inptr->unget();
        
        *inptr >> from_machine_num;
        *inptr >> to_machine_num;
        *inptr >> flow_amount;
        *inptr >> flow_cost;
        
        machines[from_machine_num-1]->add_flow(machines[to_machine_num - 1], to_machine_num,flow_amount, flow_cost);
        
        inptr->get(ch); // block ends in 2 newlines
        if (ch == '\r') {
            // make it work for windows or unix line endings
            inptr->get(ch);
        }
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
    // open text containing data file
    string fname = "Data/DataSet3.txt";
    ifstream* inptr = new ifstream;
    inptr->open(fname);
    if (inptr->fail())
    {
        cout << "Couldn't open file" << endl;
        return 1;
    }
    
    // read in file and assign data to Factory object
    Factory* myfactory;
    myfactory = file_io(inptr);
    
    // for each machine, sort flows by unit cost * amount of flow
    myfactory->sort_flows();
    
    /*
    // assign starting machine to a region in the middle
    myfactory->set_first_region(1);
    
    // assign the machines that the first machine flows to to the regions
    // closest to the first machine, starting with the machines with the
    // most expensive flow. Continue this process for all machines
    myfactory->set_all_other_regions();
    
    // display initial factory setup and its total cost
    cout << "Initial setup" << endl;
    myfactory->print_factory();
    myfactory->print_total_cost();
    cout << "--------------------------" << endl << endl;
    
    // if switching two machines' locations would decrease total price, make the switch
    // continue this process until price converges (i.e. switching no longer helps)
    myfactory->switch_all_machines();
    
    cout << "Best setup found:" << endl;
    myfactory->print_factory();
    myfactory->print_total_cost();
     */
    
    
    // loop through all machines for starting machine
    Factory best_factory = *myfactory;
    float best_cost = -1;
    for (int i = 0; i < myfactory->get_num_machines(); i++) {
        // assign starting machine to a region in the middle
        myfactory->set_first_region(i+1);
        
        // assign the machines that the first machine flows to to the regions
        // closest to the first machine, starting with the machines with the
        // most expensive flow. Continue this process for all machines
        myfactory->set_all_other_regions();
        
        // display initial factory setup and its total cost
        //cout << "Initial setup" << endl;
        //myfactory->print_factory();
        //myfactory->print_total_cost();
        //cout << "--------------------------" << endl << endl;
        cout << i+1 << endl;
        
        // if switching two machines' locations would decrease total price, make the switch
        // continue this process until price converges (i.e. switching no longer helps)
        myfactory->switch_all_machines();
        
        // keep the factory has the lowest cost of all the starting machines
        float cost_after_switches = myfactory->get_total_cost();
        if (best_cost == -1 || cost_after_switches < best_cost) {
            best_factory = *myfactory;
            best_cost = cost_after_switches;
        }
        
        // reset so can try a new starting machine
        myfactory->reset_regions();
        //cout << "Reseting Factory" << endl;
        //cout << "--------------------------" << endl << endl;
    }
    
    // display lowest cost solution found
    cout << "Best setup found:" << endl;
    best_factory.print_factory();
    cout << "Total cost = $" << fixed << setprecision(2) << best_cost << endl;
    
    
    // deallocate memory
    delete myfactory;
    inptr->close();
    delete inptr;
    return 0;
}
