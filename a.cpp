#include <iostream>

typedef struct {
    float x, y, z;
} vec3;

enum d_type {
    NONE = 0,
    INT,
    FLOAT,
    VEC3
};


typedef struct {
    d_type type = NONE;
    void* data = nullptr;
} Any;

class Fruit
{
    protected:
    uint32_t m_lol;
    public:
    Fruit(){}
    Fruit(uint32_t lol): m_lol(lol)
    {
        puts("Created a fruit \n");
    }
    ~Fruit()
    {
        puts("Deleted a fruit \n");
    }
    virtual void eat() = 0;
};

class Apple: public Fruit
{
    public:
    Apple(uint32_t lol)
    {
        puts("Created an apple \n");
    }
    ~Apple()
    {
        puts("Deleted an apple \n");
    }
    void eat()
    {
        
    }
};

int main(void)
{
    Apple apl(32);
    return 0;
}