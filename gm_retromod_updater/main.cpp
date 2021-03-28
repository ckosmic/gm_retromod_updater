#define GMMODULE

#include <string>
#include <fstream>

#include "Interface.h"
#include "githubhelper.h"
#include "version.h"

using namespace std;

string g_latestVersion = "";

string GrabLatestVersion() {
	if (g_latestVersion.length() == 0)
		g_latestVersion = GetLatestVersion();
	return g_latestVersion;
}

string GrabLocalVersion() {
	string localVersionString;
	ifstream versionFile("garrysmod\\data\\retromod2\\version.txt");
	if (getline(versionFile, localVersionString)) {
		versionFile.close();
		return localVersionString;
	}
	versionFile.close();
	return "";
}

bool IsUpdateAvailable() {
	string localVersionString = GrabLocalVersion();
	if (localVersionString.length() > 0) {
		Version vLocal(localVersionString);
		Version vOnline(GrabLatestVersion());
		return (vLocal < vOnline);
	}
	return false;
}

LUA_FUNCTION(UpdateRetromod) {
	try {
		if (IsUpdateAvailable())
			LUA->PushBool(DownloadRelease(GrabLatestVersion()));
		else
			LUA->PushBool(false);
	}
	catch (int e) {
		LUA->PushBool(false);
	}
	return 1;
}

LUA_FUNCTION(UpdateAvailable) {
	LUA->PushBool(IsUpdateAvailable());
	return 1;
}

LUA_FUNCTION(GetLocalVersion) {
	LUA->PushString(GrabLocalVersion().c_str());
	return 1;
}

LUA_FUNCTION(GetLatestVersion) {
	LUA->PushString(GrabLatestVersion().c_str());
	return 1;
}

GMOD_MODULE_OPEN() 
{
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
	LUA->PushCFunction(UpdateRetromod);
	LUA->SetField(-2, "UpdateRetromod");
	LUA->PushCFunction(UpdateAvailable);
	LUA->SetField(-2, "UpdateAvailable");
	LUA->PushCFunction(GetLocalVersion);
	LUA->SetField(-2, "GetLocalVersion");
	LUA->PushCFunction(GetLatestVersion);
	LUA->SetField(-2, "GetLatestVersion");

	return 0;
}

GMOD_MODULE_CLOSE() 
{
	return 0;
}