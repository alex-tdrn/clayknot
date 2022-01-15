#pragma once

#include "clk/gui/widgets/data_writer.hpp"
#include "clk/gui/widgets/widget.hpp"
#include "clk/util/bounded.hpp"
#include "clk/util/color_rgb.hpp"
#include "clk/util/color_rgba.hpp"
#include "clk/util/time_unit.hpp"
#include "clk/util/type_list.hpp"

#include <any>
#include <imgui.h>
#include <range/v3/view.hpp>
#include <typeindex>

namespace clk::gui
{
class editor : public widget
{
public:
	editor() = default;
	editor(editor const&) = delete;
	editor(editor&&) = delete;
	auto operator=(editor const&) -> editor& = delete;
	auto operator=(editor&&) -> editor& = delete;
	~editor() override = default;

	template <typename data_type, typename editorImplementation>
	static void register_factory();
	template <typename data_type>
	static auto create(data_writer<data_type> data_writer, std::string_view name) -> std::unique_ptr<editor>;
	static auto create(data_writer<void> data_writer, std::uint64_t data_hash, std::string_view name)
		-> std::unique_ptr<editor>;

private:
	using factory = std::unique_ptr<editor> (*)(std::any, std::string_view);
	using nested_data_writer_factory = std::any (*)(data_writer<void>);

	static auto create(std::any data_writer, std::string_view name) -> std::unique_ptr<editor>;
	static auto factories_map() -> std::unordered_map<std::uint64_t, factory>&;
	static auto nested_data_writer_factories_map() -> std::unordered_map<std::uint64_t, nested_data_writer_factory>&;
};

template <typename data_type>
class editor_of : public editor
{
public:
	editor_of() = default;
	editor_of(editor_of const&) = delete;
	editor_of(editor_of&&) = delete;
	auto operator=(editor_of const&) -> editor_of& = delete;
	auto operator=(editor_of&&) -> editor_of& = delete;
	~editor_of() override = default;

	auto clone() const -> std::unique_ptr<widget> override;
	void copy(widget const& other) override;

	void set_data_writer(data_writer<data_type> data);
	virtual auto draw_contents(data_type& data) const -> bool;

private:
	data_writer<data_type> _data;

