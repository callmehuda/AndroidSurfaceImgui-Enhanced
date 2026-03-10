#include "Hooks.h"
#include "Vars.h"
#include "Dobby/dobby.h"
// <== Hooks ==>    

/*void (*Main$$_Draw)(BNM::IL2CPP::Il2CppObject *instance, void *gameTime);
void Main$$Draw(BNM::IL2CPP::Il2CppObject *instance, void *gameTime)
{
    Main$$_Draw(instance, gameTime);
    ofs::Main::spriteBatch.Get();
}*/

MYHOOK(void, set_curSnowNum, void *instance, int value) {

    if (snowAurora) {
        value = 6; 
    }

    origset_curSnowNum(instance, value);
}

MYHOOK(bool, IsPreparePhase) {
    bool result = origIsPreparePhase();

    if (forceIsPrepare) return true;

    return result; 
}

// <== Initializing ==>
void Setup_Hooks() {
    
    auto assembly = UnityResolve::Get("Assembly-CSharp.dll");

    auto snowAuroraMethod = assembly->Get("MCLogicSpecialMonsterAurora", "*")->Get<UnityResolve::Method>("set_curSnowNum", {"System.Int32"});
    
    auto isPreparePhaseMethod = assembly->Get("MCLogicUtils", "*")->Get<UnityResolve::Method>("IsPreparePhase");
    
    
    startPreparePhaseMethod = assembly->Get("MCLogicBattleManager")->Get<UnityResolve::Method>("OnStartPreparePhase");
    
    
    
    DobbyHook(
        snowAuroraMethod->function,
        (void*)myset_curSnowNum,
        (void**)&origset_curSnowNum
    );
    DobbyHook(
        isPreparePhaseMethod->function, 
        (void*)myIsPreparePhase, 
        (void**)&origIsPreparePhase
    );    
}
