#include "Representations/BehaviorControl/FieldBall.h"
#include "Representations/BehaviorControl/Skills.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Modeling/RobotPose.h"
#include "Tools/BehaviorControl/Framework/Card/Card.h"
#include "Tools/BehaviorControl/Framework/Card/CabslCard.h"

CARD(DribbleToGoalCard,{,
    CALLS(Activity),//los llamados son para llamar todas las skills se hace por CALLS()

    REQUIRES(FieldBall),//Llama a los representaciones que necesita, siempre se usan estas por lo general
    REQUIRES(FieldDimensions),
    REQUIRES(RobotPose),
    DEFINES_PARAMETERS( 
    //Aca son todas las variables que se quiera definir para despues usarse dentro de la clase
    {,//Mirar cuales sirven y cuales no
        //definir parametros
    }),
}); 

class DribbleToGoalCard : public DribbleToGoalCardBase
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
            //Aca es el cambio de escena
        }
        action{
          
          //Lo que quieres que haga cuando la se llegue a este estado
        }
      }
      state(){
        transition{
            if()//Condicion para que pase al siguiente bloque
                //siguiente bloque
        }
        action{
        
        }
      }
      //CRear los estados que sean necesarios, debe ser ciclico en la mayoria de casos.
    }
}
//Esto es lo que crea la carta
MAKE_CARD(DribbleToGoalCard)