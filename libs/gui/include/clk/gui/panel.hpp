#pragma once

#include <imgui.h>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "clk/gui/widgets/widget.hpp"

namespace clk::gui
{
class panel final
{
	friend void draw();

public:
	enum class action_type
	{
		duplicate,
		remove
	};

	enum class resizability
	{
		off = 0,
		manual,
		automatic
	};

	static void orphan(panel&& panel);

	template <typename... Args>
	static void create_orphan(Args... args)
	{
		orphan(panel(std::forward<Args>(args)...));
	}

	panel();
	explicit panel(std::unique_ptr<clk::gui::widget>&& widget);
	panel(std::unique_ptr<clk::gui::widget>&& widget, std::string_view title);
	panel(panel const& other);
	panel(panel&&) noexcept;
	auto operator=(panel const& other) -> panel&;
	auto operator=(panel&&) noexcept -> panel& = default;
	~panel();

	auto operator==(panel const& other) const noexcept -> bool;

	void queue(action_type action_type) const;
	void set_widget(std::unique_ptr<clk::gui::widget>&& widget) noexcept;
	void draw();
	void set_title(std::string_view title);
	auto title() const noexcept -> std::string_view;
	auto visible() const noexcept -> bool;
	void toggle_visibility() noexcept;
	void show() noexcept;
	void hide() noexcept;
	void set_title_bar_visibility(bool visible) noexcept;
	auto title_bar_is_visible() const noexcept -> bool;
	void set_resizability(resizability mode) noexcept;
	auto resizability_mode() const noexcept -> resizability;
	void set_movability(bool movable) noexcept;
	auto is_movable() const noexcept -> bool;
	void set_docking(bool enabled) noexcept;
	auto docking_is_enabled() const noexcept -> bool;
	void set_interactivity(bool interactive) noexcept;
	auto is_interactive() const noexcept -> bool;
	void set_opactiy(float opacity) noexcept;
	auto opacity() const noexcept -> float;

private:
	inline static std::vector<panel*> _all_panels; // NOLINT
	inline static std::vector<panel> _orphaned_panels; // NOLINT
	inline static std::vector<std::pair<int, action_type>> _queued_actions; // NOLINT

	int _id = generate_id();
	std::unique_ptr<clk::gui::widget> _widget;
	std::string _title_with_id;
	std::string _title;
	bool _visible = true;
	ImGuiWindowFlags _flags = ImGuiWindowFlags_None;
	float _opacity = 1.0f;
	bool _interactive = true;
	bool _orphan = false;

	static auto generate_id() noexcept -> int;

	void register_self();
	void deregister_self() noexcept;
	void update_title_with_id();
	void handle_context_menu();
};

} // namespace clk::gui