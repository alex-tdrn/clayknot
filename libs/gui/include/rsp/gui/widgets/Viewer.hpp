#pragma once

#include "rsp/gui/widgets/Widget.hpp"
#include "rsp/util/Bounded.hpp"
#include "rsp/util/ColorRGB.hpp"
#include "rsp/util/ColorRGBA.hpp"
#include "rsp/util/TimeUnit.hpp"
#include "rsp/util/TypeList.hpp"

#include <imgui.h>
#include <typeindex>
#include <unordered_map>

namespace rsp::gui
{
class Viewer : public Widget
{
public:
	using Factory = std::unique_ptr<Viewer> (*)(void const*, std::string const&);

	Viewer() = delete;
	explicit Viewer(std::string const& dataName);
	Viewer(Viewer const&) = delete;
	Viewer(Viewer&&) = delete;
	auto operator=(Viewer const&) -> Viewer& = delete;
	auto operator=(Viewer&&) -> Viewer& = delete;
	~Viewer() override = default;

	template <typename DataType, typename ViewerImplementation>
	static void registerFactory();
	template <typename DataType>
	static auto create(DataType const* data, std::string const& dataName) -> std::unique_ptr<Viewer>;
	static auto create(std::size_t typeHash, void const* data, std::string const& dataName) -> std::unique_ptr<Viewer>;
	virtual void updateDataPointer(void const* data) = 0;

private:
	static auto getFactories() -> std::unordered_map<std::uint64_t, Factory>&;
};

template <typename DataType>
class ViewerOf : public Viewer
{
public:
	ViewerOf() = delete;
	ViewerOf(DataType const* data, std::string const& dataName);
	ViewerOf(ViewerOf const&) = delete;
	ViewerOf(ViewerOf&&) = delete;
	auto operator=(ViewerOf const&) -> ViewerOf& = delete;
	auto operator=(ViewerOf&&) -> ViewerOf& = delete;
	~ViewerOf() override = default;

