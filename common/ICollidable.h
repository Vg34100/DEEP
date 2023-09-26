// ICollidable.h

#pragma once

class ICollidable {
public:
    virtual ~ICollidable() = default;

    // This function should be implemented by concrete classes (like Player or Enemy) to
    // return whether the object is currently colliding with another entity or obstacle.
    virtual bool isColliding() = 0;
};
