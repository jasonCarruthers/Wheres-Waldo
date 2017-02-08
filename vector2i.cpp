#include "Vector2I.h"


/*
 * Implementation of class Vector2I.
 */
/*Constructors/destructor*/
Vector2I::Vector2I(int newX, int newY)
{
    mX = newX;
    mY = newY;
}

/*Accessors*/
int Vector2I::GetX() const
{
    return mX;
}

int Vector2I::GetY() const
{
    return mY;
}

float Vector2I::GetDistanceSquaredFromPoint(const Vector2I& other) const
{
    return (float)((mX - other.mX)*(mX - other.mX) + (mY - other.mY)*(mY - other.mY));
}

/*Mutators*/
void Vector2I::SetX(int newX)
{
    mX = newX;
}

void Vector2I::SetY(int newY)
{
    mY = newY;
}

/*Overloaded operators.*/
Vector2I& Vector2I::operator=(const Vector2I &right)
{
    mX = right.mX;
    mY = right.mY;

    return *this;
}

void Vector2I::operator+=(const Vector2I &right)
{
    mX += right.mX;
    mY += right.mY;
}

void Vector2I::operator/=(int divisor)
{
    mX /= divisor;
    mY /= divisor;
}
