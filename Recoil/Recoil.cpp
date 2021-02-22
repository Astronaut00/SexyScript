#include "Recoil.h"
#include "Settings.h"
#include "Utils.h"
#include "skCrypter.h"
#include "Globals.h"
#include <Windows.h> 
#include <Chrono> 
#include <Thread>
#include <fstream>
#include <sstream>
#include <iostream>

namespace recoil
{
	std::array<weapon_info, WEAPON_MAX> weapons;
	using namespace std::chrono;
	bool recoil_checks(bool& crouched);
    int get_game_values();
	int get_keybinds();
	void init();

	const std::vector<std::vector<std::array<double, 2>>>recoil_tables = {
		/* None */		{ { 0.0 } },
		/* AK-47 */		{ { -17.169213, 24.740016 }, { 2.645343, 22.718943 }, { -27.293053, 20.648278 }, { -20.986154, 18.528020 }, { -0.088147, 16.358169 }, { 8.249090, 14.138726 }, { 14.513057, 11.869690 }, { 18.703753, 9.551062 }, { 20.821180, 7.182841 }, { 20.865336, 4.765027 }, { 18.836222, 4.895916 }, { 14.733838, 9.057304 }, { 8.558183, 12.351879 }, { 0.458104, 14.688290 }, { -7.416482, 16.066539 }, { -13.671136, 16.486625 }, { -18.308246, 15.948549 }, { -21.327812, 14.452310 }, { -22.729835, 11.997907 }, { -22.514314, 8.585343 }, { -20.681250, 4.214615 }, { -17.230642, 2.454901 }, { -12.162491, 6.865652 }, { -5.476796, 10.339396 }, { 5.927938, 12.666128 }, { 17.881893, 13.845847 }, { 24.029965, 13.878553 }, { 24.363372, 12.764247 }, { 18.882112, 10.502927 } },
		/* LR-300 */	{ { -1.214385, 12.406517 }, { -3.068991, 15.353016 }, { -4.990952, 16.380538 }, { -7.107006, 15.489083 }, { -7.792356, 12.699350 }, { -7.031537, 10.231337 }, { -4.824550, 8.794116 }, { -1.103092, 7.481138 }, { 4.516583, 6.292402 }, { 8.534249, 5.227908 }, { 9.954713, 4.287657 }, { 8.777975, 3.471649 }, { 5.004034, 2.798281 }, { -0.216487, 2.446719 }, { -2.753139, 2.197909 }, { -4.597158, 1.977744 }, { -6.008460, 1.786225 }, { -6.987044, 1.623352 }, { -7.532911, 1.489125 }, { -7.646061, 1.383543 }, { -7.326494, 1.306608 }, { -6.574209, 1.258318 }, { -5.389206, 1.238674 }, { -3.771487, 1.247676 }, { -1.673765, 1.285324 }, { 6.651075, 1.351617 }, { 15.124492, 1.446557 }, { 17.881128, 1.570142 }, { 14.920983, 1.722373 } },
		/* M249 */		{ { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 }, { -0.000000, 37.345679 } },
		/* MP5 */		{ { -0.000000, 10.726397 }, { -0.000000, 14.486180 }, { 0.000000, 16.397804 }, { 6.301087, 16.461269 }, { 14.529738, 14.676575 }, { 16.198115, 11.043721 }, { 11.306218, 6.472762 }, { 0.409516, 5.321490 }, { -8.935200, 4.752146 }, { -12.297982, 4.213769 }, { -9.749490, 3.706359 }, { -1.849479, 3.229915 }, { 3.824337, 2.784438 }, { 6.863592, 2.369927 }, { 8.299069, 1.986382 }, { 8.130768, 1.633805 }, { 6.358688, 1.312193 }, { 2.982831, 1.021548 }, { -0.706410, 0.761870 }, { -2.658379, 0.533159 }, { -4.201576, 0.335413 }, { -5.366272, 0.168635 }, { -6.152468, 0.032823 }, { -6.560163, -0.072023 }, { -6.589357, -0.145902 }, { -6.240051, -0.188814 }, { -5.512245, -0.200760 }, { -4.405937, -0.181740 }, { -2.921130, -0.131753 } },
		/* Thompson */  { {-7.500000, 16.500000}, {-2.500000, 16.000000}, {1.500000, 15.500000}, {5.500000, 14.500000}, {6.500000, 13.000000}, {5.000000, 11.000000}, {1.000000, 9.000000}, {-3.500000, 8.000000}, {-6.500000, 7.000000}, {-6.500000, 6.500000}, {-3.500000, 5.500000}, {1.000000, 5.000000}, {5.000000, 4.500000}, {6.000000, 4.000000}, {5.500000, 3.500000}, {2.500000, 3.500000}, {-1.000000, 3.000000}, {-3.000000, 3.000000}, {-3.500000, 3.000000} },
		/* Custom */	{ {-6.500000, 13.500000}, {-3.000000, 13.500000}, {0.000000, 13.000000}, {3.000000, 12.500000}, {5.000000, 11.500000}, {5.500000, 10.500000}, {4.500000, 9.000000}, {2.000000, 8.000000}, {-1.500000, 7.000000}, {-4.500000, 6.500000}, {-5.500000, 6.000000}, {-5.000000, 5.000000}, {-3.000000, 4.500000}, {0.000000, 4.500000}, {3.000000, 4.000000}, {4.500000, 3.500000}, {5.000000, 3.000000}, {4.500000, 3.000000}, {2.000000, 2.500000}, {0.000000, 2.500000}, {-2.000000, 2.500000}, {-3.000000, 2.500000}, {-2.500000, 2.500000} }
	};
	const std::array<double, WEAPON_MAX> weapon_delays = {
		/* None */		0.0,
		/* AK-47 */		133.333333333,
		/* LR-300 */	120.0,
		/* M249 */		130.0,
		/* MP5 */		100.0,
		/* Thompson */	130.0,
		/* Custom */	100.0
	};
	const std::array<double, WEAPON_MAX> crouch_delays = {
		0.0,
		0.0,
		0.0,
		130.0,
		0.0,
		0.0,
		0.0
	};
	const std::vector<std::vector<double>> control_times = {
		{ 0 },
		{ {121.961516,92.633356,138.606013,113.378747,66.251547,66.295303,75.932732,85.055126,89.202546,86.680192,78.821495,70.045184,60.859441,59.516501,71.667596,86.740825,98.336474,104.341144,104.093287,97.587559,85.480627,70.488800,56.564358,47.386454,56.637950,91.593555,112.386841,111.393398,87.506773} },
		{ {50.486527,63.409837,69.352231,69.019084,60.342866,50.279209,40.623896,30.626204,31.369554,40.533283,43.897277,38.230196,23.219868,9.947927,14.267603,20.268351,25.386810,29.051251,31.098685,31.469423,30.140472,27.108869,22.395387,16.088652,8.546886,27.487437,61.533720,72.697228,60.831257} },
		{ {151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000,151.250000} },
		{ {43.441909,58.669031,66.411108,71.385438,83.641583,79.398875,52.763157,21.615756,40.987258,52.649410,42.242421,15.073904,19.158961,29.407978,34.560589,33.587834,26.295315,12.769285,4.207833,10.980831,17.070518,21.744130,24.917849,26.570261,26.693439,25.283775,22.339392,17.859221,11.842602} },
		{ {86.598887,78.347140,74.408310,73.568908,69.211680,57.652541,44.231602,43.172386,46.847870,43.832255,32.594771,25.597451,32.269315,36.054248,31.898137,20.621504,16.997289,22.046002,22.690070} },
		{ {72.942129,66.636213,62.974812,61.750535,60.935042,57.200952,49.164097,38.976078,35.006739,37.638419,39.187920,36.075356,28.171659,21.234233,24.358432,28.987042,29.927871,26.070199,18.119360,13.438176,16.856357,19.274035,18.320346} },
	};
	const std::array<double, SCOPE_MAX> scope_multipliers = {
		1.0, //Silencer and muzzle brake stay the same
		0.8, //Silencer and muzzle brake stay the same
		1.2, //Silencer and muzzle brake stay the same
		3.83721, //Silencer becomes 1.0, Muzzle brake becomes 0.8
		7.65116 //Silencer becomes 1.0, Muzzle brake becomes 0.8
	};
	const std::array<std::pair<double,double>, BARREL_MAX> barrel_multipliers = {
		std::pair(1, 1),
		std::pair(0.8, 1),
		std::pair(1, 0.9),
		std::pair(0.5, 1),
	};
}

