#pragma once
	
namespace Shadow
{
	class Time
	{
	public:
		Time() {  };
		/// <summary>
		/// This return the last frame duration in seconds
		/// </summary>
		static float GetDeltaTime() { return deltaTime; };
		static float GetDeltaTimeMilliseconds() { return deltaTime * 1000.0f; };
		
	private:
		void CalculateDeltaTime();

		static float deltaTime;
		float lastFrameTime = 0.0;

		friend class Application;
	};
}