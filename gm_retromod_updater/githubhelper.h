#include <string>
#include <Urlmon.h>
#include <atlbase.h>
#include <stdio.h>
#include <nlohmann/json.hpp>
#pragma comment(lib, "urlmon.lib")

using namespace std;
using json = nlohmann::json;

string GetLatestVersion();
bool DownloadRelease(string version);