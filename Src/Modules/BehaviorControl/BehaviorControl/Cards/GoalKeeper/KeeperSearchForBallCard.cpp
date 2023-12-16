#include "Tools/BehaviorControl/Framework/Card/Card.h"
#include "Representations/BehaviorControl/Skills.h" //Estos son las importaciones necesarias para que las cartas se creen correctamente
CARD(KeeperSearchForBallCard,{,
    CALLS(Activity)//los llamados son para llamar todas las skills
    CALLS()
    REQUIRES(FieldBall),//Llama a los modulos que necesita
    REQUIRES(FieldDimensions),
    REQUIRES(RobotPose),
    DEFINES_PARAMETERS( //Aca son todas las variables que se quiera definir para despues usarse dentro de la clase
    {,//Mirar cuales sirven y cuales no
        (float)(0.3f) walkSpeed,  // este valor estaba en 0.8
        (int)(1000) initialWaitTime,
        (int)(7000) ballNotSeenTimeout,
        (Angle)(5_deg) ballAlignThreshold,
        (float)(500.f) ballNearThreshold,
        (Angle)(10_deg) angleToGoalThreshold,
        (float)(400.f) ballAlignOffsetX,
        (float)(100.f) ballYThreshold,
        (Angle)(2_deg) angleToGoalThresholdPrecise,
        (float)(150.f) ballOffsetX,
        (Rangef)({140.f, 170.f}) ballOffsetXRange,
        (float)(40.f) ballOffsetY,
        (Rangef)({20.f, 50.f}) ballOffsetYRange,
        (int)(10) minKickWaitTime,
        (int)(3000) maxKickWaitTime,
    }),
}); //Esto es lo que crea la carta


class KeeperSearchForBallCard : public KeeperSearchForBallCard
{
    bool preconditions() const override
    {
        return true;
    }

    bool postconditions() const override
    {
        return true;
    }

    option
    {
    theActivitySkill(BehaviorStatus::KeeperSearchForBallCard);

    initial_state(start)//inicializa el estado a buscar el balon
    {
      transition
      {
        if(state_time > initialWaitTime)
          goto turnToBall;
      }

      action
      {
        theLookForwardSkill();
        theStandSkill();
      }
    }

    //Siguiente estado, darse cuenta que van como en escalera.
    state(walkToBall)
    {
      transition
      {
        if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))//Hacer la condicion de que vaya a caminar solo si esta dentro del area.
          goto searchForBall;
        if(theFieldBall.positionRelative.squaredNorm() < sqr(ballNearThreshold))
          goto true; //Tiene que caminar hasta allá, mirar el siguiente estado
      }

      action
      {
        theLookForwardSkill();
        theWalkToTargetSkill(Pose2f(walkSpeed, walkSpeed, walkSpeed), theFieldBall.positionRelative);
      }
    }



};