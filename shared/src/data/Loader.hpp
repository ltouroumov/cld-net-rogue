//
// Created by ldavid on 5/23/17.
//

#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Dungeon.hpp"

#ifndef PREZ_GAME_LOADER_HPP
#define PREZ_GAME_LOADER_HPP

namespace data
{

    class Loader
    {
    public:
        Loader(const std::string &base);

        std::string filePath(const std::string& fileName);

        Dungeon::Ptr getDungeon(const std::string &name);

        Level::Ptr getLevel(const std::string &name);

        std::unordered_map<std::string, Dungeon::Ptr> getDungeons();

    private:
        void buildIndex();
        Dungeon::Ptr loadDungeon(const std::string& file);
        Level::Ptr loadLevel(const std::string& file, const std::string& levelName);

        const std::string mBase;
        std::unordered_map<std::string, Dungeon::Ptr> mDungeons;
        std::unordered_map<std::string, Level::Ptr> mLevels;
    };

}

#endif //PREZ_GAME_LOADER_HPP
