#pragma once
#include "raylib.h"
#include <cstddef>
#include <vector>
#include <Components/Position.h>

class SpatialGrid
{
public:
    //Create a uniform grid with the given cell size and world size
    //this is for the collision detection system, to reduce the number of checks needed

    SpatialGrid(int worldWidth, int worldHeight, float cellSize);

    void Clear(); //to remove all entities from the grid

    //inserts an enemy's dense vector idx into the grid based on its position
    void InsertEnemy(Vector2 position, size_t enemyIndex);

    //same for projectile
    void InsertProjectile(Vector2 position, size_t projectileIndex);

    //returns the enemy indices contaid in the cell at the given position
    std::vector<size_t>QueryEnemies(Vector2 position) const;

    //returns the projectile indices contaid in the cell at the given position
    std::vector<size_t>QueryProjectiles(Vector2 position) const;

    // Returns enemy indices contained in all grid cells
    // touched by the query radius
    // This prevents collisions from being missed when
    // a projectile and enemy are in neighboring cells.
    std::vector<size_t>QueryEnemiesInRadius(Vector2 position, float radius) const;

    //rebuilds he enemy portion of the grid 
    //from the current enemy dense enemy pos array
    void BuildEnemyGrid(const std::vector<Position>& enemyPositions);

    //world pos into grid cell index
    size_t GetCellIndex(Vector2 position) const;

    // Returns the number of cells in the grid.
    size_t GetCellCount() const;

    // Returns the number of columns.
    int GetColumnCount() const;

    // Returns the number of rows.
    int GetRowCount() const;

    // Returns the configured cell size.
    float GetCellSize() const;

    size_t GetEnemyCountInCell(int col, int row) const;

    size_t GetProjectileCountInCell(int col, int row) const;


private:

    int GetColumn(Vector2 position) const;
    int GetRow(Vector2 position) const;
    size_t ToCellIndex(int col, int row) const;

private:

    int m_worldWidth;
    int m_worldHeight;

    float m_cellSize;

    int m_columns;
    int m_rows;

    // Each cell contains a vector of enemy indices and a vector of projectile indices
    std::vector<std::vector<size_t>> m_enemyCells;
    std::vector<std::vector<size_t>> m_projectileCells;

};