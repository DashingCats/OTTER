/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CKnightFSM.h
Simple FSM component for our animated knight.
*/

#include "CWarriorFSM.h"
#include "Sprites/CSpriteAnimator.h"

namespace nou
{
	//String labels for animations.
	const std::string CWarriorFSM::idleClip = "idle";
	const std::string CWarriorFSM::runClip = "walk";
	const std::string CWarriorFSM::attackClip = "attack";

	CWarriorFSM::CWarriorFSM(Entity& owner)
	{
		m_owner = &owner;
		m_state = AnimState::IDLE;

		SetState(m_state);
	}

	void CWarriorFSM::SetState(CWarriorFSM::AnimState state)
	{
		m_state = state;

		//Gets the animator from the entity.
		auto& animator = m_owner->Get<CSpriteAnimator>();

		ClearTriggers();

		//TODO: Complete this function.
		switch (m_state)
		{
		case AnimState::IDLE:
			animator.PlayLoop(idleClip);
			break;
		case AnimState::RUN:
			animator.PlayLoop(runClip);
			break;
		default:
			animator.PlayOnce(attackClip);
			break;
		}
	}


	void CWarriorFSM::Update()
	{
		auto& animator = m_owner->Get<CSpriteAnimator>();

		//TODO: Complete this function.
		switch (m_state)
		{
		case AnimState::IDLE:
			if (GetVariable("move"))
				SetState(AnimState::RUN);
		case AnimState::RUN:
			if (!GetVariable("move"))
				SetState(AnimState::IDLE);
		case AnimState::ATTACK:
			if (GetVariable("fight"))
				SetState(AnimState::ATTACK);

		}
	}
}