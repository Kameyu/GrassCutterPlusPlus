#include "boost/locale.hpp"
#include "Grasscutter.h"
#include "Crypto.h"
#include "json/json.h"

//Required to init mongo client
#include <filesystem>
#include <string>

#include "mongocxx/client.hpp"
#include "mongocxx/instance.hpp"
#include "mongocxx/uri.hpp"

#include "resource.h"

namespace fs = std::filesystem;

Grasscutter::Grasscutter()
{
	init();
}

inline void Grasscutter::init()
{
	loadConfig();
	// TODO : Mongo Logger once config done
	setCrypto(new Crypto);
	setConstants(new GameConstants);
	cryptoModule->setEncryptSeed(std::stoull("11468049314633205968"));
	cryptoModule->loadKeys(resolveJsonPath(configFile, "folderStructure.resources").asString()); // Load keys once Crypto module is up

	/*
	 * Server integrity checkup
	 */
	if (!integrityCheckup())
		shutdown(IMPORTANT, "Server integrity checkup failed.");

	// TODO : language
	loadLanguage(resolveJsonPath(configFile, "language.language").asString());

	/* TODO : Init mongocxx client */
	mongocxx::instance instance{}; // This should be done only once. Init the instance.
	const mongocxx::client client(mongocxx::uri{});  // default URI string : mongodb://localhost:27017, no need to specify it for now
	//const mongocxx::client client(mongocxx::uri(configFile["databaseInfo"]["server"]["connectionUri"].asString())); // <--- Will be used when server is fully operational
	const mongocxx::database db = client["grasscutter"];
	mongocxx::collection coll = db["accounts"];
	/* MongoCxx connection is up
	 * do stuff... */
}

/**
 * \brief Loads the config file from the root directory
 */
void Grasscutter::loadConfig()
{
	Json::Reader reader;
	std::string s;
	if (std::ifstream file("./config.json", std::ios::binary); file)
		s = std::string((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());
	else
	{
		std::cerr << "config.json not found, generating new default... ";
		generateDefaultConfig(); // will overwrite configFile and write out config.json
		std::cerr << "Done." << std::endl; // TODO : add return check
		return;
	}
	//			src		dest
	reader.parse(s, this->configFile);
}

/**
 * \brief Loads the language file into the emulator class
 * \param langCode contains the formatted language code as locale, e.g. en_US
 */
void Grasscutter::loadLanguage(std::string langCode)
{
	std::string RC_DIR = resolveJsonPath(configFile, "folderStructure.resources").asString();
	langCode[2] = '-'; // replace _ with - as GC Java uses dashes instead of underscores - I'm too lazy to edit everything manually
	this->lang = langCode;

	// get contents of language file
	Json::Reader reader;
	std::string s;
	if (std::ifstream file(RC_DIR+std::string("languages/"+this->lang+".json"), std::ios::binary); file)
		s = std::string((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());
	else
	{
		if (lang != "en-US")
		{
			std::cerr << RC_DIR+this->lang << ".json not found. Loading fallback en-US instead.";
			loadLanguage("en-US");
		}
		else
			shutdown(CRITICAL, "languages/en-US.json not found.");
		return;
	}
	//			src		dest
	reader.parse(s, this->languageFile);
}

/**
 * \brief If config.json cannot be found, will generate a new file.
 * The default file is compiled as a resource file within the code (defaultconfig.json).
 */
void Grasscutter::generateDefaultConfig()
{
	Json::Reader reader;
	const std::string s = LoadResource(IDR_JSON1, L"JSON");
	reader.parse(s, this->defaultConfig);

	// set system language in default config file
	defaultConfig["language"]["language"] = boost::locale::util::get_system_locale().substr(0, 5).c_str();

	// define which file to write, and write it
	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "	";
	const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

	// define which file to write, and write it
	std::ofstream outputFileStream("./config.json");
	writer->write(defaultConfig, &outputFileStream);
	outputFileStream.close();

	setConfig(defaultConfig);
}

/**
 * \brief Will check if the folders are present in the resources directory
 * \return true if folders exist, false if folders do not exist or if gacha
 * file is missing in the data directory.
 */
bool Grasscutter::integrityCheckup()
{
	// GameServer resources (not App resources)
	const std::string res = configFile["folderStructure"]["resources"].asString();

	// if resources dir does not exist, BinOutput & ExcelBinOutput do not exist either
	if (!fs::directory_iterator(res)->exists())
	{
		std::cout << "resources folder not found. Creating and exiting..." << std::endl;
		fs::create_directories(res+"BinOutput/");
		fs::create_directory(res+"ExcelBinOutput/");
		return false;
	}

	// BinOutput + ExcelBinOutput
	if (!fs::directory_iterator(res+"BinOutput/")->exists())
	{
		std::cout << "resources/BinOutput folder not found. Creating and exiting..." << std::endl;
		fs::create_directory(res+"BinOutput/");
		return false;
	}

	if (!fs::directory_iterator(res+"ExcelBinOutput/")->exists())
	{
		std::cout << "resources/ExcelBinOutput folder not found. Creating and exiting..." << std::endl;
		fs::create_directory(res+"ExcelBinOutput/");
		return false;
	}

	// game data checkup
	if (!fs::directory_iterator(configFile["folderStructure"]["data"].asString())->exists())
	{
		std::cout << "data/ folder not found or incomplete. Creating and exiting..." << std::endl;
		fs::create_directories(configFile["folderStructure"]["data"].asString());

		if (is_empty(*fs::directory_iterator(configFile["folderStructure"]["data"].asString())))
			return false;

		if (!fs::directory_iterator(configFile["folderStructure"]["data"].asString()+"mappings.js")->exists())
		{
			std::cerr << "Unable to locate gacha mappings in data/" << std::endl;
			return false;
		}
	}
	return true;
}

void Grasscutter::setConfig(const Json::Value& cnf)
{
	this->configFile = cnf;
}

void Grasscutter::setCrypto(Crypto* crypto)
{
	this->cryptoModule = crypto;
}

void Grasscutter::setConstants(GameConstants* gc)
{
	this->constants = gc;
}