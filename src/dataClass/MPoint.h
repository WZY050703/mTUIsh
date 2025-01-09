#pragma once

template <typename T>
class MPoint
{
public:
    T x, y;
    MPoint();
    MPoint(T n);
    MPoint(T x, T y);
    void operator=(const MPoint other)
    {
        this->x = other.x;
        this->y = other.y;
        return;
    }
    MPoint operator+(const MPoint other)
    {
        return MPoint(this->x + other.x, this->y + other.y);
    }
    MPoint operator-(const MPoint other)
    {
        return MPoint(this->x - other.x, this->y - other.y);
    }
    MPoint operator*(const MPoint other)
    {
        return MPoint(this->x * other.x, this->y * other.y);
    }
    MPoint operator*(const T multObj) // 乘同一值
    {
        return MPoint(this->x * multObj, this->y * multObj);
    }
    MPoint operator/(const MPoint other)
    {
        return MPoint(this->x / other.x, this->y / other.y);
    }
    MPoint operator/(const T subObj) // 除同一值
    {
        return MPoint(this->x / subObj, this->y / subObj);
    }
    bool operator[](const MPoint other) // 判断括号内的在原点(1,1)到该点范围内
    {                                   // 有点无奈，只能传入一个数据
        return (1 <= other.x && other.x <= this->x && 1 <= other.y && other.y <= this->y);
    }
};

template <class T>
MPoint<T>::MPoint()
{
    ;
}

template <class T>
MPoint<T>::MPoint(T n)
{
    this->x = n;
    this->y = n;
}

template <class T> // 不知道为什么，必须要拿出来定义
MPoint<T>::MPoint(T x, T y)
{
    this->x = x;
    this->y = y;
}