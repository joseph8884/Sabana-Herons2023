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
#include <random>



CARD(FuerzaPatadaCard,
{,
  CALLS(Activity),
  CALLS(InWalkKick),
  CALLS(LookForward),
  CALLS(Stand),
  CALLS(Kick),
  CALLS(Say),
  CALLS(WalkAtRelativeSpeed),
  CALLS(WalkToTarget),
  REQUIRES(FieldBall),
  REQUIRES(FieldDimensions),
  REQUIRES(RobotPose),
  DEFINES_PARAMETERS(
  {,
    (float)(0.8f) walkSpeed,
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
});


class FuerzaPatadaCard : public FuerzaPatadaCardBase
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
        theActivitySkill(BehaviorStatus::fuerzaPatada);
    
        initial_state(start)
        {
        
          transition
          {
             if(state_time > initialWaitTime)
                goto kick;
          }
          action
          {
              theLookForwardSkill();
              theStandSkill();
              theSaySkill("Clear!");
          }
        }


        state(kick)
        {

            transition
            {
                if(state_time > maxKickWaitTime || (state_time > minKickWaitTime && theKickSkill.isDone()))
                goto start;
            }

            action
            {
                theLookForwardSkill();
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(1, 100);
                int randomNumber = dis(gen);

                if (randomNumber % 2 == 0) {
                    theKickSkill((KickRequest::kickForward), true, 0.1f, false);
                } else {
                    theKickSkill((KickRequest::kickForward), true, 0.2f, false);
                }
                            
                    theSaySkill("Go Go");
            }
        }
    }
    
    
};

MAKE_CARD(FuerzaPatadaCard);
