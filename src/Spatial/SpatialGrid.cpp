#include "SpatialGrid.h"
#include <algorithm>
#include <cmath>

SpatialGrid::SpatialGrid(int worldWidth, int worldHeight, float cellSize)
    : m_worldWidth(worldWidth), m_worldHeight(worldHeight), m_cellSize(cellSize)
    , m_columns(static_cast<int>(std::ceil(worldWidth / cellSize))) //columns = worldWidth / cellSize
    , m_rows(static_cast<int>(std::ceil(worldHeight / cellSize))) //rows = worldHeight / cellSize
{

    //allocate one vector for every grid cell
    const size_t cellCount = static_cast<size_t>(m_columns * m_rows);
    m_enemyCells.resize(cellCount);
    m_projectileCells.resize(cellCount);
}

void SpatialGrid::Clear()
{
    //we only remove the indices stored inside the vectors, we don't remove the vectors themselves

    for(std::vector<size_t>& cell : m_enemyCells)
    {
        cell.clear();
    }
    for(std::vector<size_t>& cell : m_projectileCells)
    {
        cell.clear();
    }
}

void SpatialGrid::InsertEnemy(Vector2 position, size_t enemyIndex)
{
    const size_t cellIndex = GetCellIndex(position);
    //insert the enemy index into the vector of the cell
    m_enemyCells[cellIndex].push_back(enemyIndex);
}

void SpatialGrid::InsertProjectile(Vector2 position, size_t projectileIndex)
{
    const size_t cellIndex = GetCellIndex(position);
    //insert the projectile index into the vector of the cell
    m_projectileCells[cellIndex].push_back(projectileIndex);
}

std::vector<size_t> SpatialGrid::QueryEnemies(Vector2 position) const
{
    const size_t cellIndex = GetCellIndex(position);

    return m_enemyCells[cellIndex]; //returns the copy of the vector of enemy indices in the cell
}

std::vector<size_t> SpatialGrid::QueryProjectiles(Vector2 position) const
{
    const size_t cellIndex = GetCellIndex(position);    
    return m_projectileCells[cellIndex]; //returns the copy of the vector of projectile indices in the cell
}

size_t SpatialGrid::GetCellIndex(Vector2 position) const
{
    const int col = GetColumn(position);
    const int row = GetRow(position);
    return ToCellIndex(col, row);
}

size_t SpatialGrid::GetCellCount() const
{
    return m_enemyCells.size();
}

int SpatialGrid::GetColumnCount() const
{
    return m_columns;
}

int SpatialGrid::GetRowCount() const
{
    return m_rows;
}

float SpatialGrid::GetCellSize() const
{
    return m_cellSize;
}



size_t SpatialGrid::GetEnemyCountInCell(int col, int row) const
{   
    //col and row are clamped to the valid range of the grid
    col = std::max(0, std::min(col, m_columns - 1)); 
    row = std::max(0, std::min(row, m_rows - 1));    

    const size_t cellIndex = ToCellIndex(col, row);
    return m_enemyCells[cellIndex].size();
}

size_t SpatialGrid::GetProjectileCountInCell(int col, int row) const
{
    col = std::max(0, std::min(col, m_columns - 1));
    row = std::max(0, std::min(row, m_rows - 1));

    const size_t cellIndex = ToCellIndex(col, row);
    return m_projectileCells[cellIndex].size();
}

int SpatialGrid::GetColumn(Vector2 position) const
{
    //convert world X coordinate into grid col
    int col = static_cast<int>(std::floor(position.x / m_cellSize));
    
    //clamp so objects outside the screen don't cause out of bounds access
    col = std::max(0, std::min(col, m_columns - 1));
    return col;
}

int SpatialGrid::GetRow(Vector2 position) const
{
    //convert world Y coordinate into grid row
    int row = static_cast<int>(std::floor(position.y / m_cellSize));

    //clamp so objects outside the screen don't cause out of bounds access
    row = std::max(0, std::min(row, m_rows - 1));
    return row;
}

size_t SpatialGrid::ToCellIndex(int col, int row) const
{
    //flatten 2D grid coordinates into a 1D vector index
    return static_cast<size_t>(row * m_columns + col);  //row major order
    //example: col=2, row=3, columns=5 => index = 3*5 + 2 = 17
}
