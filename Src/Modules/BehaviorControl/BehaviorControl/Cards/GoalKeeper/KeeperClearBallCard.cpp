#include "Representations/BehaviorControl/FieldBall.h"
#include "Representations/BehaviorControl/Skills.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Modeling/RobotPose.h"
#include "Tools/BehaviorControl/Framework/Card/Card.h"
#include "Tools/BehaviorControl/Framework/Card/CabslCard.h"

CARD(KeeperClearBallCard,{,
    CALLS(Activity),//los llamados son para llamar todas las skills se hace por CALLS()
    CALLS(LookForward),
    CALLS(Stand),
    CALLS(WalkAtRelativeSpeed),
    CALLS(WalkToTarget),
    REQUIRES(FieldBall),//Llama a los representaciones que necesita, siempre se usan estas por lo general
    REQUIRES(FieldDimensions),
    REQUIRES(RobotPose),
    DEFINES_PARAMETERS( 
    //Aca son todas las variables que se quiera definir para despues usarse dentro de la clase
    {,//Mirar cuales sirven y cuales no
        //definir parametros
      (float)(0.5f) walkSpeed,
      (int)(1000) initialWaitTime,
      (int)(4000) ballNotSeenTimeOut,
      (float)(300.f) ballNearThreshold,
    }),
}); 

class KeeperClearBallCard : public KeeperClearBallCardBase
{
    bool preconditions() const override
    {
        return true;
    }

    bool postconditions() const override
    {
        return true;
    }

    option{
      theActivitySkill(BehaviorStatus::KeeperClearBallCard);
     initial_state(start){
        transition{
          if(state_time > initialWaitTime)
            goto turnToBall;//Aca es el cambio de escena
        }
        action{
          theLookForwardSkill();
          theStandSkill();
          //Lo que quieres que haga cuando la se llegue a este estado
        }
      }
      state(turnToBall){
        transition{
            if(!theFieldBall.ballWasSeen(ballNotSeenTimeOut))
              goto searchForBall;
            else
              goto waitToBall;
            
        }
        action{
          theLookForwardSkill();
          theStandSkill();
        }
      }
      state(waitToBall){
        transition{
          if(!theFieldBall.ballWasSeen(ballNotSeenTimeOut))
            goto searchForBall;
          if(theFieldBall.positionRelative.squaredNorm() < sqr(ballNearThreshold)) 
            goto walkToBall;

        }
        action{
          theLookForwardSkill();
          theStandSkill();
        }
      }
      state(walkToBall)
    {
      transition
      {
        if(!theFieldBall.ballWasSeen(ballNotSeenTimeOut))
          goto searchForBall;
        else
          goto waitToBall;
      }

      action
      {
        theLookForwardSkill();
        theWalkToTargetSkill(Pose2f(walkSpeed, walkSpeed, walkSpeed), theFieldBall.positionRelative);
      }
    }
      state(searchForBall){
        transition{
          if(theFieldBall.ballWasSeen())
            goto turnToBall;
        }
        action{
          theLookForwardSkill();
          theWalkAtRelativeSpeedSkill(Pose2f(walkSpeed, 0.f, 0.f));
        }
      }
      //CRear los estados que sean necesarios, debe ser ciclico en la mayoria de casos.
    }
};
//Esto es lo que crea la carta
MAKE_CARD(KeeperClearBallCard)