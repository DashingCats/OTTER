/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

CMorphAnimator.cpp
Simple animator component for demonstrating morph target animation.

As a convention in NOU, we put "C" before a class name to signify
that we intend the class for use as a component with the ENTT framework.
*/

#include "CMorphAnimator.h"
#include "CMorphMeshRenderer.h"
#include "NOU/Mesh.h"

namespace nou
{
	CMorphAnimator::AnimData::AnimData()
	{
		frameTime = 1.0f;
	}

	CMorphAnimator::CMorphAnimator(Entity& owner)
	{
		m_owner = &owner;
		m_data = std::make_unique<AnimData>();
		m_timer = 0.0f;
		m_forwards = true;
	}

	void CMorphAnimator::Update(float deltaTime)
	{
		//TODO: Complete this function.
		if (m_data->frames.size() <= 2)
			return;

		float t;

		if (m_data->frameTime > 0.0f)
		{
			m_timer += deltaTime;

			if (m_timer > m_data->frameTime)
			{
				//m_forwards = !m_forwards;

				currentFrame++;
				nextFrame++;
					if (currentFrame > 7)
						currentFrame = 0;
					if (nextFrame > 7)
						nextFrame = 0;
				

			}

			m_timer = fmod(m_timer, m_data->frameTime);

			t = m_timer / m_data->frameTime;
		}
		else
		{
			t = 0.0f;
		}
			

		if (m_forwards)
			m_owner->Get<CMorphMeshRenderer>().UpdateData(*m_data->frames[currentFrame], *m_data->frames[nextFrame], t);
		else
			m_owner->Get<CMorphMeshRenderer>().UpdateData(*m_data->frames[nextFrame], *m_data->frames[currentFrame], t);
	}
	void CMorphAnimator::SetFrames(std::vector<std::unique_ptr<Mesh>>& frames)
	{
		for (int i = 0; i < frames.size(); i++)
		{
			m_data->frames.push_back(frames[i].get());
		}
	}
	void CMorphAnimator::SetFrameTime(float frameTime)
	{
		m_data->frameTime = frameTime;
	}

}