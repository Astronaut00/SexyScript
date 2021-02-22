#include <array>
#include <vector>
#include <string>

namespace recoil
{
	enum Weapons
	{
		WEAPON_NONE = 0,
		WEAPON_AK,
		WEAPON_LR,
		WEAPON_M249,
		WEAPON_MP5A4,
		WEAPON_THOMPSON,
		WEAPON_CUSTOM,
		WEAPON_MAX
	};

	enum Scopes
	{
		SCOPE_NONE,
		SCOPE_HANDMADE,
		SCOPE_HOLOSIGHT,
		SCOPE_8X,
		SCOPE_16X,
	    SCOPE_MAX
	};

	enum Barrel
	{
		BARREL_NONE = 0,
		BARREL_SILENCER,
		BARREL_MUZZLE_BOOST,
		BARREL_MUZZLE_BRAKE,
		BARREL_MAX
	};

	struct weapon_info
	{
		float delay;
		float crouch_delay;
		std::string name;
		std::vector<std::array<double, 2>> recoil_table;
		std::vector<double> control_time;
	};

	extern std::array<weapon_info, WEAPON_MAX> weapons;
	extern void recoil();
}