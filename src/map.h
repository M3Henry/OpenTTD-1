/* $Id$ */

/** @file map.h */

#ifndef MAP_H
#define MAP_H

#include "stdafx.h"
#include "direction.h"

extern uint _map_tile_mask;

/**
 * 'Wraps' the given tile to it is within the map. It does
 * this by masking the 'high' bits of.
 * @param x the tile to 'wrap'
 */

#define TILE_MASK(x) ((x) & _map_tile_mask)
/**
 * Asserts when the tile is outside of the map.
 * @param x the tile to check
 */
#define TILE_ASSERT(x) assert(TILE_MASK(x) == (x));

/**
 * Data that is stored per tile. Also used TileExtended for this.
 * Look at docs/landscape.html for the exact meaning of the members.
 */
struct Tile {
	byte type_height; ///< The type (bits 4..7) and height of the northern corner
	byte m1;   ///< Primarily used for ownership information
	uint16 m2; ///< Primarily used for indices to towns, industries and stations
	byte m3;   ///< General purpose
	byte m4;   ///< General purpose
	byte m5;   ///< General purpose
	byte m6;   ///< Primarily used for bridges and rainforest/desert
};

/**
 * Data that is stored per tile. Also used Tile for this.
 * Look at docs/landscape.html for the exact meaning of the members.
 */
struct TileExtended {
	byte m7; ///< Primarily used for newgrf support
};

/**
 * Pointer to the tile-array.
 *
 * This variable points to the tile-array which contains the tiles of
 * the map.
 */
extern Tile *_m;

/**
 * Pointer to the extended tile-array.
 *
 * This variable points to the extended tile-array which contains the tiles
 * of the map.
 */
extern TileExtended *_me;

/**
 * Allocate a new map with the given size.
 */
void AllocateMap(uint size_x, uint size_y);

/**
 * Logarithm of the map size along the X side.
 * @note try to avoid using this one
 * @return 2^"return value" == MapSizeX()
 */
static inline uint MapLogX()
{
	extern uint _map_log_x;
	return _map_log_x;
}

/**
 * Get the size of the map along the X
 * @return the number of tiles along the X of the map
 */
static inline uint MapSizeX()
{
	extern uint _map_size_x;
	return _map_size_x;
}

/**
 * Get the size of the map along the Y
 * @return the number of tiles along the Y of the map
 */
static inline uint MapSizeY()
{
	extern uint _map_size_y;
	return _map_size_y;
}

/**
 * Get the size of the map
 * @return the number of tiles of the map
 */
static inline uint MapSize()
{
	extern uint _map_size;
	return _map_size;
}

/**
 * Gets the maximum X coordinate within the map, including MP_VOID
 * @return the maximum X coordinate
 */
static inline uint MapMaxX()
{
	return MapSizeX() - 1;
}

/**
 * Gets the maximum X coordinate within the map, including MP_VOID
 * @return the maximum X coordinate
 */
static inline uint MapMaxY()
{
	return MapSizeY() - 1;
}

/**
 * Scales relative to the number of tiles.
 */
uint ScaleByMapSize(uint);

/**
 * Scale relative to the circumference of the map.
 */
uint ScaleByMapSize1D(uint);

/**
 * The index/ID of a Tile.
 */
typedef uint32 TileIndex;

/**
 * An offset value between to tiles.
 *
 * This value is used fro the difference between
 * to tiles. It can be added to a tileindex to get
 * the resulting tileindex of the start tile applied
 * with this saved difference.
 *
 * @see TileDiffXY(int, int)
 */
typedef int32 TileIndexDiff;

/**
 * Returns the TileIndex of a coordinate.
 *
 * @param x The x coordinate of the tile
 * @param y The y coordinate of the tile
 * @return The TileIndex calculated by the coordinate
 */
static inline TileIndex TileXY(uint x, uint y)
{
	return (y * MapSizeX()) + x;
}

/**
 * Calculates an offset for the given coordinate(-offset).
 *
 * This function calculate an offset value which can be added to an
 * #TileIndex. The coordinates can be negative.
 *
 * @param x The offset in x direction
 * @param y The offset in y direction
 * @return The resulting offset value of the given coordinate
 * @see ToTileIndexDiff(TileIndexDiffC)
 */
