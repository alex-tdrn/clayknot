#pragma once
#include <glm/glm.hpp>
#include <string>
#include <imgui.h>

class Window
{
private:
	static inline int idCounter = 0;

protected:
	const int id = idCounter++;
	std::string title; 
	bool visible = true;
	ImGuiWindowFlags windowFlags;

public:
	Window() = default;
	Window(Window const&) = delete;
	Window(Window&&) = default;
	Window& operator=(Window const&) = delete;
	Window& operator=(Window&&) = default;
	virtual ~Window() = default;

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