	auto clone() const -> std::unique_ptr<Widget> override;
	void updateDataPointer(void const* data) final;
	auto isDataAvailable() const noexcept -> bool final;
	void drawContents() const override;

protected:
	auto getData() const -> DataType const*;

private:
	DataType const* data;
};

template <typename DataType, typename ViewerImplementation>
inline void Viewer::registerFactory()
{
	getFactories()[std::type_index(typeid(DataType)).hash_code()] =
		[](void const* data, std::string const& dataName) -> std::unique_ptr<Viewer> {
		return std::make_unique<ViewerImplementation>(static_cast<DataType const*>(data), dataName);
	};
}

inline auto Viewer::create(std::size_t typeHash, void const* data, std::string const& dataName)
	-> std::unique_ptr<Viewer>
{
	auto& factories = getFactories();
	if(factories.count(typeHash) == 0)
		return std::make_unique<ViewerOf<void>>(data, dataName);
	return factories.at(typeHash)(data, dataName);
}

template <typename DataType>
auto Viewer::create(DataType const* data, std::string const& dataName) -> std::unique_ptr<Viewer>
{
	return Viewer::create(std::type_index(typeid(DataType)).hash_code(), data, dataName);
}

inline auto Viewer::getFactories() -> std::unordered_map<std::uint64_t, Factory>&
{
	static auto factories = []() {
		std::unordered_map<std::uint64_t, Factory> factories;

		using SupportedTypes = meta::TypeList<bool, int, float, glm::vec2, glm::vec3, glm::vec4, Bounded<int>,
			Bounded<float>, Bounded<glm::vec2>, Bounded<glm::vec3>, Bounded<glm::vec4>, ColorRGB, ColorRGBA,
			std::chrono::nanoseconds>;

		SupportedTypes::for_each([&factories](auto* dummy) {
			using CurrentType = std::remove_cv_t<std::remove_pointer_t<decltype(dummy)>>;

			std::size_t hash = std::type_index(typeid(CurrentType)).hash_code();
			assert("Type hash function collision!" && factories.count(hash) == 0);

			factories[hash] = [](void const* data, std::string const& dataName) -> std::unique_ptr<Viewer> {
				return std::make_unique<ViewerOf<CurrentType>>(static_cast<CurrentType const*>(data), dataName);
			};
		});
		return factories;
	}();
	return factories;
}

inline Viewer::Viewer(std::string const& dataName) : Widget(dataName)
{
}

template <typename DataType>
ViewerOf<DataType>::ViewerOf(DataType const* data, std::string const& dataName) : Viewer(dataName), data(data)
{
}

template <typename DataType>
auto ViewerOf<DataType>::clone() const -> std::unique_ptr<Widget>
{
	auto clonedEditor = std::make_unique<ViewerOf<DataType>>(data, getDataName());
	return clonedEditor;
}

template <typename DataType>
void ViewerOf<DataType>::updateDataPointer(void const* data)
{
	this->data = static_cast<DataType const*>(data);
}

template <typename DataType>
auto ViewerOf<DataType>::isDataAvailable() const noexcept -> bool
{
	return data != nullptr;
}

template <typename DataType>
void ViewerOf<DataType>::drawContents() const
{
	ImGui::Text("NO VIEWER IMPLEMENTATION");
}

template <typename DataType>
auto ViewerOf<DataType>::getData() const -> DataType const*
{
	return data;
}

template <>
inline void ViewerOf<bool>::drawContents() const
{
	ImGui::SameLine();
	if(*data)
		ImGui::Text("On");
	else
		ImGui::Text("Off");
}

template <>
inline void ViewerOf<int>::drawContents() const
{
	ImGui::SameLine();
	ImGui::Text("%i", *data);
}

template <>
inline void ViewerOf<float>::drawContents() const
{
	ImGui::SameLine();
	ImGui::Text("%.3f", *data);
}

template <>
inline void ViewerOf<glm::vec2>::drawContents() const
{
	ImGui::Text("%.3f, %.3f", data->x, data->y);
}

template <>
inline void ViewerOf<glm::vec3>::drawContents() const
{
	ImGui::Text("%.3f, %.3f, %.3f", data->x, data->y, data->z);
}

template <>
inline void ViewerOf<glm::vec4>::drawContents() const
{
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", data->x, data->y, data->z, data->w);
}

template <>
inline void ViewerOf<Bounded<int>>::drawContents() const
{
	ImGui::Text("%i (%i - %i)", data->getVal(), data->getMin(), data->getMax());
}

template <>
inline void ViewerOf<Bounded<float>>::drawContents() const
{
	ImGui::Text("%.3f (%.3f - %.3f)", data->getVal(), data->getMin(), data->getMax());
}

template <>
inline void ViewerOf<Bounded<glm::vec2>>::drawContents() const
{
	ImGui::Text("%.3f, %.3f", data->getVal().x, data->getVal().y);
	if(isExtendedPreferred())
	{
		ImGui::Text("X (%.3f - %.3f)", data->getMin()[0], data->getMax()[0]);
		ImGui::Text("Y (%.3f - %.3f)", data->getMin()[1], data->getMax()[1]);
	}
}

template <>
inline void ViewerOf<Bounded<glm::vec3>>::drawContents() const
{
	ImGui::Text("%.3f, %.3f, %.3f", data->getVal().x, data->getVal().y, data->getVal().z);
	if(isExtendedPreferred())
	{
		ImGui::Text("X (%.3f - %.3f)", data->getMin()[0], data->getMax()[0]);
		ImGui::Text("Y (%.3f - %.3f)", data->getMin()[1], data->getMax()[1]);
		ImGui::Text("Z (%.3f - %.3f)", data->getMin()[2], data->getMax()[2]);
	}
}

template <>
inline void ViewerOf<Bounded<glm::vec4>>::drawContents() const
{
	ImGui::Text("%.3f, %.3f, %.3f, %.3f", data->getVal().x, data->getVal().y, data->getVal().z, data->getVal().w);
	if(isExtendedPreferred())
	{
		ImGui::Text("X (%.3f - %.3f)", data->getMin()[0], data->getMax()[0]);
		ImGui::Text("Y (%.3f - %.3f)", data->getMin()[1], data->getMax()[1]);
		ImGui::Text("Z (%.3f - %.3f)", data->getMin()[2], data->getMax()[2]);
		ImGui::Text("W (%.3f - %.3f)", data->getMin()[3], data->getMax()[3]);
	}
}

template <>
inline void ViewerOf<ColorRGB>::drawContents() const
{
	ImGui::Text("R:%.3f, G:%.3f, B:%.3f", data->r(), data->g(), data->b());
	if(isExtendedPreferred())
	{
		auto s = getAvailableWidth();

		ImGui::ColorButton("##", ImVec4(data->r(), data->g(), data->b(), 1.0f),
			ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, {s, s});
	}
	else
	{
		ImGui::SameLine();
		ImGui::ColorButton("##", ImVec4(data->r(), data->g(), data->b(), 1.0f),
			ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	}
}

template <>
inline void ViewerOf<ColorRGBA>::drawContents() const
{
	ImGui::Text("R:%.3f, G:%.3f, B:%.3f, A:%.3f", data->r(), data->g(), data->b(), data->a());
	if(isExtendedPreferred())
	{
		auto s = getAvailableWidth();

		ImGui::ColorButton("##", *data, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop, {s, s});
	}
	else
	{
		ImGui::SameLine();
		ImGui::ColorButton("##", *data, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoDragDrop);
	}
}

template <>
inline void ViewerOf<std::chrono::nanoseconds>::drawContents() const
{
	auto timeUnits = TimeUnit::decompose(*data);
	if(isExtendedPreferred())
	{
		for(auto const& unit : timeUnits)
		{
			if(unit.value != 0)
			{
				ImGui::Text(("%i" + unit.suffix).c_str(), unit.value);
				ImGui::SameLine();
			}
		}
	}
	else
	{
		const int maxUnitsToDraw = 2;
		for(int i = 0; i < timeUnits.size(); i++)
		{
			if(timeUnits[i].value)
			{
				for(int j = i; j < timeUnits.size() && j - i < maxUnitsToDraw; j++)
				{
					ImGui::Text(("%i" + timeUnits[j].suffix).c_str(), timeUnits[j].value);
					ImGui::SameLine();
				}
				break;
			}
		}
		ImGui::NewLine();
	}
}

} // namespace rsp::gui
