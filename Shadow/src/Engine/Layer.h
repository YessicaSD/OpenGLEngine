#pragma once

#include "Engine/Core.h"
#include "Engine/Event/Event.h"

#include <string>

namespace Shadow
{
	class SHADOW_API Layer
	{
	public:
		Layer(const std::string& name = "Module");
		virtual ~Layer();

		virtual void OnEvent(Event& event) {};
		virtual void OnUpdate() {};

		const std::string& GetName() const { return debugName; };

	protected:
		std::string debugName;
	};

}