static inline TileIndexDiff TileDiffXY(int x, int y)
{
	/* Multiplication gives much better optimization on MSVC than shifting.
	 * 0 << shift isn't optimized to 0 properly.
	 * Typically x and y are constants, and then this doesn't result
	 * in any actual multiplication in the assembly code.. */
	return (y * MapSizeX()) + x;
}

static inline TileIndex TileVirtXY(uint x, uint y)
{
	return (y >> 4 << MapLogX()) + (x >> 4);
}


enum {
	INVALID_TILE = (TileIndex)-1 ///< The very nice invalid tile marker
};

enum {
	TILE_SIZE   = 16,   ///< Tiles are 16x16 "units" in size
	TILE_PIXELS = 32,   ///< a tile is 32x32 pixels
	TILE_HEIGHT =  8,   ///< The standard height-difference between tiles on two levels is 8 (z-diff 8)
};


/**
 * Get the X component of a tile
 * @param tile the tile to get the X component of
 * @return the X component
 */
static inline uint TileX(TileIndex tile)
{
	return tile & MapMaxX();
}

/**
 * Get the Y component of a tile
 * @param tile the tile to get the Y component of
 * @return the Y component
 */
static inline uint TileY(TileIndex tile)
{
	return tile >> MapLogX();
}

/**
 * A pair-construct of a TileIndexDiff.
 *
 * This can be used to save the difference between to
 * tiles as a pair of x and y value.
 */
struct TileIndexDiffC {
	int16 x;        ///< The x value of the coordinate
	int16 y;        ///< The y value of the coordinate
};

/**
 * Return the offset between to tiles from a TileIndexDiffC struct.
 *
 * This function works like #TileDiffXY(int, int) and returns the
 * difference between two tiles.
 *
 * @param tidc The coordinate of the offset as TileIndexDiffC
 * @return The difference between two tiles.
 * @see TileDiffXY(int, int)
 */
static inline TileIndexDiff ToTileIndexDiff(TileIndexDiffC tidc)
{
	return (tidc.y << MapLogX()) + tidc.x;
}


#ifndef _DEBUG
        /**
         * Adds to tiles together.
         *
         * @param x One tile
         * @param y An other tile to add
         * @return The resulting tile(index)
         */
	#define TILE_ADD(x,y) ((x) + (y))
#else
	extern TileIndex TileAdd(TileIndex tile, TileIndexDiff add,
		const char *exp, const char *file, int line);
	#define TILE_ADD(x, y) (TileAdd((x), (y), #x " + " #y, __FILE__, __LINE__))
#endif

/**
 * Adds a given offset to a tile.
 *
 * @param tile The tile to add an offset on it
 * @param x The x offset to add to the tile
 * @param y The y offset to add to the tile
 */
#define TILE_ADDXY(tile, x, y) TILE_ADD(tile, TileDiffXY(x, y))

/**
 * Adds an offset to a tile and check if we are still on the map.
 */
uint TileAddWrap(TileIndex tile, int addx, int addy);

/**
 * Returns the TileIndexDiffC offset from a DiagDirection.
 *
 * @param dir The given direction
 * @return The offset as TileIndexDiffC value
 */
static inline TileIndexDiffC TileIndexDiffCByDiagDir(DiagDirection dir)
{
	extern const TileIndexDiffC _tileoffs_by_diagdir[DIAGDIR_END];

	assert(IsValidDiagDirection(dir));
	return _tileoffs_by_diagdir[dir];
}
/**
 * Add a TileIndexDiffC to a TileIndex and returns the new one.
 *
 * Returns tile + the diff given in diff. If the result tile would end up
 * outside of the map, INVALID_TILE is returned instead.
 *
 * @param tile The base tile to add the offset on
 * @param diff The offset to add on the tile
 * @return The resulting TileIndex
 */
