//
// Created by ldavid on 5/23/17.
//

#include "Loader.hpp"
#include <yaml-cpp/yaml.h>
#include <spdlog/fmt/bundled/format.h>
#include <spdlog/spdlog.h>

using namespace data;

Loader::Loader(const std::string &base)
    : mBase(base)
{
    buildIndex();
}

void Loader::buildIndex()
{
    auto logger = spdlog::get("main");

    YAML::Node index = YAML::LoadFile(filePath("index.yaml"));

    logger->debug("Loading levels");
    for(auto levelData : index["levels"]) {
        std::string name = levelData["name"].as<std::string>();
        std::string file = levelData["file"].as<std::string>();
        Level::Ptr level = loadLevel(file, name);
        mLevels.insert(std::make_pair(name, level));
    }

    logger->debug("Loading dungeons");
    for(auto dungeonData : index["dungeons"]) {
        std::string name = dungeonData["name"].as<std::string>();
        std::string file = dungeonData["file"].as<std::string>();
        Dungeon::Ptr dungeon = loadDungeon(file);
        mDungeons.insert(std::make_pair(name, dungeon));
    }

    logger->debug("Loading done");
}

std::string Loader::filePath(const std::string &fileName)
{
    return fmt::format("{:s}/{:s}", mBase, fileName);
}

Dungeon::Ptr Loader::loadDungeon(const std::string& file)
{
    YAML::Node meta = YAML::LoadFile(filePath(file));

    Dungeon::Ptr dungeon = std::make_shared<Dungeon>(meta["name"].as<std::string>());

    for (auto levelName : meta["levels"]) {
        std::string name = levelName.as<std::string>();
        Level::Ptr level = getLevel(name);
        dungeon->addLevel(level);
    }

    return dungeon;
}

Level::Ptr Loader::loadLevel(const std::string &file, const std::string& levelName)
{
    YAML::Node data = YAML::LoadFile(filePath(file));

    Level::Ptr level = std::make_shared<Level>(levelName);
    level->setSize(data["size"].as<Level::Size>());

    for (auto pair : data["layers"]) {
        std::string name = pair.first.as<std::string>();
        YAML::Node layerData = pair.second;
        bool is_solid = layerData["is_solid"].as<bool>();
        std::string tiles = layerData["data"].as<std::string>();
        Level::Layer layer(name, level->size());
        layer.solid = is_solid;
        for(int tidx = 0; tidx < layer.size.area(); ++tidx) {
            int offset = tidx / layer.size.height;
            layer.tiles[tidx] = tiles[tidx + offset];
        }

        level->addLayer(layer);
    }

    return level;
}

std::unordered_map<std::string, Dungeon::Ptr> Loader::getDungeons()
{
    return mDungeons;
}

Level::Ptr Loader::getLevel(const std::string &name)
{
    return mLevels.at(name);
}

Dungeon::Ptr Loader::getDungeon(const std::string &name)
{
    return mDungeons.at(name);
}
