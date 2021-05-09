#pragma once
#include "Shadow/Event/Event.h"

#include <string>

namespace Shadow
{
	class SHADOW_API Layer
	{
	public:
		Layer(const std::string& name = "Module");
		virtual ~Layer();

		virtual void Init() {};
		virtual void OnEvent(Event& event) {};
		virtual void OnUpdate() {};
		virtual void OnAttach() {};
		virtual void OnMainTopBar() {};
		virtual void OnImGuiRender() {};
		virtual void OnDetach() {};

		const std::string& GetName() const { return debugName; };

	protected:
		std::string debugName;
	};

}