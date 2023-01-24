/*#include <iostream>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>*/

// Main includes
#include "Grasscutter.h"

int main(int argc, char** argv)
{
	//TODO : args, logger
	auto* app = new Grasscutter(); // init server
	std::system("pause");
}
