/*!
 * \file error.cpp
 *
 * \brief This class defines all the errors
 *
 * \author Suraj Chafle <schafle@hawk.edu>
 * 
 * \date Sun Jun 26 16:31:18 CDT 2016
 */

#include <iostream>
#include <stdio.h>

#include "easylogging++.h"

void error(const char *msg)
{
    LOG(ERROR) << msg;
    //exit(1);
}
