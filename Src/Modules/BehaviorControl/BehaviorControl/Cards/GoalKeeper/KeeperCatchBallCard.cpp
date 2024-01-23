#include "Representations/BehaviorControl/FieldBall.h"
#include "Representations/BehaviorControl/Skills.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Modeling/RobotPose.h"
#include "Tools/BehaviorControl/Framework/Card/Card.h"
#include "Tools/BehaviorControl/Framework/Card/CabslCard.h"

CARD(KeeperCatchBallCard,{,
    CALLS(Activity),//los llamados son para llamar todas las skills se hace por CALLS()
    CALLS(LookForward),
    CALLS(KeyFrameArms),
    CALLS(Stand),
    REQUIRES(FieldBall),//Llama a los representaciones que necesita, siempre se usan estas por lo general
    REQUIRES(FieldDimensions),
    REQUIRES(RobotPose),
    DEFINES_PARAMETERS( 
    //Aca son todas las variables que se quiera definir para despues usarse dentro de la clase
    {,//Mirar cuales sirven y cuales no
        //definir parametros
        (int)(1000) initialWaitTime,
    }),
}); 

class KeeperCatchBallCard : public KeeperCatchBallCardBase
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
      theActivitySkill(BehaviorStatus::unknown);
      initial_state(start){
        transition{
          if(state_time > initialWaitTime)
            goto siguienteBloque;//Aca es el cambio de escena
        }
        action{
          theLookForwardSkill();
          theKeyFrameArmsSkill(ArmKeyFrameRequest::ArmKeyFrameId::back,false);
          theStandSkill();
          //Lo que quieres que haga cuando la se llegue a este estado
        }
      }
      state(siguienteBloque){
        transition{
            if(true)//Condicion para que pase al siguiente bloque
              goto start;  //siguiente bloque
        }
        action{
          theLookForwardSkill();
        }
      }
      //CRear los estados que sean necesarios, debe ser ciclico en la mayoria de casos.
    }
};
//Esto es lo que crea la carta
MAKE_CARD(KeeperCatchBallCard)