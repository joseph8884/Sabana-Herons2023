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


CARD(BlindBallPointCard,
	{ ,
		CALLS(Activity),
		CALLS(SpecialAction),
		CALLS(LookForward),
		CALLS(Stand),

		REQUIRES(TeamBallModel),
		REQUIRES(RobotInfo),
		REQUIRES(GameInfo),
		REQUIRES(RobotPose), 
		REQUIRES(FieldBall),
		DEFINES_PARAMETERS(
		{,
			(int)(1000) initialWaitTime,
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
      if(state_time > initialWaitTime)
        goto POINT;
		}
			action
		{
			theLookForwardSkill();
			theStandSkill();
		}
      
    }
	state(POINT)
	{
		transition
		{
				if (!theFieldBall.ballWasSeen(1000)) {
					goto STAY;
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
	state(STAY)
	{
		transition
		{
				if (theFieldBall.ballWasSeen(1000)) {
					goto POINT;
}
		}
			action
		{
			theLookForwardSkill();
			theStandSkill();
		}
	}
}
}; 
MAKE_CARD(BlindBallPointCard);
