/*
 *  sscanf 2.11.3
 *
 *  Version: MPL 1.1
 *
 *  The contents of this file are subject to the Mozilla Public License Version
 *  1.1 (the "License"); you may not use this file except in compliance with
 *  the License. You may obtain a copy of the License at
 *  http://www.mozilla.org/MPL/
 *
 *  Software distributed under the License is distributed on an "AS IS" basis,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 *  for the specific language governing rights and limitations under the
 *  License.
 *
 *  The Original Code is the sscanf 2.0 SA:MP plugin.
 *
 *  The Initial Developer of the Original Code is Alex "Y_Less" Cole.
 *  Portions created by the Initial Developer are Copyright (C) 2020
 *  the Initial Developer. All Rights Reserved.
 *
 *  Contributor(s):
 *
 *      Cheaterman
 *      DEntisT
 *      Emmet_
 *      karimcambridge
 *      kalacsparty
 *      Kirima
 *      leHeix
 *      maddinat0r
 *      Southclaws
 *      Y_Less
 *      ziggi
 *
 *  Special Thanks to:
 *
 *      SA:MP Team past, present, and future.
 *      maddinat0r, for hosting the repo for a very long time.
 *      Emmet_, for his efforts in maintaining it for almost a year.
 */

#if !defined _INC_SSCANF
	#define _INC_SSCANF
#else
	#endinput
#endif

#if !defined _samp_included
	#error Please include <a_npc> or <a_samp> first.
#endif

#if defined GetDistanceFromMeToPoint
	// NPC script.
	#define SSCANF_NPC (1)
	#pragma library sscanf
#else
	#define SSCANF_NPC (0)
#endif

#if !defined __PawnBuild && !defined SSCANF_NO_NICE_FEATURES
	#error sscanf likes the community compiler.  Use `#define SSCANF_NO_NICE_FEATURES` to ignore this.
#endif

#define SSCANF_VERSION_MAJOR  2
#define SSCANF_VERSION_MINOR 11
#define SSCANF_VERSION_BUILD  3

#define SSCANF_VERSION_STRING #SSCANF_VERSION_MAJOR "." #SSCANF_VERSION_MINOR "." #SSCANF_VERSION_BUILD

// Example:
//
//         2.10.3
//     => 02 10 03
//     => 0x021003
//
const SSCANF_VERSION =
	((SSCANF_VERSION_MAJOR / 10) << 20) |
	((SSCANF_VERSION_MAJOR % 10) << 16) |
	((SSCANF_VERSION_MINOR / 10) << 12) |
	((SSCANF_VERSION_MINOR % 10) <<  8) |
	((SSCANF_VERSION_BUILD / 10) <<  4) |
	((SSCANF_VERSION_BUILD % 10) <<  0) ;

stock const SSCANF_VERSION_BCD = SSCANF_VERSION;

#define sscanf_%0\32; sscanf_
#define SSCANF:%0(%1) forward sscanf_%0(%1);public sscanf_%0(%1)

#if defined sscanf
	#error sscanf (possibly the PAWN version) already defined.
#endif

native SSCANF__(const file[], const line, const data[], const format[], {Float, _}:...);
native UNFORMAT__(const file[], const line, const data[], const format[], {Float, _}:...) = SSCANF__;
native SSCANF_Init(players, invalid, len);
native SSCANF_Join(playerid, const name[], npc);
native SSCANF_Leave(playerid);
native SSCANF_IsConnected(playerid);

native SSCANF_Levenshtein(const string1[], const string2[]);

// Get a global SSCANF setting/option.
native SSCANF_GetOption(const name[]) = SSCANF_Option;
native SSCANF_GetOption__(const name[]) = SSCANF_Option;

// Set a global SSCANF setting/option.
native SSCANF_SetOption(const name[], value) = SSCANF_Option;
native SSCANF_SetOption__(const name[], value) = SSCANF_Option;

// Backwards-compatibility with `SSCANF_Option`.  Usage:
//
//     SSCANF_Option(SSCANF_QUIET, 1);
//     new quiet = SSCANF_Option(SSCANF_QUIET);
//
// This can't just use a default parameter because a user may want to set the
// option to whatever that parameter is.
#define SSCANF_Option SSCANF_GetOption__
#define SSCANF_GetOption__(%0,%1) SSCANF_SetOption__(%0,%1)
#define _ALS_SSCANF_Option
#define _ALS_SSCANF_GetOption__

// Get the sscanf plugin version.
native SSCANF_VersionString(version[], size = sizeof (version)) = SSCANF_Version;
native SSCANF_VersionString__(version[], size = sizeof (version)) = SSCANF_Version;

native SSCANF_VersionBCD() = SSCANF_Version;
native SSCANF_VersionBCD__() = SSCANF_Version;

// The `SSCANF_Version` function is overloaded to take 0 or 2 parameters (or 1 plus a default).
#define SSCANF_Version SSCANF_VersionString__
#define SSCANF_VersionString__() SSCANF_VersionBCD__()
#define _ALS_SSCANF_Version
#define _ALS_SSCANF_VersionString__

#if defined __PawnBuild
	#define sscanf( SSCANF__(__file,__line,
	#define unformat( SSCANF__(__file,__line,
#else
	#define sscanf( SSCANF__(SSCANF_UNK_,-1,
	#define unformat( SSCANF__(SSCANF_FOM_,-1,
#endif

stock const
	SSCANF_UNK_[] = "sscanf",   // Append "unknown file" in the plugin when line number < 0.
	SSCANF_FOM_[] = "unformat", // Append "unknown file" in the plugin when line number < 0.
	SSCANF_EXT_[] = "extract",  // Append "unknown file" in the plugin when line number < 0.
	SSCANF_QUIET[] = "SSCANF_QUIET",
	OLD_DEFAULT_NAME[] = "OLD_DEFAULT_NAME",
	MATCH_NAME_PARTIAL[] = "MATCH_NAME_PARTIAL",
	CELLMIN_ON_MATCHES[] = "CELLMIN_ON_MATCHES",
	OLD_DEFAULT_KUSTOM[] = "OLD_DEFAULT_KUSTOM",
	OLD_DEFAULT_CUSTOM[] = "OLD_DEFAULT_CUSTOM",
	SSCANF_COLOUR_FORMS[] = "SSCANF_COLOUR_FORMS",
	SSCANF_ALPHA[] = "SSCANF_ALPHA",
	SSCANF_ARGB[] = "SSCANF_ARGB";

