#include "rshp/gui/panels/RootPanel.h"

#include <imgui.h>

namespace rshp::gui
{
	
	RootPanel::RootPanel()
	{
		title = "Main Window";
		dockspaceName = title + " Dockspace";
		flags = ImGuiWindowFlags_MenuBar;
		flags |= ImGuiWindowFlags_NoDocking;
		flags |= ImGuiWindowFlags_NoTitleBar;
		flags |= ImGuiWindowFlags_NoCollapse;
		flags |= ImGuiWindowFlags_NoResize;
		flags |= ImGuiWindowFlags_NoMove;
		flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		flags |= ImGuiWindowFlags_NoNavFocus;
	}

	Panel* RootPanel::addChildImpl(std::unique_ptr<Panel>&& widget)
	{
		children.push_back(std::move(widget));
		return children.back().get();
	}

	void RootPanel::drawContents()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(title.c_str(), nullptr, flags);
		ImGui::PopStyleVar(3);

		ImGuiID dockspace_id = ImGui::GetID(dockspaceName.c_str());
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

		if(ImGui::BeginMenuBar())
		{
			if(ImGui::BeginMenu("View"))
			{
				for(auto& child : children)
					if(ImGui::MenuItem(child->getTitle().c_str(), "", child->isVisible()))
						child->toggleVisibility();
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();
		for(auto& child : children)
			child->draw();
	}

}