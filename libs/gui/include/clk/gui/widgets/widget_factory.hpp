#pragma once

#include "clk/gui/widgets/editor.hpp"
#include "clk/gui/widgets/viewer.hpp"
#include "clk/gui/widgets/widget.hpp"

#include <algorithm>
#include <any>
#include <functional>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <typeindex>

namespace clk::gui
{
class widget_factory : public std::enable_shared_from_this<widget_factory>
{
public:
	widget_factory() = default;
	widget_factory(widget_factory const&) = default;
	widget_factory(widget_factory&&) = default;
	auto operator=(widget_factory const&) -> widget_factory& = default;
	auto operator=(widget_factory&&) -> widget_factory& = default;
	~widget_factory() = default;

	template <typename data_type>
	auto is_viewer_registered() const -> bool
	{
		return _viewer_factories.find(std::type_index(typeid(data_reader<data_type>)).hash_code()) !=
			   _viewer_factories.end();
	}

	template <typename data_type>
	auto is_editor_registered() const -> bool
	{
		return _editor_factories.find(std::type_index(typeid(data_writer<data_type>)).hash_code()) !=
			   _editor_factories.end();
	}

	template <typename data_type>
	void register_viewer(std::function<std::unique_ptr<viewer>(
			data_reader<data_type>, std::shared_ptr<widget_factory>, std::string_view)>
			factory)
	{
		auto data_reader_hash = std::type_index(typeid(data_reader<data_type>)).hash_code();
		_viewer_factories[data_reader_hash] = [this, factory = std::move(factory)](
												  std::any data, std::string_view name) -> std::unique_ptr<viewer> {
			auto viewer = factory(std::any_cast<data_reader<data_type>&&>(std::move(data)), shared_from_this(), name);
			return viewer;
		};

		auto data_hash = std::type_index(typeid(data_type)).hash_code();
		_nested_data_reader_factories[data_hash] = [](data_reader<void> type_erased_data_reader) -> std::any {
			return data_reader<data_type>{[nested_reader = std::move(type_erased_data_reader)]() {
				return static_cast<data_type const*>(nested_reader.read());
			}};
		};
	}

	template <typename data_type, typename viewer_implementation>
	void register_viewer()
	{
		register_viewer<data_type>([](data_reader<data_type> data_reader, std::shared_ptr<widget_factory> factory,
									   std::string_view name) -> std::unique_ptr<viewer> {
			auto viewer = std::make_unique<viewer_implementation>(std::move(factory), name);
			viewer->set_data_reader(std::move(data_reader));
			return viewer;
		});
	}

	template <typename data_type>
	void register_editor(std::function<std::unique_ptr<editor>(
			data_writer<data_type>, std::shared_ptr<widget_factory>, std::string_view)>
			factory)
	{
		auto data_writer_hash = std::type_index(typeid(data_writer<data_type>)).hash_code();
		_editor_factories[data_writer_hash] = [this, factory = std::move(factory)](
												  std::any data, std::string_view name) -> std::unique_ptr<editor> {
			auto editor = factory(std::any_cast<data_writer<data_type>&&>(std::move(data)), shared_from_this(), name);
			return editor;
		};

		auto data_hash = std::type_index(typeid(data_type)).hash_code();
		_nested_data_writer_factories[data_hash] = [](data_writer<void> type_erased_data_writer) -> std::any {
			auto getter = [=]() {
				return static_cast<data_type*>(type_erased_data_writer.read());
			};
			auto setter = [type_erased_data_writer = std::move(type_erased_data_writer)](data_type* data) {
				type_erased_data_writer.write(data);
			};
			return data_writer<data_type>{std::move(getter), std::move(setter)};
		};
	}

	template <typename data_type, typename editor_implementation>
	void register_editor()
	{
		register_editor<data_type>([](data_writer<data_type> data_writer, std::shared_ptr<widget_factory> factory,
									   std::string_view name) -> std::unique_ptr<editor> {
			auto editor = std::make_unique<editor_implementation>(std::move(factory), name);
			editor->set_data_writer(std::move(data_writer));
			return editor;
		});
	}

	template <typename data_type>
	void unregister_viewer()
	{
		_viewer_factories.erase(std::type_index(typeid(data_reader<data_type>)).hash_code());
	}

	template <typename data_type>
	void unregister_editor()
	{
		_editor_factories.erase(std::type_index(typeid(data_writer<data_type>)).hash_code());
	}

	template <typename data_type>
	auto create(data_type const& data, std::string_view name) const -> std::unique_ptr<viewer>
	{
		return widget_factory::create(data_reader{&data}, name);
	}

	template <typename data_type>
	auto create(data_reader<data_type> data_reader, std::string_view name) const -> std::unique_ptr<viewer>
	{
		return widget_factory::create_viewer(std::any(std::move(data_reader)), name);
	}

	auto create(data_reader<void> data_reader, std::uint64_t data_hash, std::string_view name) const
		-> std::unique_ptr<viewer>
	{
		if(auto found_it = _nested_data_reader_factories.find(data_hash);
			found_it != _nested_data_reader_factories.end())
		{
			return create_viewer(found_it->second(std::move(data_reader)), name);
		}
		else
		{
			return std::make_unique<viewer_of<void>>(shared_from_this(), name);
		}
	}

	template <typename data_type>
	auto create(data_type& data, std::string_view name) const -> std::unique_ptr<editor>
	{
		return widget_factory::create(data_writer{&data}, name);
	}

	template <typename data_type>
	auto create(data_writer<data_type> data_writer, std::string_view name) const -> std::unique_ptr<editor>
	{
		return widget_factory::create_editor(std::any(std::move(data_writer)), name);
	}

	auto create(data_writer<void> data_writer, std::uint64_t data_hash, std::string_view name) const
		-> std::unique_ptr<editor>
	{
		if(auto found_it = _nested_data_writer_factories.find(data_hash);
			found_it != _nested_data_writer_factories.end())
		{
			return create_editor(found_it->second(std::move(data_writer)), name);
		}
		else
		{
			return std::make_unique<editor_of<void>>(shared_from_this(), name);
		}
	}

private:
	using viewer_factory = std::function<std::unique_ptr<viewer>(std::any, std::string_view)>;
	using editor_factory = std::function<std::unique_ptr<editor>(std::any, std::string_view)>;

	std::unordered_map<std::uint64_t, viewer_factory> _viewer_factories;
	std::unordered_map<std::uint64_t, std::any (*)(data_reader<void>)> _nested_data_reader_factories;
	std::unordered_map<std::uint64_t, editor_factory> _editor_factories;
	std::unordered_map<std::uint64_t, std::any (*)(data_writer<void>)> _nested_data_writer_factories;

	auto create_viewer(std::any data_reader, std::string_view name) const -> std::unique_ptr<viewer>
	{
		if(auto found_it = _viewer_factories.find(data_reader.type().hash_code()); found_it != _viewer_factories.end())
		{
			return found_it->second(std::move(data_reader), name);
		}
		else
		{
			return std::make_unique<viewer_of<void>>(shared_from_this(), name);
		}
	}

	auto create_editor(std::any data_writer, std::string_view name) const -> std::unique_ptr<editor>
	{
		if(auto found_it = _editor_factories.find(data_writer.type().hash_code()); found_it != _editor_factories.end())
		{
			return found_it->second(std::move(data_writer), name);
		}
		else
		{
			return std::make_unique<editor_of<void>>(shared_from_this(), name);
		}
	}
};

} // namespace clk::gui