static inline TileIndex AddTileIndexDiffCWrap(TileIndex tile, TileIndexDiffC diff)
{
	int x = TileX(tile) + diff.x;
	int y = TileY(tile) + diff.y;
	if (x < 0 || y < 0 || x > (int)MapMaxX() || y > (int)MapMaxY())
		return INVALID_TILE;
	else
		return TileXY(x, y);
}

/**
 * Returns the diff between two tiles
 *
 * @param tile_a from tile
 * @param tile_b to tile
 * @return the difference between tila_a and tile_b
 */
static inline TileIndexDiffC TileIndexToTileIndexDiffC(TileIndex tile_a, TileIndex tile_b)
{
	TileIndexDiffC difference;

	difference.x = TileX(tile_a) - TileX(tile_b);
	difference.y = TileY(tile_a) - TileY(tile_b);

	return difference;
}

/* Functions to calculate distances */
uint DistanceManhattan(TileIndex, TileIndex); ///< also known as L1-Norm. Is the shortest distance one could go over diagonal tracks (or roads)
uint DistanceSquare(TileIndex, TileIndex); ///< euclidian- or L2-Norm squared
uint DistanceMax(TileIndex, TileIndex); ///< also known as L-Infinity-Norm
uint DistanceMaxPlusManhattan(TileIndex, TileIndex); ///< Max + Manhattan
uint DistanceFromEdge(TileIndex); ///< shortest distance from any edge of the map

/**
 * Starts a loop which iterates to a square of tiles
 *
 * This macro starts 2 nested loops which iterates over a square of tiles.
 *
 * @param var The name of the variable which contains the current tile
 * @param w The width (x-width) of the square
 * @param h The heigth (y-width) of the square
 * @param tile The start tile of the square
 */
#define BEGIN_TILE_LOOP(var, w, h, tile)                      \
	{                                                        \
		int h_cur = h;                                         \
		uint var = tile;                                       \
		do {                                                   \
			int w_cur = w;                                       \
			do {
/**
 * Ends the square-loop used before
 *
 * @see BEGIN_TILE_LOOP
 */
#define END_TILE_LOOP(var, w, h, tile)                        \
			} while (++var, --w_cur != 0);                       \
		} while (var += TileDiffXY(0, 1) - (w), --h_cur != 0); \
	}
/**
 * Convert a DiagDirection to a TileIndexDiff
 *
 * @param dir The DiagDirection
 * @return The resulting TileIndexDiff
 * @see TileIndexDiffCByDiagDir
 */
static inline TileIndexDiff TileOffsByDiagDir(DiagDirection dir)
{
	extern const TileIndexDiffC _tileoffs_by_diagdir[DIAGDIR_END];

	assert(IsValidDiagDirection(dir));
	return ToTileIndexDiff(_tileoffs_by_diagdir[dir]);
}

/**
 * Convert a Direction to a TileIndexDiff.
 *
 * @param dir The direction to convert from
 * @return The resulting TileIndexDiff
 */
static inline TileIndexDiff TileOffsByDir(Direction dir)
{
	extern const TileIndexDiffC _tileoffs_by_dir[DIR_END];

	assert(IsValidDirection(dir));
	return ToTileIndexDiff(_tileoffs_by_dir[dir]);
}

/**
 * A callback function type for searching tiles.
 *
 * @param tile The tile to test
 * @param data additional data for the callback function to use
 * @return A boolean value, depend on the definition of the function.
 */
typedef bool TestTileOnSearchProc(TileIndex tile, uint32 data);

/**
 * Searches for some cirumstances of a tile around a given tile with a helper function.
 */
bool CircularTileSearch(TileIndex tile, uint size, TestTileOnSearchProc proc, uint32 data);

/** Approximation of the length of a straight track, relative to a diagonal
 * track (ie the size of a tile side).
 *
 * #defined instead of const so it can
 * stay integer. (no runtime float operations) Is this needed?
 * Watch out! There are _no_ brackets around here, to prevent intermediate
 * rounding! Be careful when using this!
 * This value should be sqrt(2)/2 ~ 0.7071 */
#define STRAIGHT_TRACK_LENGTH 7071/10000

#endif /* MAP_H */