	void draw_contents() const final;
};

template <typename data_type, typename editor_implementation>
inline void editor::register_factory()
{
	auto data_writer_hash = std::type_index(typeid(data_writer<data_type>)).hash_code();
	factories_map()[data_writer_hash] = [](std::any data, std::string_view name) -> std::unique_ptr<editor> {
		auto editor = std::make_unique<editor_implementation>();
		editor->set_name(name);
		editor->set_data_writer(std::any_cast<data_writer<data_type>>(data));
		return editor;
	};

	auto data_hash = std::type_index(typeid(data_type)).hash_code();
	nested_data_writer_factories_map()[data_hash] = [](data_writer<void> type_erased_data_writer) -> std::any {
		auto getter = [=]() {
			return static_cast<data_type*>(type_erased_data_writer.read());
		};
		auto setter = [type_erased_data_writer = std::move(type_erased_data_writer)](data_type* data) {
			type_erased_data_writer.write(data);
		};
		return data_writer<data_type>{std::move(getter), std::move(setter)};
	};
}

template <typename data_type>
auto editor::create(data_writer<data_type> data_writer, std::string_view name) -> std::unique_ptr<editor>
{
	return editor::create(std::any(std::move(data_writer)), name);
}

inline auto editor::create(data_writer<void> data_writer, std::uint64_t data_hash, std::string_view name)
	-> std::unique_ptr<editor>
{
	if(nested_data_writer_factories_map().count(data_hash) == 0)
		throw std::runtime_error("Cannot create requested editor");
	return create(nested_data_writer_factories_map()[data_hash](std::move(data_writer)), name);
}

inline auto editor::create(std::any data_writer, std::string_view name) -> std::unique_ptr<editor>
{
	auto& factories = factories_map();
	auto hash = data_writer.type().hash_code();
	if(factories.count(hash) == 0)
		throw std::runtime_error("Cannot create requested editor");
	return factories.at(hash)(std::move(data_writer), name);
}

inline auto editor::factories_map() -> std::unordered_map<std::uint64_t, factory>&
{
	static auto factories = []() {
		std::unordered_map<std::uint64_t, factory> map;

		using supported_types = meta::type_list<bool, int, float, glm::vec2, glm::vec3, glm::vec4, clk::bounded<int>,
			clk::bounded<float>, clk::bounded<glm::vec2>, clk::bounded<glm::vec3>, clk::bounded<glm::vec4>,
			clk::color_rgb, clk::color_rgba, std::chrono::nanoseconds>;

		supported_types::for_each([&map](auto* dummy) {
			using current_type = std::remove_cv_t<std::remove_pointer_t<decltype(dummy)>>;

			auto data_writer_hash = std::type_index(typeid(data_writer<current_type>)).hash_code();
			assert("Type hash function collision!" && map.count(data_writer_hash) == 0);

			map[data_writer_hash] = [](std::any data, std::string_view name) -> std::unique_ptr<editor> {
				auto editor = std::make_unique<editor_of<current_type>>();
				editor->set_name(name);
				editor->set_data_writer(std::any_cast<data_writer<current_type>&&>(std::move(data)));
				return editor;
			};

			auto data_hash = std::type_index(typeid(current_type)).hash_code();
			nested_data_writer_factories_map()[data_hash] = [](data_writer<void> type_erased_data_writer) -> std::any {
				auto getter = [=]() {
					return static_cast<current_type*>(type_erased_data_writer.read());
				};
				auto setter = [type_erased_data_writer = std::move(type_erased_data_writer)](current_type* data) {
					type_erased_data_writer.write(data);
				};
				return data_writer<current_type>{std::move(getter), std::move(setter)};
			};
		});

		return map;
	}();
	return factories;
}

inline auto editor::nested_data_writer_factories_map() -> std::unordered_map<std::uint64_t, nested_data_writer_factory>&
{
	static std::unordered_map<std::uint64_t, nested_data_writer_factory> map;
	return map;
}

template <typename data_type>
auto editor_of<data_type>::clone() const -> std::unique_ptr<widget>
{
	auto clone = std::make_unique<editor_of<data_type>>();
	clone->copy(*this);
	return clone;
}

template <typename data_type>
void editor_of<data_type>::copy(widget const& other)
{
	auto const& casted = dynamic_cast<editor_of<data_type> const&>(other);
	_data = casted._data;
	editor::copy(other);
}

template <typename data_type>
void editor_of<data_type>::set_data_writer(data_writer<data_type> data)
{
	_data = std::move(data);
}

template <typename data_type>
void editor_of<data_type>::draw_contents() const
{
	if(auto* data = _data.read(); data != nullptr)
	{
		if(draw_contents(*data))
		{
			_data.write(data); // TODO pass in new data
		}
	}
	else
	{
		ImGui::Text("NO DATA");
	}
}

template <typename data_type>
auto editor_of<data_type>::draw_contents(data_type& /*data*/) const -> bool
{
	ImGui::Text("NO EDITOR IMPLEMENTATION");
	return false;
}

template <>
inline auto editor_of<bool>::draw_contents(bool& data) const -> bool
{
	ImGui::SameLine();
	return ImGui::Checkbox("##", &data);
}

template <>
inline auto editor_of<int>::draw_contents(int& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	return ImGui::DragInt("##", &data);
}

template <>
inline auto editor_of<float>::draw_contents(float& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	return ImGui::DragFloat("##", &data, 0.01f);
}

template <>
inline auto editor_of<glm::vec2>::draw_contents(glm::vec2& data) const -> bool
{
	auto data_modified = false;
	if(extended_preferred())
	{
		ImGui::PushItemWidth(available_width());
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(data[i])))
				data_modified = true;
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(available_width() / 2.0f);
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(data[i])))
				data_modified = true;
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	return data_modified;
}

template <>
inline auto editor_of<glm::vec3>::draw_contents(glm::vec3& data) const -> bool
{
	auto data_modified = false;
	if(extended_preferred())
	{
		ImGui::PushItemWidth(available_width());
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(data[i])))
				data_modified = true;
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(available_width() / 3.0f);
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(data[i])))
				data_modified = true;
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	return data_modified;
}

template <>
inline auto editor_of<glm::vec4>::draw_contents(glm::vec4& data) const -> bool
{
	auto data_modified = false;
	if(extended_preferred())
	{
		ImGui::PushItemWidth(available_width());
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(data[i])))
				data_modified = true;
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(available_width() / 4.0f);
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(data[i])))
				data_modified = true;
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	return data_modified;
}

template <>
inline auto editor_of<clk::bounded<int>>::draw_contents(clk::bounded<int>& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	return ImGui::SliderInt("##", data.data(), data.min(), data.max());
}

template <>
inline auto editor_of<clk::bounded<float>>::draw_contents(clk::bounded<float>& data) const -> bool
{
	ImGui::SetNextItemWidth(available_width());
	return ImGui::SliderFloat("##", data.data(), data.min(), data.max());
}

