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
	cryptoModule->loadKeys(); // Load keys once Crypto module is up

	/*
	 * Server integrity checkup
	 */
	integrityCheckup();

	/* TODO : Init mongocxx client */
	mongocxx::instance instance{}; // This should be done only once. Init the instance.
	const mongocxx::client client(mongocxx::uri{});  // default URI string : mongodb://localhost:27017, no need to specify it for now
	//const mongocxx::client client(mongocxx::uri(configFile["databaseInfo"]["server"]["connectionUri"].asString())); // <--- Will be used when server is fully operational
	const mongocxx::database db = client["grasscutter"];
	mongocxx::collection coll = db["accounts"];
	/* MongoCxx connection is up
	 * do stuff... */
}

void Grasscutter::loadConfig()
{
	Json::Reader reader;
	std::string s;
	if (std::ifstream file("./config.json", std::ios::binary); file)
		s = std::string((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());
	else
	{
		generateDefaultConfig(); // will overwrite configFile and write out config.json
		return;
	}
	//			src		dest
	reader.parse(s, this->configFile);
}

void Grasscutter::generateDefaultConfig()
{
	// Account
	defaultConfig["account"]["EXPERIMENTAL_RealPassword"] = false;
	defaultConfig["account"]["autoCreate"] = false;
	defaultConfig["account"]["defaultPermissions"] = Json::arrayValue; // empty array
	defaultConfig["account"]["maxPlayer"] = -1;

	// Database Info
	defaultConfig["databaseInfo"]["game"]["collection"] = "grasscutter";
	defaultConfig["databaseInfo"]["game"]["connectionUri"] = "mongodb://localhost:27017";
	defaultConfig["databaseInfo"]["server"]["collection"] = "grasscutter";
	defaultConfig["databaseInfo"]["server"]["connectionUri"] = "mongodb://localhost:27017";

	// Folder Structure
	defaultConfig["folderStructure"]["data"] = "./data/";
	defaultConfig["folderStructure"]["packets"] = "./packets/";
	defaultConfig["folderStructure"]["plugins"] = "./plugins/"; // yet to do
	defaultConfig["folderStructure"]["resources"] = "./resources/";
	defaultConfig["folderStructure"]["scripts"] = "resources:Scripts/";

	// Language
	defaultConfig["language"]["document"] = "EN";
	defaultConfig["language"]["fallback"] = "en_US";
	defaultConfig["language"]["language"] = boost::locale::util::get_system_locale().substr(0, 5).c_str();

	// Server
	defaultConfig["server"]["debugWhitelist"] = Json::arrayValue;
	defaultConfig["server"]["debugBlacklist"] = Json::arrayValue;
	defaultConfig["server"]["debugMode"]["isShowLoopPackets"] = false;
	defaultConfig["server"]["debugMode"]["isShowPacketPayload"] = false;
	defaultConfig["server"]["debugMode"]["logPackets"] = "ALL";
	defaultConfig["server"]["debugMode"]["logRequests"] = "ALL";
	defaultConfig["server"]["debugMode"]["serverLoggerLevel"]["levelInt"] = 10000;
	defaultConfig["server"]["debugMode"]["serverLoggerLevel"]["levelStr"] = "DEBUG";
	defaultConfig["server"]["debugMode"]["servicesLoggerLevel"]["levelInt"] = 20000;
	defaultConfig["server"]["debugMode"]["servicesLoggerLevel"]["levelStr"] = "INFO";
	defaultConfig["server"]["dispatch"]["defaultName"] = "Grasscutter";
	defaultConfig["server"]["dispatch"]["logRequests"] = "NONE";
	defaultConfig["server"]["dispatch"]["regions"] = Json::arrayValue;
	defaultConfig["server"]["game"]["accessAddress"] = "127.0.0.1";
	defaultConfig["server"]["game"]["accessPort"] = 0;
	defaultConfig["server"]["game"]["bindAddress"] = "0.0.0.0";
	defaultConfig["server"]["game"]["bindPort"] = 22102;
	defaultConfig["server"]["game"]["enableConsole"] = true;
	defaultConfig["server"]["game"]["enableScriptInBigWorld"] = false;
	defaultConfig["server"]["game"]["gameOptions"]["avatarLimits"]["multiplayerTeam"] = 4;
	defaultConfig["server"]["game"]["gameOptions"]["avatarLimits"]["singlePlayerTeam"] = 4;
	defaultConfig["server"]["game"]["gameOptions"]["enableShopItems"] = true;
	defaultConfig["server"]["game"]["gameOptions"]["energyUsage"] = true;
	defaultConfig["server"]["game"]["gameOptions"]["fishhookTeleport"] = true;
	defaultConfig["server"]["game"]["gameOptions"]["inventoryLimits"]["all"] = 30000;
	defaultConfig["server"]["game"]["gameOptions"]["inventoryLimits"]["furniture"] = 2000;
	defaultConfig["server"]["game"]["gameOptions"]["inventoryLimits"]["materials"] = 2000;
	defaultConfig["server"]["game"]["gameOptions"]["inventoryLimits"]["relics"] = 2000;
	defaultConfig["server"]["game"]["gameOptions"]["inventoryLimits"]["weapons"] = 2000;
	defaultConfig["server"]["game"]["gameOptions"]["questing"] = false;
	defaultConfig["server"]["game"]["gameOptions"]["rates"]["adventureExp"] = 1.0;
	defaultConfig["server"]["game"]["gameOptions"]["rates"]["leyLines"] = 1.0;
	defaultConfig["server"]["game"]["gameOptions"]["rates"]["mora"] = 1.0;
	defaultConfig["server"]["game"]["gameOptions"]["resinOptions"]["cap"] = 160;
	defaultConfig["server"]["game"]["gameOptions"]["resinOptions"]["rechargeTime"] = 480;
	defaultConfig["server"]["game"]["gameOptions"]["resinOptions"]["resinUsage"] = false;
	defaultConfig["server"]["game"]["gameOptions"]["sceneEntityLimit"] = 1000;
	defaultConfig["server"]["game"]["gameOptions"]["staminaUsage"] = true;
	defaultConfig["server"]["game"]["gameOptions"]["watchGachaConfig"] = false;
	defaultConfig["server"]["game"]["isShowLoopPackets"] = false;
	defaultConfig["server"]["game"]["isShowPacketPayload"] = false;

	Json::Value dat(Json::arrayValue);
	dat.append(2007);
	dat.append(1002);
	dat.append(4010);
	defaultConfig["server"]["game"]["joinOptions"]["welcomeEmotes"] = dat;
	dat.clear();

	defaultConfig["server"]["game"]["joinOptions"]["welcomeMail"]["content"] =
		R"(Hi there!\r\nFirst of all, welcome to Grasscutter. If you have any issues, please let us know so that Kameyu can help you! \r\n\r\nCheck out our:\r\n<type=\"browser\" text=\"Discord\" href=\"https://discord.gg/T5vZU6UyeG\"/>\n)";

	{ //Additional scope was created so temporary values are deleted after `dat` is filled.
		Json::Value lst;
		lst["itemCount"] = 1;
		lst["itemId"] = 13509;
		lst["itemLevel"] = 1;
		dat.append(lst);
		lst.clear();
		lst["itemCount"] = 99999;
		lst["itemId"] = 201;
		lst["itemLevel"] = 1;
		dat.append(lst);
		lst.clear();
	} // `lst` will be removed after this scope, but not `dat`. We don't need `lst` anymore.
	defaultConfig["server"]["game"]["joinOptions"]["welcomeMail"]["items"] = dat;
	dat.clear();

	defaultConfig["server"]["game"]["joinOptions"]["welcomeMail"]["sender"] = "Kameyu";
	defaultConfig["server"]["game"]["joinOptions"]["welcomeMail"]["title"] = "Welcome to Grasscutter!";
	defaultConfig["server"]["game"]["joinOptions"]["welcomeMessage"] = "Welcome to a Grasscutter server.";
	defaultConfig["server"]["game"]["kcpInterval"] = 20;
	defaultConfig["server"]["game"]["loadEntitiesForPlayerRange"] = 100;
	defaultConfig["server"]["game"]["loadPackets"] = "NONE";
	defaultConfig["server"]["game"]["serverAccount"]["adventureRank"] = 1;
	defaultConfig["server"]["game"]["serverAccount"]["avatarId"] = 10000007;
	defaultConfig["server"]["game"]["serverAccount"]["nameCardId"] = 210001;
	defaultConfig["server"]["game"]["serverAccount"]["nickName"] = "Server";
	defaultConfig["server"]["game"]["serverAccount"]["signature"] = "Welcome to Grasscutter !";
	defaultConfig["server"]["game"]["serverAccount"]["worldLevel"] = 0;
	defaultConfig["server"]["http"]["accessAddress"] = "127.0.0.1";
	defaultConfig["server"]["http"]["accessPort"] = 0;
	defaultConfig["server"]["http"]["bindAddress"] = "0.0.0.0";
	defaultConfig["server"]["http"]["bindPort"] = 443;
	defaultConfig["server"]["http"]["encryption"]["keystore"] = "./keystore.p12";
	defaultConfig["server"]["http"]["encryption"]["keystorePassword"] = "123456";
	defaultConfig["server"]["http"]["encryption"]["useEncryption"] = true;
	defaultConfig["server"]["http"]["encryption"]["useInRouting"] = true;
	defaultConfig["server"]["http"]["files"]["errorFile"] = "./404.html";
	defaultConfig["server"]["http"]["files"]["indexFile"] = "./index.html";

	dat.append("*");
	defaultConfig["server"]["http"]["policies"]["cors"]["allowedOrigins"] = dat;
	dat.clear();

	defaultConfig["server"]["http"]["policies"]["cors"]["enabled"] = false;
	defaultConfig["server"]["logCommands"] = false;
	defaultConfig["server"]["runMode"] = "HYBRID";

	// Version
	defaultConfig["version"] = 4;

	// set up Json builder to write config file
	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "	";
	const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

	// define which file to write, and write it
	std::ofstream outputFileStream("./config.json");
	writer->write(defaultConfig, &outputFileStream);
	outputFileStream.close();

	// set current config to default config
	configFile = defaultConfig; // will overwrite comments
}

bool Grasscutter::integrityCheckup()
{
#ifdef _DEBUG
	shutdown(NORMAL, "Skip integrity check");
#endif

	// resources
	if (!std::filesystem::directory_iterator("./resources/")->exists())
	{
		std::cout << "Resources folder not found. Creating and exiting..." << std::endl;
		return false;
	}

	// BinOutput + ExcelBinOutput
	/*if (!std::filesystem::directory_iterator("./resources/BinOutput/")->exists() ||
		!std::filesystem::directory_iterator("./resources/ExcelBinOutput/")->exists())
	{
		std::cout << "Bin folders not found. Creating and exiting..." << std::endl;
		return false;
	}*/

	// game data checkup
	/*std::string s = configFile["folderStructure"]["data"]
	if (!std::filesystem::directory_iterator())*/

	return true;
}

void Grasscutter::setCrypto(Crypto* crypto)
{
	this->cryptoModule = crypto;
}

void Grasscutter::setConstants(GameConstants* gc)
{
	this->constants = gc;
}