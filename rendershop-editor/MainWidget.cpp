#include "MainWidget.h"

#include <imgui.h>

MainWidget::MainWidget()
{
	title = "Main Widget";
	dockspaceName = title + " Dockspace";
	windowFlags = ImGuiWindowFlags_MenuBar;
	windowFlags |= ImGuiWindowFlags_NoDocking;
	windowFlags |= ImGuiWindowFlags_NoTitleBar;
	windowFlags |= ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoMove;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	windowFlags |= ImGuiWindowFlags_NoNavFocus;
}

Widget* MainWidget::addChildImpl(std::unique_ptr<Widget>&& widget)
{
	children.push_back(std::move(widget));
	return children.back().get();
}

void MainWidget::drawContents()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin(title.c_str(), nullptr, windowFlags);
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