void recoil::init()
{
	static bool initialized = false;

	if (initialized)
	{
		return;
	}

	for (int weapon_index = 1; weapon_index < WEAPON_MAX; weapon_index++)
	{
		weapon_info* weapon = &weapons[weapon_index];
		weapon->recoil_table = recoil_tables[weapon_index];
		weapon->delay = weapon_delays[weapon_index];
		weapon->crouch_delay = crouch_delays[weapon_index];

		if (control_times.size() > weapon_index)
			weapon->control_time = control_times[weapon_index];
	}
	initialized = true;
}

void humanize(double &Recoil_x, double&Recoil_y)
{
	double range = 10.0 * g_settings.humanize;

	Recoil_x = utils::random_float(Recoil_x - range, Recoil_x + range);
	Recoil_y = utils::random_float(Recoil_y - range, Recoil_y + range);
}

int recoil::get_game_values() {
	LONG   lResult;
	HKEY   hKey;
	LPBYTE folder = new BYTE[MAX_PATH];
	DWORD  dwSize = sizeof(folder);
	char   value[64];
	DWORD  value_length = 64;
	DWORD  dwType = REG_SZ;

	lResult = RegOpenKey(HKEY_LOCAL_MACHINE,
		TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\"
			"Uninstall\\Steam App 252490"),
		&hKey);

	RegQueryValueExA(hKey, std::string(skCrypt("InstallLocation")).c_str(), NULL, &dwType,
		(LPBYTE)&value, &value_length);

	RegCloseKey(hKey);

	std::ifstream file;
	std::string line;
	int fileLine = 0;

	try {
		std::string client(skCrypt("\\cfg\\client.cfg"));

		file.open(std::string(value) + client); // path to rust

		while (std::getline(file, line))
		{
			fileLine++;

			if (line.find("graphics.fov") != std::string::npos) { // makes sure we read the correct line of the .cfg file
				std::string fovString = line;
				size_t pos = fovString.find(" ");

				fovString = fovString.substr(pos + 2);
				std::stringstream fovConv(fovString);

				fovConv >> g_settings.fov;
			}
			else if (line.find("input.sensitivity") != std::string::npos) { // makes sure we read the correct line of the .cfg file

				std::string sensString = line;
				size_t pos = sensString.find(" ");

				sensString = sensString.substr(pos + 2);
				std::stringstream sensConv(sensString);

				sensConv >> g_settings.sensitivity;
			}
		}
	}
	catch (const std::exception e) {
		utils::echo_error(e, 0x01);
	}
	file.close();
	return 0;
}

