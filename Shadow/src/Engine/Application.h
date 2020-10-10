#pragma once

#include "Core.h"

namespace Shadow
{
	class SHADOW_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// This function is going to be defined by the CLIENT
	Application* CreateApplication();

	
}

