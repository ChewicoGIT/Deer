#pragma once

namespace Deer {
	class Timestep {
	public:
		Timestep(float time = 0.0f)
			: m_time(time) { }

		float getSeconds() const { return m_time; }
		float getMilliseconds() const { return m_time * 1000; }
	private:
		float m_time;
	};
}