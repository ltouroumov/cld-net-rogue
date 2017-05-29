//
// Created by ldavid on 5/23/17.
//

#include "Dungeon.hpp"

using namespace data;

Dungeon::Dungeon(const std::string &name)
    : mName(name)
{}

const std::string &Dungeon::getName() const
{
    return mName;
}

const std::vector<Level::Ptr> &Dungeon::getLevels() const
{
    return mLevels;
}

void Dungeon::addLevel(Level::Ptr &level)
{
    mLevels.push_back(level);
}
