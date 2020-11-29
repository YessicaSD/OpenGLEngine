#pragma once

#include "Core.h"
#include "swpch.h"
#include "Window.h"

NAMESPACE_BEGAN

	class SHADOW_API Application
	{
		public:
			Application();
			virtual ~Application();
			void Run();

		private:
			std::unique_ptr<Window> window;

	};

	// This function is going to be defined by the CLIENT
	Application* CreateApplication();

	
NAMESPACE_END

