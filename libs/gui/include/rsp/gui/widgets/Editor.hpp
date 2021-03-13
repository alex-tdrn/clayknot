#pragma once

#include "rsp/gui/widgets/Widget.hpp"
#include "rsp/util/Bounded.hpp"
#include "rsp/util/ColorRGB.hpp"
#include "rsp/util/ColorRGBA.hpp"
#include "rsp/util/TimeUnit.hpp"
#include "rsp/util/TypeList.hpp"

#include <functional>
#include <imgui.h>
#include <optional>
#include <typeindex>

namespace rsp::gui
{
class Editor : public Widget
{
public:
	using Factory = std::unique_ptr<Editor> (*)(void*, std::string const&, std::optional<std::function<void()>>);

	Editor() = delete;
	explicit Editor(std::string const& dataName);
	Editor(Editor const&) = delete;
	Editor(Editor&&) = delete;
	auto operator=(Editor const&) -> Editor& = delete;
	auto operator=(Editor&&) -> Editor& = delete;
	~Editor() override = default;

	template <typename DataType, typename EditorImplementation>
	static void registerFactory();
	template <typename DataType>
	static auto create(DataType* data, std::string const& dataName,
		std::optional<std::function<void()>> modifiedCallback = std::nullopt) -> std::unique_ptr<Editor>;
	static auto create(std::size_t typeHash, void* data, std::string const& dataName,
		std::optional<std::function<void()>> modifiedCallback = std::nullopt) -> std::unique_ptr<Editor>;
	virtual void updateDataPointer(void* data) = 0;

private:
	static auto getFactories() -> std::unordered_map<std::uint64_t, Factory>&;
};

template <typename DataType>
class EditorOf : public Editor
{
public:
	EditorOf() = delete;
	EditorOf(DataType* data, std::string const& dataName, std::optional<std::function<void()>> modifiedCallback);
	EditorOf(EditorOf const&) = delete;
	EditorOf(EditorOf&&) = delete;
	auto operator=(EditorOf const&) -> EditorOf& = delete;
	auto operator=(EditorOf&&) -> EditorOf& = delete;
	~EditorOf() override = default;

	auto clone() const -> std::unique_ptr<Widget> override;
	void updateDataPointer(void* data) final;
	auto isDataAvailable() const noexcept -> bool final;
	void drawContents() const override;

protected:
	auto getData() const -> DataType*;
	auto getModifiedCallback() const -> std::optional<std::function<void()>> const&;

private:
	DataType* data;
	std::optional<std::function<void()>> modifiedCallback;

