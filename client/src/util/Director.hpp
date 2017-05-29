//
// Created by ldavid on 5/17/17.
//

#include <stack>
#include <mutex>
#include "Scene.hpp"

#ifndef PREZ_GAME_DIRECTOR_HPP
#define PREZ_GAME_DIRECTOR_HPP


class Director
{
public:
    Director();

    void tick(double delta);

    /**
     * "Fucking Tinkers!" -- Skitter
     */
    template<typename SceneType, typename... Args>
    inline void push(Args && ...args) {
        Scene* scene = new SceneType(this, std::forward<Args>(args)...);
        this->push(scene);
    }

    /**
     * "Tinkers are Bullshit!" -- Clockblocker
     */
    template<typename SceneType, typename... Args>
    inline void replace(Args && ...args) {
        Scene* scene = new SceneType(this, std::forward<Args>(args)...);
        this->replace(scene);
    }

    void push(Scene* scene);
    void pop();
    void replace(Scene* scene);
private:
    std::stack<Scene*> mSceneStack;
    std::recursive_mutex mStackAccess;
};


#endif //PREZ_GAME_DIRECTOR_HPP
