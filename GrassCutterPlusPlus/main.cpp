/*#include <iostream>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>*/

// Main includes
#include "Grasscutter.h"

int main(int argc, char** argv)
{
	/*mongocxx::instance instance{}; // This should be done only once.
	const mongocxx::uri uri("mongodb://localhost:27017");
	const mongocxx::client client(uri);

	const mongocxx::database db = client["grasscutter"];
	mongocxx::collection coll = db["accounts"];

	mongocxx::cursor cursor = coll.find({});
	for (const auto doc_view : cursor) {
		bsoncxx::document::element elem{ doc_view["token"] };
		if (elem)
			std::cout << elem.get_value().get_string().value << "\n";
	}*/

	//TODO : args, logger
	auto* app = new Grasscutter(); // init server
}
