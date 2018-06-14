#include "logic.h"

Logic::Logic(Logic::e_modes mode)
{
    this->mode = mode;
    for(int i=0; i<NUMBER_COUNT; i++){
        bool repeat = true;
        while(repeat==true){
            rndNumbers[i] = QRandomGenerator::global()->bounded(0,10);
            repeat = false;
            int k;
            for(k=0; k<NUMBER_COUNT; k++){
                if(i==0){
                    repeat=false;
                    break;
                }
                if(i!=k){
                    if(rndNumbers[i]==rndNumbers[k]){
                        repeat = true;
                        break;
                    }
                }
            }
        }
    }
}

void Logic::GetResult(int nums[], int result[]){
    for(int i=0; i<NUMBER_COUNT; i++){
        if(rndNumbers[i]==nums[i]){
            result[i]=CORRECT;
        }else{
            result[i]=0;
            for(int k=0; k<NUMBER_COUNT; k++){
                if(nums[i]==rndNumbers[k]){
                    result[i]=WRONG_PLACE;
                    break;
                }
            }
        }
    }
}
