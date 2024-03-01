#include "Representations/BehaviorControl/FieldBall.h"
#include "Representations/BehaviorControl/Skills.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Modeling/RobotPose.h"
#include "Tools/BehaviorControl/Framework/Card/Card.h"
#include "Tools/BehaviorControl/Framework/Card/CabslCard.h"
#include "Tools/Math/BHMath.h"
#include "Representations/Communication/RobotInfo.h"
#include "Tools/Modeling/Obstacle.h"
#include "Representations/Modeling/ObstacleModel.h"
#include "Representations/BehaviorControl/Libraries/LibCheck.h"


CARD(DribbleToGoalCard,{,
   CALLS(Activity),
  CALLS(InWalkKick),
  CALLS(LookForward),
  CALLS(Stand),
  CALLS(WalkAtRelativeSpeed),
  CALLS(WalkToTarget),
  CALLS(Kick),
  CALLS(Say),
  CALLS(PathToTarget),
  CALLS(LookAtAngles),
  CALLS(KeyFrameArms),
  CALLS(SpecialAction),
  REQUIRES(FieldBall),
  REQUIRES(FieldDimensions),
  REQUIRES(RobotPose),
  REQUIRES(RobotInfo),
  REQUIRES(ObstacleModel),
  REQUIRES(LibCheck),
  
  DEFINES_PARAMETERS(
  {,
    (float)(0.5f) walkSpeed,
    (int)(1000) initialWaitTime,
    (int)(6000) ballNotSeenTimeout,
    (Angle)(5_deg) ballAlignThreshold,
    (float)(400.f) ballNearThreshold,
    (Angle)(10_deg) angleToGoalThreshold,
    (float)(400.f) ballAlignOffsetX,
    (float)(100.f) ballYThreshold,
    (Angle)(2_deg) angleToGoalThresholdPrecise,
    (float)(150.f) ballOffsetX,
    (Rangef)({140.f, 190.f}) ballOffsetXRange,
    (float)(40.f) ballOffsetY,
    (Rangef)({20.f, 50.f}) ballOffsetYRange,
    (int)(10) minKickWaitTime,
    (int)(3000) maxKickWaitTime,
    (Pose2f)(Pose2f(0,2000,0)) StrikerPos,
  }),
});

class DribbleToGoalCard : public DribbleToGoalCardBase
{
  bool preconditions() const override
  {
    return true;/*theRobotInfo.number == 4;*/
  }

  bool postconditions() const override
  {
    return true; /*theRobotInfo.number != 4;*/
  }

  option
  {
    theActivitySkill(BehaviorStatus::DribbleToGoalCard);

    bool hayObstaculoCerca = false;
      if(!theObstacleModel.obstacles.empty()){     //Tenemos obstàculos, entonces, actuamos.   
      for(const auto& obstacle : theObstacleModel.obstacles){
        //See if the obstacle is first than the target   
      if (obstacle.center.norm()<200.f)  
          hayObstaculoCerca=true;
      }
    }
    
    initial_state(start)
    {
      transition
      {

        if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))
          goto giraCabezaDer;
        if(state_time > initialWaitTime)
          goto turnToBall;
      }

