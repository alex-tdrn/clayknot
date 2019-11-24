#pragma once
#include <glm/glm.hpp>
#include <string>
#include <imgui.h>

namespace rshp::gui
{
	
	class Panel
	{
	private:
		static inline int idCounter = 0;

	protected:
		const int id = idCounter++;
		std::string title; 
		bool visible = true;
		ImGuiWindowFlags flags = ImGuiWindowFlags_None;

	public:
		Panel() = default;
		Panel(Panel const&) = delete;
		Panel(Panel&&) = default;
		Panel& operator=(Panel const&) = delete;
		Panel& operator=(Panel&&) = default;
		virtual ~Panel() = default;

	protected:
		virtual void drawContents() = 0;

	public:
		void draw();
		std::string const& getTitle() const;
		bool isVisible() const;
		void toggleVisibility();
		void show();
		void hide();

	};

}