static stock
	bool:SSCANF_gInit = false;

// Pre-hook data.
#if !defined CHAIN_ORDER
	#define CHAIN_ORDER() 0
#endif

#define CHAIN_HOOK(%0) forward @CO_%0();public @CO_%0(){return CHAIN_ORDER()+1;}
#define CHAIN_NEXT(%0) @CO_%0

#define CHAIN_FORWARD:%0_%2(%1)=%3; \
	forward %0_%2(%1); \
	public %0_%2(%1) <_ALS : _ALS_x0, _ALS : _ALS_x1> { return (%3); } \
	public %0_%2(%1) <> { return (%3); }

#define CHAIN_PUBLIC:%0(%1) %0(%1) <_ALS : _ALS_go>

CHAIN_HOOK(SSCANF)
#undef CHAIN_ORDER
#define CHAIN_ORDER CHAIN_NEXT(SSCANF)

static stock _SSCANF_IncludeStates() <_ALS : _ALS_x0, _ALS : _ALS_x1, _ALS : _ALS_x2, _ALS : _ALS_x3> {}
static stock _SSCANF_IncludeStates() <_ALS : _ALS_go> {}

#if SSCANF_NPC
	forward SSCANF_PlayerCheck();

	/*
	  OnNPCModeInit

	  Called when the script starts if it is a NPC mode, sets up the system,
	  then calls the "real" OnNPCModeInit (using the new ALS 2 hook method).
	*/

	native SSCANF_SetTimer(const funcname[], interval, bool:repeating) = SetTimer;

	public OnNPCModeInit()
	{
		state _ALS : _ALS_go;
		SSCANF_Init(MAX_PLAYERS, INVALID_PLAYER_ID, MAX_PLAYER_NAME);
		#if !defined SSCANF_NO_PLAYERS
			// Initialise the system.
			SSCANF_PlayerCheck();
			// Will be run once per frame.
			SSCANF_SetTimer("SSCANF_PlayerCheck", 0, true);
		#endif
		SSCANF_OnNPCModeInit();
		return 1;
	}

	#if defined _ALS_OnNPCModeInit
		#undef OnNPCModeInit
	#else
		#define _ALS_OnNPCModeInit
	#endif
	#define OnNPCModeInit(%0) CHAIN_PUBLIC:SSCANF_OnNPCModeInit(%0)
	CHAIN_FORWARD:SSCANF_OnNPCModeInit() = 1;

	/*
	  SSCANF_PlayerCheck

	  NPC modes have no "OnPlayerConnect callback, so we need to simulate one.
	*/

	#if !defined SSCANF_NO_PLAYERS
		public SSCANF_PlayerCheck()
		{
			new
				name[MAX_PLAYER_NAME + 1];
			for (new i = 0; i != MAX_PLAYERS; ++i)
			{
				if (IsPlayerConnected(i))
				{
					if (!SSCANF_IsConnected(i))
					{
						GetPlayerName(i, name, sizeof (name));
						// We have no way to know if they are an NPC or not!
						SSCANF_Join(i, name, 0);
					}
				}
				else
				{
					if (SSCANF_IsConnected(i))
					{
						SSCANF_Leave(i);
					}
				}
			}
		}
	#endif
#else
	static stock SSCANF_RunInit()
	{
		state _ALS : _ALS_go;
		new
			name[MAX_PLAYER_NAME + 1];

		SSCANF_Init(GetMaxPlayers(), INVALID_PLAYER_ID, MAX_PLAYER_NAME);
		SSCANF_gInit = true;

		// Check if there are any players that aren't initialized.
		for (new i = 0; i < MAX_PLAYERS; i ++)
		{
			if (IsPlayerConnected(i) && !SSCANF_IsConnected(i))
			{
				GetPlayerName(i, name, sizeof (name));
				SSCANF_Join(i, name, IsPlayerNPC(i));
			}
		}
	}

	/*
	  OnScriptInit

	  Called for earlier initialisation by YSI.
	*/

	#define SSCANF_BRACKETS ()
	forward OnScriptInit SSCANF_BRACKETS;
	#undef SSCANF_BRACKETS

	public OnScriptInit()
	{
		if (!SSCANF_gInit)
		{
			SSCANF_RunInit();
		}
		SSCANF_OnScriptInit();
		return 1;
	}

	#if defined _ALS_OnScriptInit
		#undef OnScriptInit
	#else
		#define _ALS_OnScriptInit
	#endif
	#define OnScriptInit(%0) CHAIN_PUBLIC:SSCANF_OnScriptInit(%0)
	CHAIN_FORWARD:SSCANF_OnScriptInit() = 1;

	/*
	  OnFilterScriptInit

	  Called when the script starts if it is a filterscript, sets up the system,
	  then calls the "real" OnFilterScriptInit (using the new ALS 2 hook
	  method).
	*/

	public OnFilterScriptInit()
	{
		if (!SSCANF_gInit)
		{
			SSCANF_RunInit();
		}
		SSCANF_OnFilterScriptInit();
		return 1;
	}

	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif
	#define OnFilterScriptInit(%0) CHAIN_PUBLIC:SSCANF_OnFilterScriptInit(%0)
	CHAIN_FORWARD:SSCANF_OnFilterScriptInit() = 1;

	/*
	  OnGameModeInit

	  Called when the script starts if it is a gamemode.  This callback is also
	  called in filterscripts so we don't want to reinitialise the system in
	  that case.
	*/

	public OnGameModeInit()
	{
		if (!SSCANF_gInit)
		{
			SSCANF_RunInit();
		}
		SSCANF_OnGameModeInit();
		return 1;
	}

	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit(%0) CHAIN_PUBLIC:SSCANF_OnGameModeInit(%0)
	CHAIN_FORWARD:SSCANF_OnGameModeInit() = 1;

	/*
	  OnCachedInit

	  Called when the script starts if it is YSI cached mode.
	*/

	forward OnCachedInit();
	public OnCachedInit()
	{
		SSCANF_RunInit();
		SSCANF_OnCachedInit();
		return 1;
	}

	#if defined _ALS_OnCachedInit
		#undef OnCachedInit
	#else
		#define _ALS_OnCachedInit
	#endif
	#define OnCachedInit(%0) CHAIN_PUBLIC:SSCANF_OnCachedInit(%0)
	CHAIN_FORWARD:SSCANF_OnCachedInit() = 1;

	/*
	  OnPlayerConnect

	  Called when a player connects.  Actually increments an internal count so
	  that if a script ends and "OnPlayerDisconnect" is called then "sscanf"
	  still knows that the player is really connected.  Also stores their name
	  internally.
	*/

	public OnPlayerConnect(playerid)
	{
		new
			name[MAX_PLAYER_NAME + 1];
		GetPlayerName(playerid, name, sizeof(name));
		SSCANF_Join(playerid, name, IsPlayerNPC(playerid));
		SSCANF_OnPlayerConnect(playerid);
		return 1;
	}

	#if defined _ALS_OnPlayerConnect
		#undef OnPlayerConnect
	#else
		#define _ALS_OnPlayerConnect
	#endif
	#define OnPlayerConnect(%0) CHAIN_PUBLIC:SSCANF_OnPlayerConnect(%0)
	CHAIN_FORWARD:SSCANF_OnPlayerConnect(playerid) = 1;

	/*
	  OnPlayerDisconnect

	  Called when a player disconnects, or when a script is ended.
	*/

	public OnPlayerDisconnect(playerid, reason)
	{
		SSCANF_OnPlayerDisconnect(playerid, reason);
		SSCANF_Leave(playerid);
		return 1;
	}

	#if defined _ALS_OnPlayerDisconnect
		#undef OnPlayerDisconnect
	#else
		#define _ALS_OnPlayerDisconnect
	#endif
	#define OnPlayerDisconnect(%0) CHAIN_PUBLIC:SSCANF_OnPlayerDisconnect(%0)
	CHAIN_FORWARD:SSCANF_OnPlayerDisconnect(playerid, reason) = 1;
