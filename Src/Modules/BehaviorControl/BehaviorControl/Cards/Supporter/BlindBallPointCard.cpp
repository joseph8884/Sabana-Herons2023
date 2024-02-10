#include "Representations/BehaviorControl/Skills.h"
#include "Tools/BehaviorControl/Framework/Card/Card.h"
#include "Tools/BehaviorControl/Framework/Card/CabslCard.h"
#include "Representations/Modeling/TeamBallModel.h"
#include "Representations/MotionControl/SpecialActionRequest.h"
#include "Representations/Communication/GameInfo.h"
#include "Representations/Communication/RobotInfo.h"
#include "Representations/Communication/TeamInfo.h"
#include "Representations/Modeling/RobotPose.h"
#include "Representations/BehaviorControl/FieldBall.h"

#include <string>

CARD(BlindBallPointCard,
	{ ,
		CALLS(Activity),
		CALLS(SpecialAction),
		CALLS(LookForward),

		REQUIRES(TeamBallModel),
		REQUIRES(RobotInfo),
		REQUIRES(GameInfo),
		REQUIRES(RobotPose), 
		REQUIRES(FieldBall),
		DEFINES_PARAMETERS(
		{,
			(int)(1000) initialWaitTime
  }),
});

class BlindBallPointCard: public BlindBallPointCardBase{
   bool preconditions() const override
    {
        return theRobotInfo.number == 3;
    }
   bool postconditions() const override
    {
        return theRobotInfo.number != 3;
    }
    
    option{
			theActivitySkill(BehaviorStatus::BlindBallPointCard);
		initial_state(start)
  {
    transition
    {
      if(state_time > 1000)
        goto POINT;
		}
			action
		{
			theLookForwardSkill();
			theSpecialActionSkill(SpecialActionRequest::standHigh);
	}
      
    }
	state(POINT)
	{
		transition
		{
				if (!theFieldBall.ballWasSeen(1000)) {
					goto start;
}
		}
		action
		{
				if(theTeamBallModel.position.y() > 0)
				{
					theSpecialActionSkill(SpecialActionRequest::rightArm);
				}
				else
				{
					theSpecialActionSkill(SpecialActionRequest::leftArm);
			}
		}
	}
}
}; 
MAKE_CARD(BlindBallPointCard);
