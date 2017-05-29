//
// Created by ldavid on 5/17/17.
//

#include "Director.hpp"

Director::Director()
{
}

void Director::tick(double delta)
{
    std::lock_guard<std::recursive_mutex> guard(mStackAccess);

    mSceneStack.top()->tick(delta);
}

void Director::push(Scene *scene)
{
    std::lock_guard<std::recursive_mutex> guard(mStackAccess);

    if (mSceneStack.size() > 0) {
        Scene *oldTop = mSceneStack.top();
        oldTop->leave();
    }
    mSceneStack.push(scene);
    scene->attached();
    scene->enter();
}

void Director::pop()
{
    std::lock_guard<std::recursive_mutex> guard(mStackAccess);

    if (mSceneStack.size() > 1) {
        Scene *oldTop = mSceneStack.top();
        mSceneStack.pop();
        Scene *newTop = mSceneStack.top();
        oldTop->leave();
        oldTop->detached();
        newTop->enter();

        delete oldTop;
    }
}

void Director::replace(Scene *scene)
{
    std::lock_guard<std::recursive_mutex> guard(mStackAccess);

    Scene* oldTop = mSceneStack.top();
    mSceneStack.pop();
    mSceneStack.push(scene);
    oldTop->leave();
    oldTop->detached();
    scene->attached();
    scene->enter();
    
    delete oldTop;
}

