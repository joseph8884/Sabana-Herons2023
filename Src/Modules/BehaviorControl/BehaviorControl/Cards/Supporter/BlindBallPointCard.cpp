#include "Representations/BehaviorControl/Skills.h"
#include "Tools/BehaviorControl/Framework/Card/Card.h"
#include "Tools/BehaviorControl/Framework/Card/CabslCard.h"
#include "Representations/Modeling/TeamBallModel.h"
#include "Representations/MotionControl/SpecialActionRequest.h"
#include "Representations/Communication/GameInfo.h"
#include "Representations/Communication/RobotInfo.h"
#include "Representations/Communication/TeamInfo.h"

#include <string>

CARD(BlindBallPointCard,
	{ ,
		CALLS(Activity),
		CALLS(SpecialAction),

		REQUIRES(TeamBallModel),
		REQUIRES(SpecialActionRequest),
		REQUIRES(RobotInfo),
		REQUIRES(GameInfo),
		REQUIRES(OwnTeamInfo),
		DEFINES_PARAMETERS(
		{,
			(int)(1000) initialWaitTime = 1000
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
			theActivitySkill(BehaviorStatus::BlindBallPoint);
       initial_state(start)
  {
    transition
    {
      if(state_time > 20000)
        goto POINT;
		}
			action
		{
			Skills::LookForwardSkill();
			theSpecialActionSkill(SpecialActionRequest::standHigh);
	}
      
    }
	state(POINT)
	{
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
