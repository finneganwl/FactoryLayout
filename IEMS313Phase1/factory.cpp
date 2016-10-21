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