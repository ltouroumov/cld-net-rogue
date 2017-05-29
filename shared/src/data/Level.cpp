//
// Created by ldavid on 5/23/17.
//

#include "Level.hpp"

using namespace data;

Level::Level(const std::string &name)
    : mName(name), mSize(0, 0)
{

}

void Level::setSize(Level::Size size)
{
    mSize = size;
}

const Level::Size &Level::size() const
{
    return mSize;
}

void Level::addLayer(Level::Layer layer)
{
    mLayers.push_back(layer);
}

const std::vector<Level::Layer> &Level::layers() const
{
    return mLayers;
}

Level::Layer &Level::getLayer(size_t index)
{
    return mLayers.at(index);
}

const std::string &Level::name() const
{
    return mName;
}

Level::Tile Level::at(uint16_t x, uint16_t y)
{
    for (size_t lidx = mLayers.size() - 1; lidx >= 0; --lidx) {
        char tile = getLayer(lidx).at(x, y);
        if (tile != ' ')
            return Tile(tile, getLayer(lidx).solid);
    }

    return Tile(0, false);
}