	void dataModified() const;
};

template <typename DataType, typename EditorImplementation>
inline void Editor::registerFactory()
{
	getFactories()[std::type_index(typeid(DataType)).hash_code()] =
		[](void* data, const std::string& dataName,
			std::optional<std::function<void()>> modifiedCallback) -> std::unique_ptr<Editor> {
		return std::make_unique<EditorImplementation>(
			static_cast<DataType*>(data), dataName, std::move(modifiedCallback));
	};
}

inline auto Editor::create(std::size_t typeHash, void* data, std::string const& dataName,
	std::optional<std::function<void()>> modifiedCallback) -> std::unique_ptr<Editor>
{
	auto& factories = getFactories();
	if(factories.count(typeHash) == 0)
		return std::make_unique<EditorOf<void>>(data, dataName, std::nullopt);
	return factories.at(typeHash)(data, dataName, std::move(modifiedCallback));
}

template <typename DataType>
auto Editor::create(DataType* data, std::string const& dataName, std::optional<std::function<void()>> modifiedCallback)
	-> std::unique_ptr<Editor>
{
	return Editor::create(std::type_index(typeid(DataType)).hash_code(), data, dataName, modifiedCallback);
}

inline auto Editor::getFactories() -> std::unordered_map<std::uint64_t, Factory>&
{
	static auto factories = []() {
		std::unordered_map<std::uint64_t, Factory> factories;

		using SupportedTypes = meta::TypeList<bool, int, float, glm::vec2, glm::vec3, glm::vec4, Bounded<int>,
			Bounded<float>, Bounded<glm::vec2>, Bounded<glm::vec3>, Bounded<glm::vec4>, ColorRGB, ColorRGBA,
			std::chrono::nanoseconds>;

		SupportedTypes::for_each([&factories](auto* dummy) {
			using CurrentType = std::remove_cv_t<std::remove_pointer_t<decltype(dummy)>>;

			auto hash = std::type_index(typeid(CurrentType)).hash_code();
			assert("Type hash function collision!" && factories.count(hash) == 0);

			factories[hash] = [](void* data, const std::string& dataName,
								  std::optional<std::function<void()>> modifiedCallback) -> std::unique_ptr<Editor> {
				return std::make_unique<EditorOf<CurrentType>>(
					static_cast<CurrentType*>(data), dataName, std::move(modifiedCallback));
			};
		});

		return factories;
	}();
	return factories;
}

inline Editor::Editor(std::string const& dataName) : Widget(dataName)
{
}

template <typename DataType>
EditorOf<DataType>::EditorOf(
	DataType* data, std::string const& dataName, std::optional<std::function<void()>> modifiedCallback)
	: Editor(dataName), data(data), modifiedCallback(std::move(modifiedCallback))
{
}

template <typename DataType>
auto EditorOf<DataType>::clone() const -> std::unique_ptr<Widget>
{
	auto clonedEditor = std::make_unique<EditorOf<DataType>>(data, getDataName(), modifiedCallback);
	return clonedEditor;
}

template <typename DataType>
void EditorOf<DataType>::updateDataPointer(void* data)
{
	this->data = static_cast<DataType*>(data);
}

template <typename DataType>
auto EditorOf<DataType>::isDataAvailable() const noexcept -> bool
{
	return data != nullptr;
}

template <typename DataType>
void EditorOf<DataType>::drawContents() const
{
	ImGui::Text("NO EDITOR IMPLEMENTATION");
}

template <typename DataType>
void EditorOf<DataType>::dataModified() const
{
	if(modifiedCallback)
		(*modifiedCallback)();
}

template <typename DataType>
auto EditorOf<DataType>::getData() const -> DataType*
{
	return data;
}

template <typename DataType>
auto EditorOf<DataType>::getModifiedCallback() const -> std::optional<std::function<void()>> const&
{
	return modifiedCallback;
}

template <>
inline void EditorOf<bool>::drawContents() const
{
	ImGui::SameLine();
	if(ImGui::Checkbox("##", data))
		dataModified();
}

template <>
inline void EditorOf<int>::drawContents() const
{
	ImGui::SetNextItemWidth(getAvailableWidth());
	if(ImGui::DragInt("##", data))
		dataModified();
}

template <>
inline void EditorOf<float>::drawContents() const
{
	ImGui::SetNextItemWidth(getAvailableWidth());
	if(ImGui::DragFloat("##", data))
		dataModified();
}

template <>
inline void EditorOf<glm::vec2>::drawContents() const
{
	if(isExtendedPreferred())
	{
		ImGui::PushItemWidth(getAvailableWidth());
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(*data)[i]))
				dataModified();
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(getAvailableWidth() / 2.0f);
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(*data)[i]))
				dataModified();
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void EditorOf<glm::vec3>::drawContents() const
{
	if(isExtendedPreferred())
	{
		ImGui::PushItemWidth(getAvailableWidth());
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(*data)[i]))
				dataModified();
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(getAvailableWidth() / 3.0f);
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(*data)[i]))
				dataModified();
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void EditorOf<glm::vec4>::drawContents() const
{
	if(isExtendedPreferred())
	{
		ImGui::PushItemWidth(getAvailableWidth());
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(*data)[i]))
				dataModified();
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(getAvailableWidth() / 4.0f);
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::DragFloat("##", &(*data)[i]))
				dataModified();
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void EditorOf<Bounded<int>>::drawContents() const
{
	ImGui::SetNextItemWidth(getAvailableWidth());
	if(ImGui::SliderInt("##", data->data(), data->getMin(), data->getMax()))
		dataModified();
}

template <>
inline void EditorOf<Bounded<float>>::drawContents() const
{
	ImGui::SetNextItemWidth(getAvailableWidth());
	if(ImGui::SliderFloat("##", data->data(), data->getMin(), data->getMax()))
		dataModified();
}

