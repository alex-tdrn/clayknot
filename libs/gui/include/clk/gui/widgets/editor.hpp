#pragma once

#include "clk/gui/widgets/widget.hpp"
#include "clk/util/bounded.hpp"
#include "clk/util/color_rgb.hpp"
#include "clk/util/color_rgba.hpp"
#include "clk/util/time_unit.hpp"
#include "clk/util/type_list.hpp"

#include <functional>
#include <imgui.h>
#include <optional>
#include <typeindex>

namespace clk::gui
{
class editor : public widget
{
public:
	using factory = std::unique_ptr<editor> (*)(void*, std::string const&, std::optional<std::function<void()>>);

	editor() = delete;
	explicit editor(std::string const& data_name);
	editor(editor const&) = delete;
	editor(editor&&) = delete;
	auto operator=(editor const&) -> editor& = delete;
	auto operator=(editor&&) -> editor& = delete;
	~editor() override = default;

	template <typename data_type, typename editorImplementation>
	static void register_factory();
	template <typename data_type>
	static auto create(data_type* data, std::string const& data_name,
		std::optional<std::function<void()>> modified_callback = std::nullopt) -> std::unique_ptr<editor>;
	static auto create(std::size_t type_hash, void* data, std::string const& data_name,
		std::optional<std::function<void()>> modified_callback = std::nullopt) -> std::unique_ptr<editor>;
	virtual void update_data_pointer(void* data) = 0;

private:
	static auto get_factories() -> std::unordered_map<std::uint64_t, factory>&;
};

template <typename data_type>
class editor_of : public editor
{
public:
	editor_of() = delete;
	editor_of(data_type* data, std::string const& data_name, std::optional<std::function<void()>> modified_callback);
	editor_of(editor_of const&) = delete;
	editor_of(editor_of&&) = delete;
	auto operator=(editor_of const&) -> editor_of& = delete;
	auto operator=(editor_of&&) -> editor_of& = delete;
	~editor_of() override = default;

	auto clone() const -> std::unique_ptr<widget> override;
	void update_data_pointer(void* data) final;
	auto is_data_available() const noexcept -> bool final;
	void draw_contents() const override;

protected:
	auto get_data() const -> data_type*;
	auto get_modified_callback() const -> std::optional<std::function<void()>> const&;

private:
	data_type* _data;
	std::optional<std::function<void()>> _modified_callback;