      action
      {
        theLookForwardSkill();
        theStandSkill();
      }
    }

    state(turnToBall)
    {
      transition
      {
        // if(theLibCheck.closerToTheBall == 3)
        //   goto receiveLeftPass;
        // if(theLibCheck.closerToTheBall == 5)
        //   goto receiveRightPass;
        // if(theLibCheck.closerToTheBall == 2)
        //   goto receiveCentralPass;
        if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))
          goto giraCabezaDer;
        if(theFieldBall.positionRelative.norm() < 5000.0f)
          goto walkToBall;
        if(hayObstaculoCerca && theFieldBall.positionOnField.norm() > 500.f)
          goto obsAvoid;
      }
      action 
      {
        theLookForwardSkill();
        theWalkToTargetSkill(Pose2f(walkSpeed, walkSpeed, walkSpeed), Pose2f(theFieldBall.positionRelative.angle(), 0.f, 0.f));
      }
    }

    state(giraCabezaDer)
    {
      transition
      {
        if(!theFieldBall.ballWasSeen(ballNotSeenTimeout) && state_time > 2000)
          goto giraCabezaIzq;
        // if(theLibCheck.closerToTheBall == 3 && theLibCheck.LeftAttacking)
        //   goto receiveLeftPass;
        // if(theLibCheck.closerToTheBall == 5 && theLibCheck.RightAttacking)
        //   goto receiveRightPass;
        // if(theLibCheck.closerToTheBall == 2)
        //   goto receiveCentralPass;
        if(theFieldBall.ballWasSeen())
          goto turnToBall;
        if(hayObstaculoCerca && theFieldBall.positionOnField.norm() > 500.f)
          goto obsAvoid;
      }
      action
      {
        theWalkAtRelativeSpeedSkill(Pose2f(walkSpeed, 0.f, 0.f));
        theLookAtAnglesSkill(-1,2,1.f);
      }
    }

    state(giraCabezaIzq)
    {
      transition
      {
        if(!theFieldBall.ballWasSeen(ballNotSeenTimeout) && state_time > 2000)
          goto giraCabezaDer;
        // if(theLibCheck.closerToTheBall == 3 && theLibCheck.LeftAttacking)
        //   goto receiveLeftPass;
        // if(theLibCheck.closerToTheBall == 5 && theLibCheck.RightAttacking)
        //   goto receiveRightPass;
        // if(theLibCheck.closerToTheBall == 2)
        //   goto receiveCentralPass;
        if(theFieldBall.ballWasSeen())
          goto turnToBall;
        if(hayObstaculoCerca && theFieldBall.positionOnField.norm() > 500.f)
          goto obsAvoid;
      }
      action
      {
        theWalkAtRelativeSpeedSkill(Pose2f(walkSpeed, 0.f, 0.f));
        theLookAtAnglesSkill(1,2,1.f);
      }
    }

    state(walkToBall)
    {
      transition
      {
        // if(theLibCheck.closerToTheBall == 3 && theLibCheck.LeftAttacking)
        //   goto receiveLeftPass;
        // if(theLibCheck.closerToTheBall == 5 && theLibCheck.RightAttacking)
        //   goto receiveRightPass;
        // if(theLibCheck.closerToTheBall == 2)
        //   goto receiveCentralPass;
        if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))
          goto giraCabezaDer;
        if(theFieldBall.positionRelative.squaredNorm() < sqr(ballNearThreshold))
          goto alignBehindBall;
        if(hayObstaculoCerca && theFieldBall.positionOnField.norm() > 500.f)
          goto obsAvoid;
      }
      action
      {
        theLookForwardSkill();
        theKeyFrameArmsSkill(ArmKeyFrameRequest::back,false);
        theWalkToTargetSkill(Pose2f(walkSpeed, walkSpeed, walkSpeed), theFieldBall.positionRelative);
      }
    }
    

    


