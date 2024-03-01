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
#include "Representations/Communication/TeamData.h"
#include "Representations/Infrastructure/FrameInfo.h"

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
		REQUIRES(TeamData),
		REQUIRES(FrameInfo),
		DEFINES_PARAMETERS(
		{,
			(int)(1000) initialWaitTime,
	}),
	});

class BlindBallPointCard : public BlindBallPointCardBase {
	bool preconditions() const override
	{
		return true;
	}
	bool postconditions() const override
	{
		return true;
	}

	option{
		theActivitySkill(BehaviorStatus::BlindBallPointCard);
	initial_state(start)
{
	transition
	{
		if (state_time > initialWaitTime)
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
			for (auto const& teammate : theTeamData.teammates) {
				if(teammate.isPenalized){
					goto STAY;
				}
			}
	}
		action
		{
			for (auto const& teammate : theTeamData.teammates) {
				if (teammate.isPenalized) {
					if (teammate.number == 1) {
						if (theRobotInfo.number == 3) {
							theSpecialActionSkill(SpecialActionRequest::rightArm);
						}
					}
					else if (teammate.number == 2) {
						if (theRobotInfo.number == 3) {
							theSpecialActionSkill(SpecialActionRequest::leftArm);
						}
					}
				} else {
					theLookForwardSkill();
					theStandSkill();
				}
			}
		}
	}
	state(STAY)
	{
		transition
		{
			for (auto const& teammate : theTeamData.teammates) {
				if (teammate.isPenalized) {
					goto POINT;
				}
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
