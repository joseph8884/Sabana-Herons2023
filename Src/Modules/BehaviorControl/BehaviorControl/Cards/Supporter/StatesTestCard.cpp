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
#include "Representations\Communication\GameInfo.h"

CARD(StatesTestCard,
	{ ,
		CALLS(Activity),
		CALLS(SpecialAction),
		CALLS(LookForward),
		CALLS(Stand),

		REQUIRES(TeamBallModel),
		REQUIRES(RobotInfo),
		REQUIRES(RobotPose), 
		REQUIRES(FieldBall),
    REQUIRES(GameInfo),
		DEFINES_PARAMETERS(
		{,
			(int)(1000) initialWaitTime,
  }),
});

class StatesTestCard: public StatesTestCardBase{
   bool preconditions() const override
    {
        return theRobotInfo.number == 3;
    }
   bool postconditions() const override
    {
        return theRobotInfo.number != 3;
    }
    
    option{
			theActivitySkill(BehaviorStatus::StatesTestCard);
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
				if (theGameInfo.gamePhase !=  GAME_PHASE_PENALTYSHOOT ) {
					goto STAY;
}
		}
		action
		{
				if(theGameInfo.gamePhase == GAME_PHASE_PENALTYSHOOT)
				{
					if(theRobotInfo.number == 3)
					theSpecialActionSkill(SpecialActionRequest::rightArm);
				}
				else
				{
					theStandSkill();

			}
		}
	}
	state(STAY)
	{
		transition
		{
				if (theGameInfo.gamePhase == GAME_PHASE_PENALTYSHOOT) {
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
MAKE_CARD(StatesTestCard);