	void data_modified() const;
};

template <typename data_type, typename editorImplementation>
inline void editor::register_factory()
{
	get_factories()[std::type_index(typeid(data_type)).hash_code()] =
		[](void* data, const std::string& data_name,
			std::optional<std::function<void()>> modified_callback) -> std::unique_ptr<editor> {
		return std::make_unique<editorImplementation>(
			static_cast<data_type*>(data), data_name, std::move(modified_callback));
	};
}

inline auto editor::create(std::size_t type_hash, void* data, std::string const& data_name,
	std::optional<std::function<void()>> modified_callback) -> std::unique_ptr<editor>
{
	auto& factories = get_factories();
	if(factories.count(type_hash) == 0)
		return std::make_unique<editor_of<void>>(data, data_name, std::nullopt);
	return factories.at(type_hash)(data, data_name, std::move(modified_callback));
}

template <typename data_type>
auto editor::create(data_type* data, std::string const& data_name,
	std::optional<std::function<void()>> modified_callback) -> std::unique_ptr<editor>
{
	return editor::create(std::type_index(typeid(data_type)).hash_code(), data, data_name, modified_callback);
}

inline auto editor::get_factories() -> std::unordered_map<std::uint64_t, factory>&
{
	static auto factories = []() {
		std::unordered_map<std::uint64_t, factory> factories;

		using supported_types = meta::type_list<bool, int, float, glm::vec2, glm::vec3, glm::vec4, clk::bounded<int>,
			clk::bounded<float>, clk::bounded<glm::vec2>, clk::bounded<glm::vec3>, clk::bounded<glm::vec4>,
			clk::color_rgb, clk::color_rgba, std::chrono::nanoseconds>;

		supported_types::for_each([&factories](auto* dummy) {
			using current_type = std::remove_cv_t<std::remove_pointer_t<decltype(dummy)>>;

			auto hash = std::type_index(typeid(current_type)).hash_code();
			assert("Type hash function collision!" && factories.count(hash) == 0);

			factories[hash] = [](void* data, const std::string& data_name,
								  std::optional<std::function<void()>> modified_callback) -> std::unique_ptr<editor> {
				return std::make_unique<editor_of<current_type>>(
					static_cast<current_type*>(data), data_name, std::move(modified_callback));
			};
		});

		return factories;
	}();
	return factories;
}

inline editor::editor(std::string const& data_name) : widget(data_name)
{
}

template <typename data_type>
editor_of<data_type>::editor_of(
	data_type* data, std::string const& data_name, std::optional<std::function<void()>> modified_callback)
	: editor(data_name), _data(data), _modified_callback(std::move(modified_callback))
{
}

template <typename data_type>
auto editor_of<data_type>::clone() const -> std::unique_ptr<widget>
{
	auto clonededitor = std::make_unique<editor_of<data_type>>(_data, get_data_name(), _modified_callback);
	return clonededitor;
}

template <typename data_type>
void editor_of<data_type>::update_data_pointer(void* data)
{
	_data = static_cast<data_type*>(data);
}

template <typename data_type>
auto editor_of<data_type>::is_data_available() const noexcept -> bool
{
	return _data != nullptr;
}

template <typename data_type>
void editor_of<data_type>::draw_contents() const
{
	ImGui::Text("NO EDITOR IMPLEMENTATION");
}

template <typename data_type>
void editor_of<data_type>::data_modified() const
{
	if(_modified_callback)
		(*_modified_callback)();
}

template <typename data_type>
auto editor_of<data_type>::get_data() const -> data_type*
{
	return _data;
}

template <typename data_type>
auto editor_of<data_type>::get_modified_callback() const -> std::optional<std::function<void()>> const&
{
	return _modified_callback;
}

template <>
inline void editor_of<bool>::draw_contents() const
{
	ImGui::SameLine();
	if(ImGui::Checkbox("##", _data))
		data_modified();
}

template <>
inline void editor_of<int>::draw_contents() const
{
	ImGui::SetNextItemWidth(get_available_width());
	if(ImGui::DragInt("##", _data))
		data_modified();
}

template <>
inline void editor_of<float>::draw_contents() const
{
	ImGui::SetNextItemWidth(get_available_width());
	if(ImGui::DragFloat("##", _data))
		data_modified();
}

template <>
inline void editor_of<glm::vec2>::draw_contents() const
{
	if(is_extended_preferred())
	{
		ImGui::PushItemWidth(get_available_width());
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(*_data)[i]))
				data_modified();
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(get_available_width() / 2.0f);
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(*_data)[i]))
				data_modified();
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void editor_of<glm::vec3>::draw_contents() const
{
	if(is_extended_preferred())
	{
		ImGui::PushItemWidth(get_available_width());
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(*_data)[i]))
				data_modified();
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(get_available_width() / 3.0f);
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(*_data)[i]))
				data_modified();
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void editor_of<glm::vec4>::draw_contents() const
{
	if(is_extended_preferred())
	{
		ImGui::PushItemWidth(get_available_width());
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(*_data)[i]))
				data_modified();
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(get_available_width() / 4.0f);
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(*_data)[i]))
				data_modified();
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void editor_of<clk::bounded<int>>::draw_contents() const
{
	ImGui::SetNextItemWidth(get_available_width());
	if(ImGui::SliderInt("##", _data->data(), _data->get_min(), _data->get_max()))
		data_modified();
}

template <>
inline void editor_of<clk::bounded<float>>::draw_contents() const
{
	ImGui::SetNextItemWidth(get_available_width());
	if(ImGui::SliderFloat("##", _data->data(), _data->get_min(), _data->get_max()))
		data_modified();
}