#endif

#define SSCANF_Init
#define SSCANF_Join
#define SSCANF_Leave
#define SSCANF_IsConnected

#if defined __PawnBuild
	#define extract%0->%1; EXTRN%1;UNFORMAT__(__file,__line,_:EXTRV:EXTRX:%0,""#,%1,,);
#else
	#define extract%0->%1; EXTRN%1;UNFORMAT__(SSCANF_EXT_,-1,_:EXTRV:EXTRX:%0,""#,%1,,);
#endif

#define UNFORMAT__(%7,%8,_:EXTRV:EXTRX:%0,""#,%1);%2else if (UNFORMAT__(%7,%8,_:EXTRV:EXTRX:%0,""#,%1))

#define EXTRV:EXTRX:%0<%3>%4#,%9new%1,%2) EXTRZ:EXTRY:%0%4#P<%3>,|||%1|||%2)
#define EXTRZ:EXTRY:%0#P<,> EXTRY:%0"P<,>"#
#define EXTRX:%0#,%9new%1,%2) EXTRY:%0#,|||%1|||%2)
#define EXTRY: EXTR8:EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:

#define EXTR8:EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0#%1,%2|||%6:%3=%9|||%4) %6_EXTRO:%0#%1,%2|||%3=%9|||%4)
#define EXTR9:EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0#%1,%2|||%3=%9|||%4) __EXTRO:%0#%1,%2|||%3=%9|||%4)
#define EXTR0:EXTR1:EXTR2:EXTR3:EXTR4:%0#%1,%2|||%6:%3[%7]|||%4) %6_EXTRW:%0#%1,%2|||%3[%7]|||%4)
#define EXTR1:EXTR2:EXTR3:EXTR4:%0#%1,%2|||%3[%7]|||%4) __EXTRW:%0#%1,%2|||%3[%7]|||%4)
#define EXTR2:EXTR3:EXTR4:%0#%1,%2|||%6:%3|||%4) %6_EXTRN:%0#%1,%2|||%3|||%4)
#define EXTR3:EXTR4:%0#%1,,%2||||||%4) %0#%1,%2)
#define EXTR4:%0#%1,%2|||%3|||%4) __EXTRN:%0#%1,%2|||%3|||%4)

// Optional specifiers.
#define __EXTRO:%0#%1,%2|||%3=%9|||%4,%5) EXTRY:%0#%1I"("#%9")"#,%2,%3|||%4|||%5)
#define Float_EXTRO:%0#%1,%2|||%3=%9|||%4,%5) EXTRY:%0#%1F"("#%9")"#,%2,%3|||%4|||%5)
#define player_EXTRO:%0#%1,%2|||%3=%9|||%4,%5) EXTRY:%0#%1U"("#%9")"#,%2,%3|||%4|||%5)
#define string_EXTRO:%0#%1,%2|||%3[%7]=%9|||%4,%5) EXTRY:%0#%1S"("#%9")[*]",%2,(%7),%3|||%4|||%5)

// Normal specifiers (the double underscore is to work for "_:".
#define __EXTRN:%0#%1,%2|||%3|||%4,%5) EXTRY:%0#%1i,%2,%3|||%4|||%5)
#define Float_EXTRN:%0#%1,%2|||%3|||%4,%5) EXTRY:%0#%1f,%2,%3|||%4|||%5)
#define player_EXTRN:%0#%1,%2|||%3|||%4,%5) EXTRY:%0#%1u,%2,%3|||%4|||%5)
//#define string_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1s[%7],%2,%3|||%4|||%5)

// Array versions of normal specifiers.
#define __EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1a<i>[*],%2,(%7),%3|||%4|||%5)
#define Float_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1a<f>[*],%2,(%7),%3|||%4|||%5)
#define player_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1a<u>[*],%2,(%7),%3|||%4|||%5)
#define string_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1s[*],%2,(%7),%3|||%4|||%5)

// Get rid of excess leading space which causes warnings.
#define EXTRN%0new%1; new%1;

#if !defined string
    #define string:
#endif

#define player:%0;UNFORMAT__(%1) %0;UNFORMAT__(%1)

#define hex:%0;UNFORMAT__(%1) %0;UNFORMAT__(%1)
#define hex_EXTRO:%0#%1,%2|||%3=%9|||%4,%5) EXTRY:%0#%1H"("#%9")"#,%2,%3|||%4|||%5)
#define hex_EXTRN:%0#%1,%2|||%3|||%4,%5) EXTRY:%0#%1h,%2,%3|||%4|||%5)
#define hex_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1a<h>[*],%2,(%7),%3|||%4|||%5)