bool findvar(std::string line, std::string var, size_t start, size_t end, std::string&string_out)
{
	if (line.find(var) != std::string::npos) {

		std::string string = line;

		string_out = string.substr(start, end);
		return true;
	}
	return false;
}

int recoil::get_keybinds() {
	LONG   lResult;
	HKEY   hKey;
	LPBYTE folder = new BYTE[MAX_PATH];
	DWORD  dwSize = sizeof(folder);
	char   value[64];
	DWORD  value_length = 64;
	DWORD  dwType = REG_SZ;

	lResult = RegOpenKey(HKEY_LOCAL_MACHINE,
		TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\"
			"Uninstall\\Steam App 252490"),
		&hKey);

	RegQueryValueExA(hKey, std::string(skCrypt("InstallLocation")).c_str(), NULL, &dwType,
		(LPBYTE)&value, &value_length);

	RegCloseKey(hKey);

	std::ifstream file;
	std::string line;
	int fileLine = 0;

	std::string client(skCrypt("\\cfg\\keys.cfg"));

	file.open(std::string(value) + client); // path to rust

	try {
		while (std::getline(file, line))
		{
			fileLine++;

			std::string attack, scope, crouch;

			if (findvar(line, "+attack\"", line.find(" ") + 1, line.substr(line.find(" ") + 1).find(" "), attack))
				g_settings.attack_key = utils::rust_key_to_virtual_key(attack.c_str());

			else if (findvar(line, "+attack2\"", line.find(" ") + 1, line.substr(line.find(" ") + 1).find(" "), scope))
				g_settings.scope_key = utils::rust_key_to_virtual_key(scope.c_str());

			else if (findvar(line, "+duck\"", line.find(" ") + 1, line.substr(line.find(" ") + 1).find(" "), crouch))
				g_settings.crouch_key = utils::rust_key_to_virtual_key(crouch.c_str());
		}
	}
	catch (std::exception e) {
		utils::echo_error(e, 0x02);
	}
	file.close();
	return 0;
}

