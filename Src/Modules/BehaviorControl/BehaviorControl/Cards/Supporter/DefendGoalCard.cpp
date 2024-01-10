#include "Representations/BehaviorControl/Skills.h"
#include "Tools/BehaviorControl/Framework/Card/Card.h"

CARD(DefendGoalCard,
{,
  CALLS(Activity),
  CALLS(LookForward),
  CALLS(Say),
  CALLS(Stand),
});

class DefendGoalCard : public CodeReleasePositionForKickOffCardBase
{
  bool preconditions() const override
  {
    return true;
  }

  bool postconditions() const override
  {
    return true;
  }

  void execute() override
  {
    theActivitySkill(BehaviorStatus::codeReleasePositionForKickOff);
    theLookForwardSkill();
    theStandSkill();
    // Not implemented in the Code Release.
    theSaySkill("Please implement a behavior for me!");
  }
};

MAKE_CARD(DefendGoalCard);