#define bin:%0;UNFORMAT__(%1) %0;UNFORMAT__(%1)
#define bin_EXTRO:%0#%1,%2|||%3=%9|||%4,%5) EXTRY:%0#%1B"("#%9")"#,%2,%3|||%4|||%5)
#define bin_EXTRN:%0#%1,%2|||%3|||%4,%5) EXTRY:%0#%1b,%2,%3|||%4|||%5)
#define bin_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1a<b>[*],%2,(%7),%3|||%4|||%5)

#define colour:%0;UNFORMAT__(%1) %0;UNFORMAT__(%1)
#define colour_EXTRO:%0#%1,%2|||%3=%9|||%4,%5) EXTRY:%0#%1M"("#%9")"#,%2,%3|||%4|||%5)
#define colour_EXTRN:%0#%1,%2|||%3|||%4,%5) EXTRY:%0#%1m,%2,%3|||%4|||%5)
#define colour_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1a<m>[*],%2,(%7),%3|||%4|||%5)

#define kustom:%0<%1> %0
#define kustom_EXTRO:%0#%1,%2|||%3<%8>=%9|||%4,%5) EXTRY:%0#%1K<%8>"("#%9")"#,%2,%3|||%4|||%5)
#define kustom_EXTRN:%0#%1,%2|||%3<%8>|||%4,%5) EXTRY:%0#%1k<%8>,%2,%3|||%4|||%5)
//#define bin_EXTRW:%0#%1,%2|||%3[%7]|||%4,%5) EXTRY:%0#%1a<b>[*],%2,(%7),%3|||%4|||%5)

stock SSCANF_GetClosestString(const input[], const candidates[][], threshold = cellmax, count = sizeof (candidates))
{
	new
		closest = cellmin,
		distance;
	while (count--)
	{
		distance = SSCANF_Levenshtein(input, candidates[count]);
		if (distance < threshold)
		{
			closest = count,
			threshold = distance;
		}
	}
	return closest;
}

stock SSCANF_GetClosestValue(const input[], const candidates[][], const results[], fail = cellmin, threshold = cellmax, count = sizeof (candidates), check = sizeof (results))
{
	assert(count == check);
	new closest = SSCANF_GetClosestString(input, candidates, threshold, count);
	if (closest == cellmin)
	{
		return fail;
	}
	return results[closest];
}

#if !defined SSCANF_NO_K_WEAPON
	SSCANF:weapon(const string[])
	{
		static const results[] = {
			0, 0, 1, 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 22, 23, 24, 25,
			26, 27, 28, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46
		};

		static const candidates[][] = {
			"Fists",
			"Unarmed",
			"Knuckles",
			"Knuckledusters",
			"Brass Knuckles",
			"Golf Club",
			"Night Stick",
			"Knife",
			"Baseball Bat",
			"Shovel",
			"Pool cue",
			"Katana",
			"Chainsaw",
			"Purple Dildo",
			"White Dildo",
			"Long White Dildo",
			"White Dildo 2",
			"Flowers",
			"Cane",
			"Grenades",
			"Tear Gas",
			"Molotovs",
			"Pistol",
			"Silenced Pistol",
			"Desert Eagle",
			"Shotgun",
			"Sawn Off Shotgun",
			"Combat Shotgun",
			"Micro Uzi",
			"Mac 10",
			"MP5",
			"AK47",
			"M4",
			"Tec9",
			"Rifle",
			"Sniper Rifle",
			"RPG",
			"Missile Launcher",
			"Flame Thrower",
			"Minigun",
			"Sachel Charges",
			"Detonator",
			"Spray Paint",
			"Fire Extinguisher",
			"Camera",
			"Nightvision Goggles",
			"Thermal Goggles",
			"Parachute"
		};
		
		// This function is VERY basic, needs VASTLY improving to detect variations.
		if ('0' <= string[0] <= '9')
		{
			new
				ret = strval(string);
			if (0 <= ret <= 18 || 22 <= ret <= 46)
			{
				return ret;
			}
		}
		else
		{
			return SSCANF_GetClosestValue(string, candidates, results, -1);
		}
		return -1;
	}
#endif