bool recoil::recoil_checks(bool& crouched)
{
	bool is_firing = utils::key_down(g_settings.attack_key),
		is_scoped = utils::key_down(g_settings.scope_key),
		is_crouched = utils::key_down(g_settings.crouch_key),
		should_recoil = g_settings.enabled && is_firing && is_scoped;

	crouched = is_crouched;
	return should_recoil;
}

void recoil::recoil()
{
	if (g_settings.weapon == WEAPON_NONE)
		return;

	get_game_values();
	get_keybinds();
	init();

	try
	{
		weapon_info current_weapon = weapons[g_settings.weapon];
		float current_scope_multiplier = scope_multipliers[g_settings.scope];
		std::pair<double, double> current_barrel_multiplier = barrel_multipliers[g_settings.barrel];
		
		for (int bullets_shot = 0; bullets_shot < current_weapon.recoil_table.size(); bullets_shot++)
		{
			bool is_crouched = false;

			if (!recoil_checks(is_crouched))
				break;

			double Fov_multiplier = (double)g_settings.fov / 90.0;
			double barrel_scaling = current_barrel_multiplier.first;

			if (g_settings.scope == SCOPE_8X || g_settings.scope == SCOPE_16X)
			{
				if (g_settings.barrel == BARREL_SILENCER)
				{
					barrel_scaling = 1;
				}
				else if (g_settings.barrel == BARREL_MUZZLE_BRAKE)
				{
					barrel_scaling = 0.8;
				}
			}

			double table_x = current_weapon.recoil_table[bullets_shot][0];
			double table_y = current_weapon.recoil_table[bullets_shot][1];

			table_x *= current_scope_multiplier * (barrel_scaling);
			table_y *= current_scope_multiplier * (barrel_scaling);

			double recoil_x = table_x / (Fov_multiplier * g_settings.sensitivity);
			double recoil_y = table_y / (Fov_multiplier * g_settings.sensitivity);

			humanize(recoil_x, recoil_y);

			double weapon_delay = is_crouched ? current_weapon.crouch_delay + current_weapon.delay : current_weapon.delay;
			weapon_delay *= current_barrel_multiplier.second;

			double control_time = g_settings.weapon == WEAPON_M249 || g_settings.weapon == WEAPON_THOMPSON || g_settings.weapon == WEAPON_CUSTOM ? weapon_delay : current_weapon.control_time.at(bullets_shot);

			if (g_settings.humanize > 0)
			{
				utils::relative_mouse_move(weapon_delay, weapon_delay, recoil_x, recoil_y);
			}
			else
			{
				utils::relative_mouse_move(weapon_delay, control_time, recoil_x, recoil_y);
			}
		}
	}
	catch (std::exception e) {
		utils::echo_error(e, 0x03);
	}
}