template <>
inline void editor_of<clk::bounded<glm::vec2>>::draw_contents() const
{
	if(is_extended_preferred())
	{
		ImGui::PushItemWidth(get_available_width());
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(_data->data()->operator[](i)), _data->get_min()[i], _data->get_max()[i]))
				data_modified();
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(get_available_width() / 2.0f);
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(_data->data()->operator[](i)), _data->get_min()[i], _data->get_max()[i]))
				data_modified();
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void editor_of<clk::bounded<glm::vec3>>::draw_contents() const
{
	if(is_extended_preferred())
	{
		ImGui::PushItemWidth(get_available_width());
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(_data->data()->operator[](i)), _data->get_min()[i], _data->get_max()[i]))
				data_modified();
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(get_available_width() / 3.0f);
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(_data->data()->operator[](i)), _data->get_min()[i], _data->get_max()[i]))
				data_modified();
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void editor_of<clk::bounded<glm::vec4>>::draw_contents() const
{
	if(is_extended_preferred())
	{
		ImGui::PushItemWidth(get_available_width());
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(_data->data()->operator[](i)), _data->get_min()[i], _data->get_max()[i]))
				data_modified();
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(get_available_width() / 4.0f);
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(_data->data()->operator[](i)), _data->get_min()[i], _data->get_max()[i]))
				data_modified();
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void editor_of<clk::color_rgb>::draw_contents() const
{
	ImGui::PushItemWidth(get_available_width());

	if(is_extended_preferred())
	{
		ImGui::PushID(0);
		if(ImGui::ColorEdit3("##", _data->data(),
			   ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoDragDrop))
			data_modified();
		ImGui::PopID();

		ImGui::PushID(1);
		if(ImGui::ColorEdit3("##", _data->data(),
			   ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoDragDrop))
			data_modified();
		ImGui::PopID();

		ImGui::PushID(2);
		if(ImGui::ColorEdit3("##", _data->data(),
			   ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			data_modified();
		ImGui::PopID();

		ImGui::PushID(3);
		if(ImGui::ColorPicker3("##", _data->data(),
			   ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			data_modified();
		ImGui::PopID();
	}
	else
	{
		if(ImGui::ColorEdit3("##", _data->data(),
			   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop))
			data_modified();
	}
	ImGui::PopItemWidth();
}

template <>
inline void editor_of<clk::color_rgba>::draw_contents() const
{
	ImGui::PushItemWidth(get_available_width());
	if(is_extended_preferred())
	{
		ImGui::PushID(0);
		if(ImGui::ColorEdit4("##", _data->data(),
			   ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoDragDrop))
			data_modified();
		ImGui::PopID();

		ImGui::PushID(1);
		if(ImGui::ColorEdit4("##", _data->data(),
			   ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoDragDrop))
			data_modified();
		ImGui::PopID();

		ImGui::PushID(2);
		if(ImGui::ColorEdit4("##", _data->data(),
			   ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			data_modified();
		ImGui::PopID();

		ImGui::PushID(3);
		if(ImGui::ColorPicker4("##", _data->data(),
			   ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			data_modified();
		ImGui::PopID();
	}
	else
	{
		if(ImGui::ColorEdit4("##", _data->data(),
			   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop))
			data_modified();
	}
	ImGui::PopItemWidth();
}

template <>
inline void editor_of<std::chrono::nanoseconds>::draw_contents() const
{
	auto time_units = time_unit::decompose(*_data);
	if(is_extended_preferred())
	{
		ImGui::PushItemWidth(get_available_width() / 2.0f);
		int ct = 0;
		for(auto& unit : time_units)
		{
			int v = unit.value;
			ImGui::PushID(ct);
			if(ImGui::DragInt("##", &v, 0.1f, 0, 0, ("%i " + unit.suffix).c_str()))
				data_modified();
			ImGui::PopID();
			unit.value = static_cast<short>(v);
			if(++ct % 2 != 0)
				ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	else
	{
		const std::size_t max_units_to_draw = 2;
		ImGui::PushItemWidth(get_available_width() / 2.0f);
		for(std::size_t i = 0; i < time_units.size(); i++)
		{
			if((time_units[i].value != 0) || time_units.size() - i == max_units_to_draw)
			{
				for(int j = i; j < time_units.size() && j - i < max_units_to_draw; j++)
				{
					int v = time_units[j].value;
					ImGui::PushID(j);
					if(ImGui::DragInt("##", &v, 0.1f, 0, 0, ("%i " + time_units[j].suffix).c_str()))
						data_modified();
					ImGui::PopID();
					time_units[j].value = static_cast<short>(v);
					ImGui::SameLine();
				}
				break;
			}
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	*_data = time_unit::compose(time_units);
}

} // namespace clk::gui