#if !defined SSCANF_NO_K_VEHICLE
	SSCANF:vehicle(const string[])
	{
		static const results[] = {
			400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417,
			418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435,
			436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453,
			454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471,
			472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489,
			490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507,
			508, 509, 510, 511, 512, 513, 514, 515, 516, 517, 518, 519, 520, 521, 522, 523, 524, 525,
			526, 527, 528, 529, 530, 531, 532, 533, 534, 535, 536, 537, 538, 539, 540, 541, 542, 543,
			544, 545, 546, 547, 548, 549, 550, 551, 552, 553, 554, 555, 556, 557, 558, 559, 560, 561,
			562, 563, 564, 565, 566, 567, 568, 569, 570, 571, 572, 573, 574, 575, 576, 577, 578, 579,
			580, 581, 582, 583, 584, 585, 586, 587, 588, 589, 590, 591, 592, 593, 594, 595, 596, 597,
			598, 599, 600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611
		};

		static const candidates[][] = {
			"Landstalker",
			"Bravura",
			"Buffalo",
			"Linerunner",
			"Perenniel",
			"Sentinel",
			"Dumper",
			"Firetruck",
			"Trashmaster",
			"Stretch",
			"Manana",
			"Infernus",
			"Voodoo",
			"Pony",
			"Mule",
			"Cheetah",
			"Ambulance",
			"Leviathan",
			"Moonbeam",
			"Esperanto",
			"Taxi",
			"Washington",
			"Bobcat",
			"Mr Whoopee",
			"BF Injection",
			"Hunter",
			"Premier",
			"Enforcer",
			"Securicar",
			"Banshee",
			"Predator",
			"Bus",
			"Rhino",
			"Barracks",
			"Hotknife",
			"Article Trailer",
			"Previon",
			"Coach",
			"Cabbie",
			"Stallion",
			"Rumpo",
			"RC Bandit",
			"Romero",
			"Packer",
			"Monster",
			"Admiral",
			"Squallo",
			"Seasparrow",
			"Pizzaboy",
			"Tram",
			"Article Trailer 2",
			"Turismo",
			"Speeder",
			"Reefer",
			"Tropic",
			"Flatbed",
			"Yankee",
			"Caddy",
			"Solair",
			"Berkley's RC Van",
			"Skimmer",
			"PCJ-600",
			"Faggio",
			"Freeway",
			"RC Baron",
			"RC Raider",
			"Glendale",
			"Oceanic",
			"Sanchez",
			"Sparrow",
			"Patriot",
			"Quad",
			"Coastguard",
			"Dinghy",
			"Hermes",
			"Sabre",
			"Rustler",
			"ZR-350",
			"Walton",
			"Regina",
			"Comet",
			"BMX",
			"Burrito",
			"Camper",
			"Marquis",
			"Baggage",
			"Dozer",
			"Maverick",
			"SAN News Maverick",
			"Rancher",
			"FBI Rancher",
			"Virgo",
			"Greenwood",
			"Jetmax",
			"Hotring Racer",
			"Sandking",
			"Blista Compact",
			"Police Maverick",
			"Boxville",
			"Benson",
			"Mesa",
			"RC Goblin",
			"Hotring Racer",
			"Hotring Racer",
			"Bloodring Banger",
			"Rancher",
			"Super GT",
			"Elegant",
			"Journey",
			"Bike",
			"Mountain Bike",
			"Beagle",
			"Cropduster",
			"Stuntplane",
			"Tanker",
			"Roadtrain",
			"Nebula",
			"Majestic",
			"Buccaneer",
			"Shamal",
			"Hydra",
			"FCR-900",
			"NRG-500",
			"HPV1000",
			"Cement Truck",
			"Towtruck",
			"Fortune",
			"Cadrona",
			"FBI Truck",
			"Willard",
			"Forklift",
			"Tractor",
			"Combine Harvester",
			"Feltzer",
			"Remington",
			"Slamvan",
			"Blade",
			"Freight (Train)",
			"Brownstreak (Train)",
			"Vortex",
			"Vincent",
			"Bullet",
			"Clover",
			"Sadler",
			"Firetruck LA",
			"Hustler",
			"Intruder",
			"Primo",
			"Cargobob",
			"Tampa",
			"Sunrise",
			"Merit",
			"Utility Van",
			"Nevada",
			"Yosemite",
			"Windsor",
			"Monster \"A\"",
			"Monster \"B\"",
			"Uranus",
			"Jester",
			"Sultan",
			"Stratum",
			"Elegy",
			"Raindance",
			"RC Tiger",
			"Flash",
			"Tahoma",
			"Savanna",
			"Bandito",
			"Freight Flat Trailer (Train)",
			"Streak Trailer (Train)",
			"Kart",
			"Mower",
			"Dune",
			"Sweeper",
			"Broadway",
			"Tornado",
			"AT400",
			"DFT-30",
			"Huntley",
			"Stafford",
			"BF-400",
			"Newsvan",
			"Tug",
			"Petrol Trailer",
			"Emperor",
			"Wayfarer",
			"Euros",
			"Hotdog",
			"Club",
			"Freight Box Trailer (Train)",
			"Article Trailer 3",
			"Andromada",
			"Dodo",
			"RC Cam",
			"Launch",
			"Police Car (LSPD)",
			"Police Car (SFPD)",
			"Police Car (LVPD)",
			"Police Ranger",
			"Picador",
			"S.W.A.T.",
			"Alpha",
			"Phoenix",
			"Glendale Shit",
			"Sadler Shit",
			"Baggage Trailer \"A\"",
			"Baggage Trailer \"B\"",
			"Tug Stairs Trailer",
			"Boxville",
			"Farm Trailer",
			"Utility Trailer"
		};

		// This function is VERY basic, needs VASTLY improving to detect variations.
		if ('0' <= string[0] <= '9')
		{
			new
				ret = strval(string);
			if (400 <= ret <= 611)
			{
				return ret;
			}
		}
		else
		{
			return SSCANF_GetClosestValue(string, candidates, results, -1);
		}
		return -1;
	}
#endif

