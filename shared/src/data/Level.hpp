//
// Created by ldavid on 5/23/17.
//

#include <memory>
#include <yaml-cpp/yaml.h>

#ifndef PREZ_GAME_DATA_LEVEL_HPP
#define PREZ_GAME_DATA_LEVEL_HPP

namespace data
{

    class Level
    {
    public:
        typedef std::shared_ptr<Level> Ptr;

        struct Size
        {
            uint16_t width;
            uint16_t height;

            Size()
                : width(0), height(0)
            {}

            Size(uint16_t w, uint16_t h)
                : width(w), height(h)
            {}

            size_t area()
            {
                return width * height;
            }
        };

        struct Layer
        {
            std::string name;
            Size size;
            std::string tiles;
            bool solid;

            Layer(const std::string &name, Size pSize = Size(0, 0))
                : name(name), size(pSize), solid(false)
            {
                tiles.resize(size.area(), ' ');
            }

            char at(uint16_t x, uint16_t y)
            {
                return tiles.at(x + y * size.width);
            }
        };

        struct Tile
        {
            char tile;
            bool solid;

            Tile(char tile, bool solid)
                : tile(tile), solid(solid)
            {}
        };

        Level(const std::string &name);

        const std::string &name() const;

        void setSize(Size size);

        const Size &size() const;

        void addLayer(Layer layer);

        Layer &getLayer(size_t index);

        const std::vector<Layer> &layers() const;

        Tile at(uint16_t x, uint16_t y);

    private:
        std::string mName;
        Size mSize;
        std::vector<Layer> mLayers;
    };

}

namespace YAML
{
    template<>
    struct convert<data::Level::Size>
    {
        static Node encode(const data::Level::Size &rhs)
        {
            Node node;
            node.push_back(rhs.width);
            node.push_back(rhs.height);
            return node;
        }

        static bool decode(const Node &node, data::Level::Size &rhs)
        {
            if (!node.IsSequence() || node.size() != 2) {
                return false;
            }

            rhs.width = node[0].as<uint16_t>();
            rhs.height = node[1].as<uint16_t>();
            return true;
        }
    };
}

#endif //PREZ_GAME_LEVEL_HPP
