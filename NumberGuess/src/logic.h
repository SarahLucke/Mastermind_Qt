#ifndef LOGIC_H
#define LOGIC_H

#define NUMBER_COUNT 5

#include <QRandomGenerator>

class Logic
{
public:
    enum e_modes{
        EASY,
        HARD
    };
    enum e_result{
        WRONG,
        WRONG_PLACE,
        CORRECT,
    };

    Logic(e_modes mode);
    void GetResult(int nums[], int result[]);

private:
    int rndNumbers[NUMBER_COUNT];
    int mode;
};

#endif // LOGIC_H
