#include "swpch.h"
#include "LayerMaterialEditor.h"

#include <imgui.h>
#include <imgui_node_editor.h>

NAMESPACE_BEGAN

namespace ed = ax::NodeEditor;

static ed::EditorContext* g_Context = nullptr;

struct LinkInfo
{
	ed::LinkId Id;
	ed::PinId  InputId;
	ed::PinId  OutputId;
};


MaterialEditor::MaterialEditor()
{
	g_Context = ed::CreateEditor();
}

MaterialEditor::~MaterialEditor()
{
	ed::DestroyEditor(g_Context);
}

void MaterialEditor::OnImGuiRender()
{
	ed::SetCurrentEditor(g_Context);

	ed::Begin("My Editor");

	int uniqueId = 35;
	static ImVector<LinkInfo>   g_Links;                // List of live links. It is dynamic unless you want to create read-only view over nodes.
	static int                  g_NextLinkId = 100;     // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.

	 //Start drawing nodes.
	ed::BeginNode(uniqueId++);
	ImGui::Text("Node A");
	ed::BeginPin(uniqueId++, ed::PinKind::Input);
	ImGui::Text("-> In");

	ed::EndPin();
	ImGui::SameLine();
	ed::BeginPin(uniqueId++, ed::PinKind::Output);
	ImGui::Text("Out ->");
	ed::EndPin();
	ImGui::PushItemWidth(100);
	static float color[3] = { 0,0,0 };
	ImGui::ColorPicker3("Colors", color);
	ImGui::PopItemWidth();
	ed::EndNode();

	// Submit Node B
	ed::NodeId nodeB_Id = uniqueId++;
	ed::PinId  nodeB_InputPinId1 = uniqueId++;
	ed::PinId  nodeB_InputPinId2 = uniqueId++;
	ed::PinId  nodeB_OutputPinId = uniqueId++;


	ed::BeginNode(nodeB_Id);
	ImGui::Text("PBR Material");

	ed::BeginPin(nodeB_InputPinId1, ed::PinKind::Input);
	ImGui::Text("-> Alberdo");
	ed::EndPin();
	ed::BeginPin(nodeB_InputPinId2, ed::PinKind::Input);
	ImGui::Text("-> In2");
	ed::EndPin();

	ed::BeginPin(nodeB_OutputPinId, ed::PinKind::Output);
	ImGui::Text("Out ->");
	ed::EndPin();
	ed::EndNode();

	// Submit Links
	for (auto& linkInfo : g_Links)
		ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);

	// Handle creation action, returns true if editor want to create new object (node or link)
	if (ed::BeginCreate())
	{
		ed::PinId inputPinId, outputPinId;
		if (ed::QueryNewLink(&inputPinId, &outputPinId))
		{

			if (inputPinId && outputPinId) // both are valid, let's accept link
			{
				// ed::AcceptNewItem() return true when user release mouse button.
				if (ed::AcceptNewItem())
				{
					// Since we accepted new link, lets add one to our list of links.
					g_Links.push_back({ ed::LinkId(g_NextLinkId++), inputPinId, outputPinId });

					// Draw new link.
					ed::Link(g_Links.back().Id, g_Links.back().InputId, g_Links.back().OutputId);
				}
			}
		}
	}
	ed::EndCreate(); // Wraps up object creation action handling.

	 // Handle deletion action
	if (ed::BeginDelete())
	{
		// There may be many links marked for deletion, let's loop over them.
		ed::LinkId deletedLinkId;
		while (ed::QueryDeletedLink(&deletedLinkId))
		{
			// If you agree that link can be deleted, accept deletion.
			if (ed::AcceptDeletedItem())
			{
				// Then remove link from your data.
				for (auto& link : g_Links)
				{
					if (link.Id == deletedLinkId)
					{
						g_Links.erase(&link);
						break;
					}
				}
			}

			// You may reject link deletion by calling:
			// ed::RejectDeletedItem();
		}
	}
	ed::EndDelete(); // Wrap up deletion action

	ed::End();
}

NAMESPACE_END