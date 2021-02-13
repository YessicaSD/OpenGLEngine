#pragma once

#include "Core.h"
#include "swpch.h"
#include "Window.h"
#include "LayerManager.h"
#include "Event/Event.h"


NAMESPACE_BEGAN

	class SHADOW_API Application
	{
		public:
			Application();
			virtual ~Application();
			
			void Run();
			
			void OnEvent(Event& e);
			
			void PushLayer(Layer* layer);
			void PushOverlay(Layer* layer);

		private:
			bool running =true;
			std::unique_ptr<Window> window;
			LayerManager layerManager;
	};

	// This function is going to be defined by the CLIENT
	Application* CreateApplication();

	
NAMESPACE_END