#if !defined SSCANF_NO_K_SKIN
	SSCANF:skin(string[])
	{
		// This function is VERY basic, needs VASTLY improving to detect variations.
		if ('0' <= string[0] <= '9')
		{
			new
				ret = strval(string);
			if (0 <= ret <= 311)
			{
				return ret;
			}
		}
		else if (!strcmp(string, "cj", true)) return 0;
		else if (!strcmp(string, "truth", true)) return 1;
		else if (!strcmp(string, "maccer", true)) return 2;
		else if (!strcmp(string, "andre", true)) return 3;
		else if (!strcmp(string, "bbthin", true)) return 4;
		else if (!strcmp(string, "bb", true)) return 5;
		else if (!strcmp(string, "emmet", true)) return 6;
		else if (!strcmp(string, "male01", true)) return 7;
		else if (!strcmp(string, "janitor", true)) return 8;
		else if (!strcmp(string, "bfori", true)) return 9;
		else if (!strcmp(string, "bfost", true)) return 10;
		else if (!strcmp(string, "vbfycrp", true)) return 11;
		else if (!strcmp(string, "bfyri", true)) return 12;
		else if (!strcmp(string, "bfyst", true)) return 13;
		else if (!strcmp(string, "bmori", true)) return 14;
		else if (!strcmp(string, "bmost", true)) return 15;
		else if (!strcmp(string, "bmyap", true)) return 16;
		else if (!strcmp(string, "bmybu", true)) return 17;
		else if (!strcmp(string, "bmybe", true)) return 18;
		else if (!strcmp(string, "bmydj", true)) return 19;
		else if (!strcmp(string, "bmyri", true)) return 20;
		else if (!strcmp(string, "bmycr", true)) return 21;
		else if (!strcmp(string, "bmyst", true)) return 22;
		else if (!strcmp(string, "wmybmx", true)) return 23;
		else if (!strcmp(string, "wbdyg1", true)) return 24;
		else if (!strcmp(string, "wbdyg2", true)) return 25;
		else if (!strcmp(string, "wmybp", true)) return 26;
		else if (!strcmp(string, "wmycon", true)) return 27;
		else if (!strcmp(string, "bmydrug", true)) return 28;
		else if (!strcmp(string, "wmydrug", true)) return 29;
		else if (!strcmp(string, "hmydrug", true)) return 30;
		else if (!strcmp(string, "dwfolc", true)) return 31;
		else if (!strcmp(string, "dwmolc1", true)) return 32;
		else if (!strcmp(string, "dwmolc2", true)) return 33;
		else if (!strcmp(string, "dwmylc1", true)) return 34;
		else if (!strcmp(string, "hmogar", true)) return 35;
		else if (!strcmp(string, "wmygol1", true)) return 36;
		else if (!strcmp(string, "wmygol2", true)) return 37;
		else if (!strcmp(string, "hfori", true)) return 38;
		else if (!strcmp(string, "hfost", true)) return 39;
		else if (!strcmp(string, "hfyri", true)) return 40;
		else if (!strcmp(string, "hfyst", true)) return 41;
		else if (!strcmp(string, "jethro", true)) return 42;
		else if (!strcmp(string, "hmori", true)) return 43;
		else if (!strcmp(string, "hmost", true)) return 44;
		else if (!strcmp(string, "hmybe", true)) return 45;
		else if (!strcmp(string, "hmyri", true)) return 46;
		else if (!strcmp(string, "hmycr", true)) return 47;
		else if (!strcmp(string, "hmyst", true)) return 48;
		else if (!strcmp(string, "omokung", true)) return 49;
		else if (!strcmp(string, "wmymech", true)) return 50;
		else if (!strcmp(string, "bmymoun", true)) return 51;
		else if (!strcmp(string, "wmymoun", true)) return 52;
		else if (!strcmp(string, "ofori", true)) return 53;
		else if (!strcmp(string, "ofost", true)) return 54;
		else if (!strcmp(string, "ofyri", true)) return 55;
		else if (!strcmp(string, "ofyst", true)) return 56;
		else if (!strcmp(string, "omori", true)) return 57;
		else if (!strcmp(string, "omost", true)) return 58;
		else if (!strcmp(string, "omyri", true)) return 59;
		else if (!strcmp(string, "omyst", true)) return 60;
		else if (!strcmp(string, "wmyplt", true)) return 61;
		else if (!strcmp(string, "wmopj", true)) return 62;
		else if (!strcmp(string, "bfypro", true)) return 63;
		else if (!strcmp(string, "hfypro", true)) return 64;
		else if (!strcmp(string, "kendl", true)) return 65;
		else if (!strcmp(string, "bmypol1", true)) return 66;
		else if (!strcmp(string, "bmypol2", true)) return 67;
		else if (!strcmp(string, "wmoprea", true)) return 68;
		else if (!strcmp(string, "sbfyst", true)) return 69;
		else if (!strcmp(string, "wmosci", true)) return 70;
		else if (!strcmp(string, "wmysgrd", true)) return 71;
		else if (!strcmp(string, "swmyhp1", true)) return 72;
		else if (!strcmp(string, "swmihp2", true)) return 73;
		else if (!strcmp(string, "-", true)) return 74;
		else if (!strcmp(string, "swfopro", true)) return 75;
		else if (!strcmp(string, "wfystew", true)) return 76;
		else if (!strcmp(string, "swmotr1", true)) return 77;
		else if (!strcmp(string, "wmotr1", true)) return 78;
		else if (!strcmp(string, "bmotr1", true)) return 79;
		else if (!strcmp(string, "vbmybox", true)) return 80;
		else if (!strcmp(string, "vwmybox", true)) return 81;
		else if (!strcmp(string, "vhmyelv", true)) return 82;
		else if (!strcmp(string, "vbmyelv", true)) return 83;
		else if (!strcmp(string, "vimyelv", true)) return 84;
		else if (!strcmp(string, "vwfypro", true)) return 85;
		else if (!strcmp(string, "ryder3", true)) return 86;
		else if (!strcmp(string, "vwfyst1", true)) return 87;
		else if (!strcmp(string, "wfori", true)) return 88;
		else if (!strcmp(string, "wfost", true)) return 89;
		else if (!strcmp(string, "wfyjg", true)) return 90;
		else if (!strcmp(string, "wfyri", true)) return 91;
		else if (!strcmp(string, "wfyro", true)) return 92;
		else if (!strcmp(string, "wfyst", true)) return 93;
		else if (!strcmp(string, "wmori", true)) return 94;
		else if (!strcmp(string, "wmost", true)) return 95;
		else if (!strcmp(string, "wmyjg", true)) return 96;
		else if (!strcmp(string, "wmylg", true)) return 97;
		else if (!strcmp(string, "wmyri", true)) return 98;
		else if (!strcmp(string, "wmyro", true)) return 99;
		else if (!strcmp(string, "wmycr", true)) return 100;
		else if (!strcmp(string, "wmyst", true)) return 101;
		else if (!strcmp(string, "ballas1", true)) return 102;
		else if (!strcmp(string, "ballas2", true)) return 103;
		else if (!strcmp(string, "ballas3", true)) return 104;
		else if (!strcmp(string, "fam1", true)) return 105;
		else if (!strcmp(string, "fam2", true)) return 106;
		else if (!strcmp(string, "fam3", true)) return 107;
		else if (!strcmp(string, "lsv1", true)) return 108;
		else if (!strcmp(string, "lsv2", true)) return 109;
		else if (!strcmp(string, "lsv3", true)) return 110;
		else if (!strcmp(string, "maffa", true)) return 111;
		else if (!strcmp(string, "maffb", true)) return 112;
		else if (!strcmp(string, "mafboss", true)) return 113;
		else if (!strcmp(string, "vla1", true)) return 114;
		else if (!strcmp(string, "vla2", true)) return 115;
		else if (!strcmp(string, "vla3", true)) return 116;
		else if (!strcmp(string, "triada", true)) return 117;
		else if (!strcmp(string, "triadb", true)) return 118;
		else if (!strcmp(string, "sindaco", true)) return 119;
		else if (!strcmp(string, "triboss", true)) return 120;
		else if (!strcmp(string, "dnb1", true)) return 121;
		else if (!strcmp(string, "dnb2", true)) return 122;
		else if (!strcmp(string, "dnb3", true)) return 123;
		else if (!strcmp(string, "vmaff1", true)) return 124;
		else if (!strcmp(string, "vmaff2", true)) return 125;
		else if (!strcmp(string, "vmaff3", true)) return 126;
		else if (!strcmp(string, "vmaff4", true)) return 127;
		else if (!strcmp(string, "dnmylc", true)) return 128;
		else if (!strcmp(string, "dnfolc1", true)) return 129;
		else if (!strcmp(string, "dnfolc2", true)) return 130;
		else if (!strcmp(string, "dnfylc", true)) return 131;
		else if (!strcmp(string, "dnmolc1", true)) return 132;
		else if (!strcmp(string, "dnmolc2", true)) return 133;
		else if (!strcmp(string, "sbmotr2", true)) return 134;
		else if (!strcmp(string, "swmotr2", true)) return 135;
		else if (!strcmp(string, "sbmytr3", true)) return 136;
		else if (!strcmp(string, "swmotr3", true)) return 137;
		else if (!strcmp(string, "wfybe", true)) return 138;
		else if (!strcmp(string, "bfybe", true)) return 139;
		else if (!strcmp(string, "hfybe", true)) return 140;
		else if (!strcmp(string, "sofybu", true)) return 141;
		else if (!strcmp(string, "sbmyst", true)) return 142;
		else if (!strcmp(string, "sbmycr", true)) return 143;
		else if (!strcmp(string, "bmycg", true)) return 144;
		else if (!strcmp(string, "wfycrk", true)) return 145;
		else if (!strcmp(string, "hmycm", true)) return 146;
		else if (!strcmp(string, "wmybu", true)) return 147;
		else if (!strcmp(string, "bfybu", true)) return 148;
		else if (!strcmp(string, "smokev", true)) return 149;
		else if (!strcmp(string, "wfybu", true)) return 150;
		else if (!strcmp(string, "dwfylc1", true)) return 151;
		else if (!strcmp(string, "wfypro", true)) return 152;
		else if (!strcmp(string, "wmyconb", true)) return 153;
		else if (!strcmp(string, "wmybe", true)) return 154;
		else if (!strcmp(string, "wmypizz", true)) return 155;
		else if (!strcmp(string, "bmobar", true)) return 156;
		else if (!strcmp(string, "cwfyhb", true)) return 157;
		else if (!strcmp(string, "cwmofr", true)) return 158;
		else if (!strcmp(string, "cwmohb1", true)) return 159;
		else if (!strcmp(string, "cwmohb2", true)) return 160;
		else if (!strcmp(string, "cwmyhr", true)) return 161;
		else if (!strcmp(string, "cwmyhb1", true)) return 162;
		else if (!strcmp(string, "bmyboun", true)) return 163;
		else if (!strcmp(string, "wmyboun", true)) return 164;
		else if (!strcmp(string, "wmomib", true)) return 165;
		else if (!strcmp(string, "bmymib", true)) return 166;
		else if (!strcmp(string, "wmybell", true)) return 167;
		else if (!strcmp(string, "bmochil", true)) return 168;
		else if (!strcmp(string, "sofyri", true)) return 169;
		else if (!strcmp(string, "somyst", true)) return 170;
		else if (!strcmp(string, "vwmybjd", true)) return 171;
		else if (!strcmp(string, "vwfycrp", true)) return 172;
		else if (!strcmp(string, "sfr1", true)) return 173;
		else if (!strcmp(string, "sfr2", true)) return 174;
		else if (!strcmp(string, "sfr3", true)) return 175;
		else if (!strcmp(string, "bmybar", true)) return 176;
		else if (!strcmp(string, "wmybar", true)) return 177;
		else if (!strcmp(string, "wfysex", true)) return 178;
		else if (!strcmp(string, "wmyammo", true)) return 179;
		else if (!strcmp(string, "bmytatt", true)) return 180;
		else if (!strcmp(string, "vwmycr", true)) return 181;
		else if (!strcmp(string, "vbmocd", true)) return 182;
		else if (!strcmp(string, "vbmycr", true)) return 183;
		else if (!strcmp(string, "vhmycr", true)) return 184;
		else if (!strcmp(string, "sbmyri", true)) return 185;
		else if (!strcmp(string, "somyri", true)) return 186;
		else if (!strcmp(string, "somybu", true)) return 187;
		else if (!strcmp(string, "swmyst", true)) return 188;
		else if (!strcmp(string, "wmyva", true)) return 189;
		else if (!strcmp(string, "copgrl2", true)) return 190;
		else if (!strcmp(string, "gungrl3", true)) return 191;
		else if (!strcmp(string, "mecgrl3", true)) return 192;
		else if (!strcmp(string, "nurgrl3", true)) return 193;
		else if (!strcmp(string, "crogrl3", true)) return 194;
		else if (!strcmp(string, "gangrl3", true)) return 195;
		else if (!strcmp(string, "cwfofr", true)) return 196;
		else if (!strcmp(string, "cwfohb", true)) return 197;
		else if (!strcmp(string, "cwfyfr1", true)) return 198;
		else if (!strcmp(string, "cwfyfr2", true)) return 199;
		else if (!strcmp(string, "cwmyhb2", true)) return 200;
		else if (!strcmp(string, "dwfylc2", true)) return 201;
		else if (!strcmp(string, "dwmylc2", true)) return 202;
		else if (!strcmp(string, "omykara", true)) return 203;
		else if (!strcmp(string, "wmykara", true)) return 204;
		else if (!strcmp(string, "wfyburg", true)) return 205;
		else if (!strcmp(string, "vwmycd", true)) return 206;
		else if (!strcmp(string, "vhfypro", true)) return 207;
		else if (!strcmp(string, "suzie", true)) return 208;
		else if (!strcmp(string, "omonood", true)) return 209;
		else if (!strcmp(string, "omoboat", true)) return 210;
		else if (!strcmp(string, "wfyclot", true)) return 211;
		else if (!strcmp(string, "vwmotr1", true)) return 212;
		else if (!strcmp(string, "vwmotr2", true)) return 213;
		else if (!strcmp(string, "vwfywai", true)) return 214;
		else if (!strcmp(string, "sbfori", true)) return 215;
		else if (!strcmp(string, "swfyri", true)) return 216;
		else if (!strcmp(string, "wmyclot", true)) return 217;
		else if (!strcmp(string, "sbfost", true)) return 218;
		else if (!strcmp(string, "sbfyri", true)) return 219;
		else if (!strcmp(string, "sbmocd", true)) return 220;
		else if (!strcmp(string, "sbmori", true)) return 221;
		else if (!strcmp(string, "sbmost", true)) return 222;
		else if (!strcmp(string, "shmycr", true)) return 223;
		else if (!strcmp(string, "sofori", true)) return 224;
		else if (!strcmp(string, "sofost", true)) return 225;
		else if (!strcmp(string, "sofyst", true)) return 226;
		else if (!strcmp(string, "somobu", true)) return 227;
		else if (!strcmp(string, "somori", true)) return 228;
		else if (!strcmp(string, "somost", true)) return 229;
		else if (!strcmp(string, "swmotr5", true)) return 230;
		else if (!strcmp(string, "swfori", true)) return 231;
		else if (!strcmp(string, "swfost", true)) return 232;
		else if (!strcmp(string, "swfyst", true)) return 233;
		else if (!strcmp(string, "swmocd", true)) return 234;
		else if (!strcmp(string, "swmori", true)) return 235;
		else if (!strcmp(string, "swmost", true)) return 236;
		else if (!strcmp(string, "shfypro", true)) return 237;
		else if (!strcmp(string, "sbfypro", true)) return 238;
		else if (!strcmp(string, "swmotr4", true)) return 239;
		else if (!strcmp(string, "swmyri", true)) return 240;
		else if (!strcmp(string, "smyst", true)) return 241;
		else if (!strcmp(string, "smyst2", true)) return 242;
		else if (!strcmp(string, "sfypro", true)) return 243;
		else if (!strcmp(string, "vbfyst2", true)) return 244;
		else if (!strcmp(string, "vbfypro", true)) return 245;
		else if (!strcmp(string, "vhfyst3", true)) return 246;
		else if (!strcmp(string, "bikera", true)) return 247;
		else if (!strcmp(string, "bikerb", true)) return 248;
		else if (!strcmp(string, "bmypimp", true)) return 249;
		else if (!strcmp(string, "swmycr", true)) return 250;
		else if (!strcmp(string, "wfylg", true)) return 251;
		else if (!strcmp(string, "wmyva2", true)) return 252;
		else if (!strcmp(string, "bmosec", true)) return 253;
		else if (!strcmp(string, "bikdrug", true)) return 254;
		else if (!strcmp(string, "wmych", true)) return 255;
		else if (!strcmp(string, "sbfystr", true)) return 256;
		else if (!strcmp(string, "swfystr", true)) return 257;
		else if (!strcmp(string, "heck1", true)) return 258;
		else if (!strcmp(string, "heck2", true)) return 259;
		else if (!strcmp(string, "bmycon", true)) return 260;
		else if (!strcmp(string, "wmycd1", true)) return 261;
		else if (!strcmp(string, "bmocd", true)) return 262;
		else if (!strcmp(string, "vwfywa2", true)) return 263;
		else if (!strcmp(string, "wmoice", true)) return 264;
		else if (!strcmp(string, "tenpen", true)) return 265;
		else if (!strcmp(string, "pulaski", true)) return 266;
		else if (!strcmp(string, "hern", true)) return 267;
		else if (!strcmp(string, "dwayne", true)) return 268;
		else if (!strcmp(string, "smoke", true)) return 269;
		else if (!strcmp(string, "sweet", true)) return 270;
		else if (!strcmp(string, "ryder", true)) return 271;
		else if (!strcmp(string, "forelli", true)) return 272;
		else if (!strcmp(string, "tbone", true)) return 273;
		else if (!strcmp(string, "laemt1", true)) return 274;
		else if (!strcmp(string, "lvemt1", true)) return 275;
		else if (!strcmp(string, "sfemt1", true)) return 276;
		else if (!strcmp(string, "lafd1", true)) return 277;
		else if (!strcmp(string, "lvfd1", true)) return 278;
		else if (!strcmp(string, "sffd1", true)) return 279;
		else if (!strcmp(string, "lapd1", true)) return 280;
		else if (!strcmp(string, "sfpd1", true)) return 281;
		else if (!strcmp(string, "lvpd1", true)) return 282;
		else if (!strcmp(string, "csher", true)) return 283;
		else if (!strcmp(string, "lapdm1", true)) return 284;
		else if (!strcmp(string, "swat", true)) return 285;
		else if (!strcmp(string, "fbi", true)) return 286;
		else if (!strcmp(string, "army", true)) return 287;
		else if (!strcmp(string, "dsher", true)) return 288;
		else if (!strcmp(string, "zero", true)) return 289;
		else if (!strcmp(string, "rose", true)) return 290;
		else if (!strcmp(string, "paul", true)) return 291;
		else if (!strcmp(string, "cesar", true)) return 292;
		else if (!strcmp(string, "oglock", true)) return 293;
		else if (!strcmp(string, "wuzimu", true)) return 294;
		else if (!strcmp(string, "torino", true)) return 295;
		else if (!strcmp(string, "jizzy", true)) return 296;
		else if (!strcmp(string, "maddogg", true)) return 297;
		else if (!strcmp(string, "cat", true)) return 298;
		else if (!strcmp(string, "claude", true)) return 299;
		else if (!strcmp(string, "lapdna", true)) return 300;
		else if (!strcmp(string, "sfpdna", true)) return 301;
		else if (!strcmp(string, "lvpdna", true)) return 302;
		else if (!strcmp(string, "lapdpc", true)) return 303;
		else if (!strcmp(string, "lapdpd", true)) return 304;
		else if (!strcmp(string, "lvpdpc", true)) return 305;
		else if (!strcmp(string, "wfyclpd", true)) return 306;
		else if (!strcmp(string, "vbfycpd", true)) return 307;
		else if (!strcmp(string, "wfyclem", true)) return 308;
		else if (!strcmp(string, "wfycllv", true)) return 309;
		else if (!strcmp(string, "csherna", true)) return 310;
		else if (!strcmp(string, "dsherna", true)) return 311;
		return -1;
	}
#endif

// Fix the compiler crash when both the PAWN and Plugin versions of sscanf are
// found by renaming the old version at declaration.  (fixes.inc compatible
// naming scheme: "BAD_Function()").
#define SSCANF__(%0:...) BAD_sscanf(%0:...)

