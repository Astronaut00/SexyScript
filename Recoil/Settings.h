#pragma once
#include <vector>
typedef int virtual_key;

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
	SCOPE_NONE = 0,
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

struct settings {
	//Menu
	bool enabled = false;

	//Recoil settings
	int weapon = WEAPON_NONE;
	int scope  = SCOPE_NONE;
	int barrel = BARREL_NONE;

	float sensitivity = 0.0f;
	int fov = 0;
	float humanize = 0;

	virtual_key attack_key = 0x1;
	virtual_key scope_key = 0x2;
	virtual_key crouch_key = 0x11;

	//Keybind settings
	virtual_key enable_hotkey = 0x00; //None
	virtual_key weapon_cycler = 0x00; //None
	virtual_key scope_cycler  = 0x00; //None
	virtual_key barrel_cycler = 0x00; //None
};
extern settings g_settings;