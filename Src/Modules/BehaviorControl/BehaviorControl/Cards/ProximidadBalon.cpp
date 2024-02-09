#include "Representations/BehaviorControl/Skills.h"
#include "Tools/BehaviorControl/Framework/Card/Card.h"

CARD(ProximidadBalonCard,
{,
  CALLS(Activity),
  CALLS(LookForward),
  CALLS(Say),
  CALLS(Stand),
});

class ProximidadBalonCard : public DefendGoalCardBase
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

MAKE_CARD(ProximidadBalonCard);