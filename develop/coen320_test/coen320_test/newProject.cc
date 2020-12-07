#include <cstdlib>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <limits.h>

#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

#include "TaskScheduler.h"

int main(int argc, char *argv[]) {
	char* cwd;
    char buff[PATH_MAX + 1];
	cwd = getcwd( buff, PATH_MAX + 1 );
    if( cwd != NULL ) {
        std::cout <<  "My working directory is " << cwd << std::endl ;
    }
    
	std::cout << "Welcome to the Momentics IDE" << std::endl;
	try{
		start();
		//std::system("pause");
	}
	catch (std::runtime_error e)
	{
        std::cout << ":(" << std::endl;
	}
    
	return EXIT_SUCCESS;
}