template <>
inline auto editor_of<clk::bounded<glm::vec2>>::draw_contents(clk::bounded<glm::vec2>& data) const -> bool
{
	auto data_modified = false;
	if(extended_preferred())
	{
		ImGui::PushItemWidth(available_width());
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data.data()->operator[](i)), data.min()[i], data.max()[i]))
				data_modified = true;
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(available_width() / 2.0f);
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data.data()->operator[](i)), data.min()[i], data.max()[i]))
				data_modified = true;
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	return data_modified;
}

template <>
inline auto editor_of<clk::bounded<glm::vec3>>::draw_contents(clk::bounded<glm::vec3>& data) const -> bool
{
	auto data_modified = false;
	if(extended_preferred())
	{
		ImGui::PushItemWidth(available_width());
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data.data()->operator[](i)), data.min()[i], data.max()[i]))
				data_modified = true;
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(available_width() / 3.0f);
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data.data()->operator[](i)), data.min()[i], data.max()[i]))
				data_modified = true;
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	return data_modified;
}

template <>
inline auto editor_of<clk::bounded<glm::vec4>>::draw_contents(clk::bounded<glm::vec4>& data) const -> bool
{
	auto data_modified = false;
	if(extended_preferred())
	{
		ImGui::PushItemWidth(available_width());
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data.data()->operator[](i)), data.min()[i], data.max()[i]))
				data_modified = true;
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(available_width() / 4.0f);
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data.data()->operator[](i)), data.min()[i], data.max()[i]))
				data_modified = true;
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	return data_modified;
}

template <>
inline auto editor_of<clk::color_rgb>::draw_contents(clk::color_rgb& data) const -> bool
{
	auto data_modified = false;
	ImGui::PushItemWidth(available_width());

	if(extended_preferred())
	{
		ImGui::PushID(0);
		if(ImGui::ColorEdit3("##", data.data(),
			   ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();

		ImGui::PushID(1);
		if(ImGui::ColorEdit3("##", data.data(),
			   ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();

		ImGui::PushID(2);
		if(ImGui::ColorEdit3("##", data.data(),
			   ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();

		ImGui::PushID(3);
		if(ImGui::ColorPicker3("##", data.data(),
			   ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();
	}
	else
	{
		if(ImGui::ColorEdit3("##", data.data(),
			   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
	}
	ImGui::PopItemWidth();
	return data_modified;
}

template <>
inline auto editor_of<clk::color_rgba>::draw_contents(clk::color_rgba& data) const -> bool
{
	auto data_modified = false;
	ImGui::PushItemWidth(available_width());
	if(extended_preferred())
	{
		ImGui::PushID(0);
		if(ImGui::ColorEdit4("##", data.data(),
			   ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();

		ImGui::PushID(1);
		if(ImGui::ColorEdit4("##", data.data(),
			   ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();

		ImGui::PushID(2);
		if(ImGui::ColorEdit4("##", data.data(),
			   ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();

		ImGui::PushID(3);
		if(ImGui::ColorPicker4("##", data.data(),
			   ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
		ImGui::PopID();
	}
	else
	{
		if(ImGui::ColorEdit4("##", data.data(),
			   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop))
			data_modified = true;
	}
	ImGui::PopItemWidth();
	return data_modified;
}

template <>
inline auto editor_of<std::chrono::nanoseconds>::draw_contents(std::chrono::nanoseconds& data) const -> bool
{
	auto data_modified = false;
	auto time_units = time_unit::decompose(data);
	if(extended_preferred())
	{
		ImGui::PushItemWidth(available_width() / 2.0f);
		int ct = 0;
		for(auto& unit : time_units)
		{
			int v = unit.value;
			ImGui::PushID(ct);
			if(ImGui::DragInt("##", &v, 0.1f, 0, 0, ("%i " + unit.suffix).c_str()))
				data_modified = true;
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
		ImGui::PushItemWidth(available_width() / 2.0f);
		const std::size_t max_units_to_draw = 2;

		auto non_empty_units = time_units | ranges::views::drop_while([&, dropped = std::size_t(0)](auto unit) mutable {
			dropped++;
			return unit.value <= 0 && time_units.size() - dropped < max_units_to_draw;
		});

		for(auto [index, unit] : ranges::views::enumerate(non_empty_units | ranges::views::take(max_units_to_draw)))
		{
			int v = unit.value;
			ImGui::PushID(static_cast<int>(index));
			if(ImGui::DragInt("##", &v, 0.1f, 0, 0, ("%i " + unit.suffix).c_str()))
				data_modified = true;
			ImGui::PopID();
			unit.value = static_cast<short>(v);
			ImGui::SameLine();
		}

		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	data = time_unit::compose(time_units);

	return data_modified;
}

} // namespace clk::gui