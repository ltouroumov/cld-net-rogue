//
// Created by ldavid on 5/23/17.
//

#ifndef PREZ_GAME_ENTITY_HPP
#define PREZ_GAME_ENTITY_HPP

#include <memory>

namespace world
{
    class Level;

    class Entity : public std::enable_shared_from_this<Entity>
    {
    public:
        struct Position
        {
            Position(int x, int y)
                : x(x), y(x)
            {}

            int x, y;
        };

        struct Display
        {
            char icon;
        };

        struct Controller
        {
            virtual void think(Entity& entity, size_t tickId);
        };

        Entity(size_t id, Controller* controller);
        ~Entity();

        virtual void doTick(size_t tickId);

        void attach(std::shared_ptr<Level> level);

        void kill();

        std::shared_ptr<Level> &level();

        const size_t& id();
        Position& position();
        Display& display();
        Controller* controller();

    protected:
        std::shared_ptr<Level> mLevel;
        size_t mId;
        Position mPosition;
        Display mDisplay;
        Controller* mController;
    };

    class PlayerController : public Entity::Controller
    {
    public:
        void think(Entity &entity, size_t tickId) override;

        void move(uint8_t nextMove);

    private:
        uint8_t mNextMove;
    };

}


#endif //PREZ_GAME_ENTITY_HPP
