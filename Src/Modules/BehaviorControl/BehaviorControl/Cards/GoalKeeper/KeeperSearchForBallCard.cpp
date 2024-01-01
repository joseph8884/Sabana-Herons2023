#include "Representations/BehaviorControl/FieldBall.h"
#include "Representations/BehaviorControl/Skills.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Modeling/RobotPose.h"
#include "Tools/BehaviorControl/Framework/Card/Card.h"
#include "Tools/BehaviorControl/Framework/Card/CabslCard.h"
#include "Tools/Math/BHMath.h"
CARD(KeeperSearchForBallCard,{,
    CALLS(Activity),//los llamados son para llamar todas las skills
    CALLS(InWalkKick),
    CALLS(LookForward),
    CALLS(Stand),
    CALLS(WalkAtRelativeSpeed),
    CALLS(WalkToTarget),
    REQUIRES(FieldBall),//Llama a los representaciones que necesita
    REQUIRES(FieldDimensions),
    REQUIRES(RobotPose),
    DEFINES_PARAMETERS( //Aca son todas las variables que se quiera definir para despues usarse dentro de la clase
    {,//Mirar cuales sirven y cuales no
        (float)(0.3f) walkSpeed,  // este valor estaba en 0.8
        (int)(1000) initialWaitTime,
        (int)(7000) ballNotSeenTimeout,
        (Angle)(5_deg) ballAlignThreshold,
        (float)(500.f) ballNearThreshold,
        (float)(1500.f) ballNearToMeThreshold,
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

    option{
      theActivitySkill(BehaviorStatus::KeeperSearchForBallCard);
      initial_state(start){//inicializa el estado a buscar el balon
        transition{
          if(state_time > initialWaitTime)
            goto turnToBall;
        }
        action{
          theLookForwardSkill();
          theStandSkill();
        }
      }
      //Siguiente estado, darse cuenta que van como en escalera.
      state(turnToBall){
        transition{
          if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))
            goto searchForBall;
          if(std::abs(theFieldBall.positionRelative.angle()) < ballAlignThreshold)
            goto waitToBall;
        }
        action{
          theLookForwardSkill();
          theWalkToTargetSkill(Pose2f(walkSpeed, walkSpeed, walkSpeed), Pose2f(theFieldBall.positionRelative.angle(), 0.f, 0.f));
        }
      }
      state(waitToBall){
        transition{
          if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))//Hacer la condicion de que vaya a caminar solo si esta dentro del area.
            goto searchForBall;
          if(theFieldBall.positionRelative.squaredNorm() < sqr(ballNearToMeThreshold))//crear la condicion para saber si el balon esta cerca o no, si esta cerca que vaya a por ella, si no que espere
            goto walkToBall;
        }
        action{
          theLookForwardSkill();
          theStandSkill();
        }
      }
      state(walkToBall){
        transition{
          if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))
            goto searchForBall;
          if(theFieldBall.positionRelative.squaredNorm() < sqr(ballNearThreshold))
            goto alignToGoal;
        }
        action{
          theLookForwardSkill();
          theWalkToTargetSkill(Pose2f(walkSpeed, walkSpeed, walkSpeed), theFieldBall.positionRelative);
        }
      }
      state(alignToGoal){
        const Angle angleToGoal = calcAngleToGoal();
        transition{
          if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))
            goto searchForBall;
          if(std::abs(angleToGoal) < angleToGoalThreshold && std::abs(theFieldBall.positionRelative.y()) < ballYThreshold)
            goto alignBehindBall;
        }
        action{
          theLookForwardSkill();
          theWalkToTargetSkill(Pose2f(walkSpeed, walkSpeed, walkSpeed), Pose2f(angleToGoal, theFieldBall.positionRelative.x() - ballAlignOffsetX, theFieldBall.positionRelative.y()));
        }
      }
      state(alignBehindBall){
        const Angle angleToGoal = calcAngleToGoal();
        transition{
          if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))
            goto searchForBall;
          if(std::abs(angleToGoal) < angleToGoalThresholdPrecise && ballOffsetXRange.isInside(theFieldBall.positionRelative.x()) && ballOffsetYRange.isInside(theFieldBall.positionRelative.y()))
            goto kick;
        }
        action{
          theLookForwardSkill();
          theWalkToTargetSkill(Pose2f(walkSpeed, walkSpeed, walkSpeed), Pose2f(angleToGoal, theFieldBall.positionRelative.x() - ballOffsetX, theFieldBall.positionRelative.y() - ballOffsetY));
        }
      }
      state(kick){
        const Angle angleToGoal = calcAngleToGoal();
        transition{
          if(state_time > maxKickWaitTime || (state_time > minKickWaitTime && theInWalkKickSkill.isDone()))
            goto start;
        }
        action{
          theLookForwardSkill();
          theInWalkKickSkill(WalkKickVariant(WalkKicks::forward, Legs::left), Pose2f(angleToGoal, theFieldBall.positionRelative.x() - ballOffsetX, theFieldBall.positionRelative.y() - ballOffsetY));
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
    };
//metodo para calcular el area de gol
Angle calcAngleToGoal() const
  {
    return (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundline, 0.f)).angle();
  }
};
//Crea la carta
MAKE_CARD(KeeperSearchForBallCard);