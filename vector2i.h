#ifndef VECTOR2I_H
#define VECTOR2I_H

class Vector2I
{
public:
    /*Constructors/destructor*/
    Vector2I(int newX = 0, int newY = 0);

    /*Accessors*/
    int GetX() const;
    int GetY() const;
    float GetDistanceSquaredFromPoint(const Vector2I& other) const;

    /*Mutators*/
    void SetX(int newX);
    void SetY(int newY);

    /*Overloaded operators.*/
    Vector2I& operator=(const Vector2I &right);
    void operator+=(const Vector2I& right);
    void operator/=(int divisor);

private:
    int mX;
    int mY;
};


#endif // VECTOR2I_H
