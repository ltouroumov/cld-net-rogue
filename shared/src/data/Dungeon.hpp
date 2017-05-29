//
// Created by ldavid on 5/23/17.
//

#include <string>
#include <vector>
#include <memory>
#include "Level.hpp"

#ifndef PREZ_GAME_DUNGEON_HPP
#define PREZ_GAME_DUNGEON_HPP

namespace data
{

    class Dungeon
    {
    public:
        typedef std::shared_ptr<Dungeon> Ptr;

        Dungeon(const std::string &name);

        const std::string &getName() const;

        const std::vector<Level::Ptr> &getLevels() const;

        void addLevel(Level::Ptr &level);

    private:
        const std::string mName;
        std::vector<Level::Ptr> mLevels;
    };

}

#endif //PREZ_GAME_DUNGEON_HPP