/*
    state(alignToGoal)
    {
      const Angle angleToGoal = calcAngleToGoal();
      bool hayObstaculos = hayObstaculo();
      int random = randomNum();

      transition
      {
        if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))
          goto giraCabezaDer;
        /*if(std::abs(angleToGoal) < angleToGoalThreshold && std::abs(theFieldBall.positionRelative.y()) < ballYThreshold && hayObstaculos && random == 0)
          //goto alignRight;
        if(std::abs(angleToGoal) < angleToGoalThreshold && std::abs(theFieldBall.positionRelative.y()) < ballYThreshold && hayObstaculos && random == 1)
          goto alignLeft;
        if(std::abs(angleToGoal) < angleToGoalThresholdPrecise && std::abs(theFieldBall.positionRelative.y()) < ballYThreshold )
          goto alignBehindBall;
      }
      action
      {
        theLookForwardSkill();
        theWalkToTargetSkill(Pose2f(walkSpeed + 0.2f, walkSpeed + 0.2f, walkSpeed + 0.2f), Pose2f(angleToGoal, theFieldBall.positionRelative.x() - ballAlignOffsetX, theFieldBall.positionRelative.y()));
      }
    }
*/ 

    state(alignBehindBall)
    {
      

      transition
      {
        if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))
          goto giraCabezaDer;
        if(ballOffsetXRange.isInside(theFieldBall.positionRelative.x()) && ballOffsetYRange.isInside(theFieldBall.positionRelative.y()))
          goto kickAtGoal; 
        if(hayObstaculoCerca && theFieldBall.positionOnField.norm() > 500.f)
          goto obsAvoid;
      }
      action
      {
        theLookForwardSkill();
        theKeyFrameArmsSkill(ArmKeyFrameRequest::back,false);
        theWalkToTargetSkill(Pose2f(walkSpeed, walkSpeed, walkSpeed), theFieldBall.positionRelative);
      }
    }


    state(kickAtGoal)
    {
      bool hayObstaculos = hayObstaculo();
      const Angle angleToGoal = calcAngleToGoal();
      transition
      {
        if(state_time > maxKickWaitTime || (state_time > minKickWaitTime && theInWalkKickSkill.isDone()))
          goto start;
        if(!theFieldBall.ballWasSeen(ballNotSeenTimeout))
          goto giraCabezaDer;
        
      }
      action
      {
        theLookForwardSkill();
        theKeyFrameArmsSkill(ArmKeyFrameRequest::back,false);
        /*theKickSkill((KickRequest::kickForward), true,0.2f, false);*/
        theInWalkKickSkill(WalkKickVariant(WalkKicks::Type::forward, Legs::Leg::right),Pose2f(0.f, theFieldBall.positionRelative.x() - ballOffsetX-25.f, theFieldBall.positionRelative.y() - ballOffsetY));
      }
    }
    state(obsAvoid)
    {  
      transition 
      {
        if(theFieldBall.ballWasSeen())
          goto turnToBall;
        if(state_time > maxKickWaitTime || (state_time > minKickWaitTime && theInWalkKickSkill.isDone()))
          goto start;
      }
      action
      {
        for(const auto& obstacle : theObstacleModel.obstacles) {
          theWalkToTargetSkill(Pose2f(walkSpeed, walkSpeed, walkSpeed), Pose2f(0.f, 0.f, obstacle.center.norm()+300)); 
        }
      }
    }



        // state(receiveCentralPass)
    // {
    //   const Angle angleToPass = calcAngleToCentralPass();

    //   transition
    //   {
    //     if(hayObstaculoCerca)
    //       goto obsAvoid;
    //     if(theLibCheck.closerToTheBall == 3)
    //       goto receiveLeftPass;
    //     if(theLibCheck.closerToTheBall == 5)
    //       goto receiveRightPass;
    //     if(theRobotPose.translation.x() >= 1500 && theRobotPose.translation.x() < 2000 && theRobotPose.translation.y() >= 500 && theRobotPose.translation.y() < 1500)
    //       goto waitAtCentralPass;
    //     if(theFieldBall.positionRelative.norm() > 500.f && theFieldBall.positionOnField.x() > theFieldDimensions.xPosHalfWayLine)
    //       goto walkToBall;
    //   }
    //   action
    //   {
    //     theLookForwardSkill();
    //     thePathToTargetSkill(walkSpeed + 0.3f,Pose2f(angleToPass, 0.f, 2000.f));
    //   }
    // }

    // state(waitAtCentralPass)
    // {
    //   const Angle angleToPass = calcAngleToCentralPass();

    //   transition
    //   {
    //     if(theFieldBall.positionRelative.norm() < 1500.f && theFieldBall.positionOnField.x() > theFieldDimensions.xPosHalfWayLine && ((!theLibCheck.LeftAttacking && !theLibCheck.RightAttacking)))
    //       goto walkToBall;
    //     if(!theFieldBall.ballWasSeen(8000))
    //       goto giraCabezaDer;
    //   }
    //   action
    //   {
    //     theLookForwardSkill();
    //     theWalkToTargetSkill(Pose2f(walkSpeed, walkSpeed, walkSpeed), Pose2f(angleToPass, 0.f, 0.f));
    //   }
    // }

  }

  Angle calcAngleToGoal() const
  {
    return (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundline, 0.f)).angle();
  }

  Angle calcAngleToPass() const
  {
    return (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentPenaltyMark, theFieldDimensions.yPosCenterGoal)).angle();
  }

  Angle calcAngleToCentralPass() const
  {
    return (theRobotPose.inversePose * Vector2f(100.f, 100.f)).angle();
  }

  bool hayObstaculo()
  {
    bool x = false;
    if(!theObstacleModel.obstacles.empty()){     //Tenemos obstàculos, entonces, actuamos.   
      for(const auto& obstacle : theObstacleModel.obstacles){
        //See if the obstacle is first than the target   
      if(obstacle.center.norm() < 500.f && (obstacle.center.y() < 150 && obstacle.center.y() > -150))
        x = true;
      }
    }
    return x;
  } 

  int randomNum() 
  {
    int random = 0;
    random = rand()%(2 + 0);
    if(theRobotPose.translation.y() > (theFieldDimensions.yPosLeftFieldBorder - 1500.f))
      random = 0;
    if(theRobotPose.translation.y() < (theFieldDimensions.yPosRightFieldBorder + 1500.f))
      random = 1;

    return random;
  } 

};

//Esto es lo que crea la carta
MAKE_CARD(DribbleToGoalCard)