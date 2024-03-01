#include "Representations/BehaviorControl/FieldBall.h"
#include "Representations/BehaviorControl/Skills.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Modeling/RobotPose.h"
#include "Tools/BehaviorControl/Framework/Card/Card.h"
#include "Tools/BehaviorControl/Framework/Card/CabslCard.h"

CARD(ProximidadBalonCard,{,
    CALLS(Activity),//los llamados son para llamar todas las skills se hace por CALLS()
    CALLS(LookForward),
    CALLS(KeyFrameSingleArm),
    CALLS(Stand),
    CALLS(WalkAtRelativeSpeed),
    CALLS(SpecialAction),
    REQUIRES(FieldBall),//Llama a los representaciones que necesita, siempre se usan estas por lo general
    REQUIRES(FieldDimensions),
    REQUIRES(RobotPose),
    DEFINES_PARAMETERS( 
    //Aca son todas las variables que se quiera definir para despues usarse dentro de la clase
    {,//Mirar cuales sirven y cuales no
        //definir parametros
        (int)(1000) initialWaitTime,
        (Angle)(5_deg) ballAlignThreshold,
        (int)(7000) ballNotSeenTimeout,
        (float)(0.3f) walkSpeed,
        (float)(500.f) ballNearToMeThreshold,
    }),
}); 

class ProximidadBalonCard : public ProximidadBalonCardBase
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
      theActivitySkill(BehaviorStatus::ProximidadBalonCard);
      initial_state(start){
        transition{
          if(state_time > initialWaitTime)
            goto waitToBall;//Aca es el cambio de escenads
        }
        action{
          theStandSkill();
          //Lo que quieres que haga cuando la se llegue a este estado
        }
      }
      state(searchForBall)
    {
      transition
      {
        if(theFieldBall.ballWasSeen())
          goto turnToBall;
      }

      action
      {
        theWalkAtRelativeSpeedSkill(Pose2f(walkSpeed, 0.f, 0.f));
      }
    }
      state(turnToBall)
    {
      transition
      {
        if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))
          goto searchForBall;
        if(std::abs(theFieldBall.positionRelative.angle()) < ballAlignThreshold)
          goto waitToBall;
      }

      action
      {
        theLookForwardSkill();
      }
    }
    state(waitToBall){
        transition{
          if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))//Hacer la condicion de que vaya a caminar solo si esta dentro del area.
            goto start;
          if(theFieldBall.positionRelative.squaredNorm() < sqr(ballNearToMeThreshold))//crear la condicion para saber si el balon esta cerca o no, si esta cerca que vaya a por ella, si no que espere
            goto raiseHand;
        }
        action{
                    theStandSkill();
        }
      }
      state(raiseHand){
        transition{
            if(theFieldBall.positionRelative.squaredNorm() > sqr(ballNearToMeThreshold))//crear la condicion para saber si el balon esta cerca o no, si esta cerca que vaya a por ella, si no que espere
              goto waitToBall;
            if(!theFieldBall.ballWasSeen(2000))//Hacer la condicion de que vaya a caminar solo si esta dentro del area.
              goto start;
        }
        action{
          theSpecialActionSkill(SpecialActionRequest::rightArm);
        }
      }
      //CRear los estados que sean necesarios, debe ser ciclico en la mayoria de casos.
    }
};
//Esto es lo que crea la carta
MAKE_CARD(ProximidadBalonCard)