//
// Created by ldavid on 5/17/17.
//

#ifndef PREZ_GAME_SCENE_HPP
#define PREZ_GAME_SCENE_HPP

class Director;

class Scene
{
public:
    Scene(Director* director)
        : mDirector(director)
    {}

    virtual void tick(double delta) = 0;

    // When moving through the stack
    virtual void enter() {};
    virtual void leave() {};

    virtual void attached() {};
    virtual void detached() {};
protected:
    Director* director() { return mDirector; }
private:
    Director* mDirector;
};


#endif //PREZ_GAME_SCENE_HPP