template <>
inline void EditorOf<Bounded<glm::vec2>>::drawContents() const
{
	if(isExtendedPreferred())
	{
		ImGui::PushItemWidth(getAvailableWidth());
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data->data()->operator[](i)), data->getMin()[i], data->getMax()[i]))
				dataModified();
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(getAvailableWidth() / 2.0f);
		for(int i = 0; i < 2; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data->data()->operator[](i)), data->getMin()[i], data->getMax()[i]))
				dataModified();
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void EditorOf<Bounded<glm::vec3>>::drawContents() const
{
	if(isExtendedPreferred())
	{
		ImGui::PushItemWidth(getAvailableWidth());
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data->data()->operator[](i)), data->getMin()[i], data->getMax()[i]))
				dataModified();
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(getAvailableWidth() / 3.0f);
		for(int i = 0; i < 3; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data->data()->operator[](i)), data->getMin()[i], data->getMax()[i]))
				dataModified();
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void EditorOf<Bounded<glm::vec4>>::drawContents() const
{
	if(isExtendedPreferred())
	{
		ImGui::PushItemWidth(getAvailableWidth());
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data->data()->operator[](i)), data->getMin()[i], data->getMax()[i]))
				dataModified();
			ImGui::PopID();
		}
		ImGui::PopItemWidth();
	}
	else
	{
		ImGui::PushItemWidth(getAvailableWidth() / 4.0f);
		for(int i = 0; i < 4; i++)
		{
			ImGui::PushID(i);
			if(ImGui::SliderFloat("##", &(data->data()->operator[](i)), data->getMin()[i], data->getMax()[i]))
				dataModified();
			ImGui::PopID();
			ImGui::SameLine();
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
}

template <>
inline void EditorOf<ColorRGB>::drawContents() const
{
	ImGui::PushItemWidth(getAvailableWidth());

	if(isExtendedPreferred())
	{
		ImGui::PushID(0);
		if(ImGui::ColorEdit3("##", data->data(),
			   ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoDragDrop))
			dataModified();
		ImGui::PopID();

		ImGui::PushID(1);
		if(ImGui::ColorEdit3("##", data->data(),
			   ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoDragDrop))
			dataModified();
		ImGui::PopID();

		ImGui::PushID(2);
		if(ImGui::ColorEdit3("##", data->data(),
			   ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			dataModified();
		ImGui::PopID();

		ImGui::PushID(3);
		if(ImGui::ColorPicker3("##", data->data(),
			   ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			dataModified();
		ImGui::PopID();
	}
	else
	{
		if(ImGui::ColorEdit3("##", data->data(),
			   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop))
			dataModified();
	}
	ImGui::PopItemWidth();
}

template <>
inline void EditorOf<ColorRGBA>::drawContents() const
{
	ImGui::PushItemWidth(getAvailableWidth());
	if(isExtendedPreferred())
	{
		ImGui::PushID(0);
		if(ImGui::ColorEdit4("##", data->data(),
			   ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoDragDrop))
			dataModified();
		ImGui::PopID();

		ImGui::PushID(1);
		if(ImGui::ColorEdit4("##", data->data(),
			   ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_Float |
				   ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoDragDrop))
			dataModified();
		ImGui::PopID();

		ImGui::PushID(2);
		if(ImGui::ColorEdit4("##", data->data(),
			   ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			dataModified();
		ImGui::PopID();

		ImGui::PushID(3);
		if(ImGui::ColorPicker4("##", data->data(),
			   ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_NoSidePreview |
				   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop))
			dataModified();
		ImGui::PopID();
	}
	else
	{
		if(ImGui::ColorEdit4("##", data->data(),
			   ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop))
			dataModified();
	}
	ImGui::PopItemWidth();
}

template <>
inline void EditorOf<std::chrono::nanoseconds>::drawContents() const
{
	auto timeUnits = TimeUnit::decompose(*data);
	if(isExtendedPreferred())
	{
		ImGui::PushItemWidth(getAvailableWidth() / 2.0f);
		int ct = 0;
		for(auto& unit : timeUnits)
		{
			int v = unit.value;
			ImGui::PushID(ct);
			if(ImGui::DragInt("##", &v, 0.1f, 0, 0, ("%i " + unit.suffix).c_str()))
				dataModified();
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
		const std::size_t maxUnitsToDraw = 2;
		ImGui::PushItemWidth(getAvailableWidth() / 2.0f);
		for(std::size_t i = 0; i < timeUnits.size(); i++)
		{
			if((timeUnits[i].value != 0) || timeUnits.size() - i == maxUnitsToDraw)
			{
				for(int j = i; j < timeUnits.size() && j - i < maxUnitsToDraw; j++)
				{
					int v = timeUnits[j].value;
					ImGui::PushID(j);
					if(ImGui::DragInt("##", &v, 0.1f, 0, 0, ("%i " + timeUnits[j].suffix).c_str()))
						dataModified();
					ImGui::PopID();
					timeUnits[j].value = static_cast<short>(v);
					ImGui::SameLine();
				}
				break;
			}
		}
		ImGui::PopItemWidth();
		ImGui::NewLine();
	}
	*data = TimeUnit::compose(timeUnits);
}

} // namespace rsp::gui