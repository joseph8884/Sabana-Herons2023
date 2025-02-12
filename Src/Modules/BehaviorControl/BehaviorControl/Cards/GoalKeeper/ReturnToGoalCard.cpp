#include "Representations/BehaviorControl/FieldBall.h"
#include "Representations/BehaviorControl/Skills.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Modeling/RobotPose.h"
#include "Tools/BehaviorControl/Framework/Card/Card.h"
#include "Tools/BehaviorControl/Framework/Card/CabslCard.h"
#include "Representations/Modeling/TeamBallModel.h"
#include "Representations/Communication/RobotInfo.h"

CARD(ReturnToGoalCard,{,
    CALLS(Activity),//los llamados son para llamar todas las skills se hace por CALLS()
    CALLS(LookForward),
    REQUIRES(FieldBall),//Llama a los representaciones que necesita, siempre se usan estas por lo general
    REQUIRES(FieldDimensions),
    REQUIRES(RobotPose),
    REQUIRES(RobotInfo),
    REQUIRES(TeamBallModel),
    DEFINES_PARAMETERS( 
    //Aca son todas las variables que se quiera definir para despues usarse dentro de la clase
    {,//Mirar cuales sirven y cuales no
        //definir parametros
        (int)(1000) initialWaitTime,
    }),
}); 

class ReturnToGoalCard : public ReturnToGoalCardBase
{
   bool preconditions() const override
    {
        return theRobotInfo.number == 2;
    }
   bool postconditions() const override
    {
        return theRobotInfo.number != 2;
    }

    option{
      theActivitySkill(BehaviorStatus::ReturnToGoalCard);
      initial_state(start){
        transition{
          if(state_time > initialWaitTime)
            goto siguienteBloque;//Aca es el cambio de escena
        }
        action{
          theLookForwardSkill();
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
MAKE_CARD(ReturnToGoalCard)