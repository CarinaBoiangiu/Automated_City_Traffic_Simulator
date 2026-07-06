#ifndef IINTERSECTIONOBSERVER_H
#define IINTERSECTIONOBSERVER_H

enum class LightColor {
    RED,
    GREEN,
    YELLOW
};

class IIntersectionObserver{
    public:
        virtual ~IIntersectionObserver() = default;
        virtual  void update(LightColor color) = 0;
};

#endif // IINTERSECTIONOBSERVER_H
