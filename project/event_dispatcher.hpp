#pragma once

// http://quick-bench.com/cUIGOFxcLFmOQzJeap3XC3pPkEI

#include <unordered_map>
#include <functional>
#include <typeinfo>
#include <typeindex>
#include <any>
#include <queue>

namespace goth2018::event
{
	template <typename event_t>
	struct trait
	{
		using event_type = event_t;
		using handler_type = typename std::function<void(event_type const &)>;
	};

	class static_dispatcher
	{
		struct channel
		{	// create a safe y-shapped inheritance tree
			// that leads to "un-templatized" container_type<T>
			// NB : type-safety relies on caller
			struct base
			{	// EBO
				virtual ~base() {}
			};

			template <typename T>
			using container_type = typename std::vector<typename trait<T>::handler_type>; // deque

			template <typename T>
			struct impl : base, container_type<T>
			{};

			template <typename T>
			static inline container_type<T> & cast(base & channel) noexcept
			{	// resolve type
				auto & y_node = static_cast<impl<T>&>(channel);
				return static_cast<container_type<T>&>(y_node);
			}
			template <typename T>
			static inline const container_type<T> & cast(const base & channel) noexcept
			{	// resolve type
				const auto & y_node = static_cast<const impl<T>&>(channel);
				return static_cast<const container_type<T>&>(y_node);
			}
		};

		using container_type = std::unordered_map<std::type_index, std::unique_ptr<channel::base>>;
		container_type content;

	public:
		template
		<
			typename message_type,
			typename handler_type,
			typename std::enable_if_t<!std::is_same_v<std::decay_t<message_type>, std::any>> * = nullptr
		>
		void add_listener(handler_type && handler)
		{
			auto & channel = access<message_type>();
			auto & element = channel.emplace_back(std::forward<handler_type>(handler));

			// accessor, for removal ?
		}

		template
		<
			typename message_type,
			typename std::enable_if_t<!std::is_same_v<std::decay_t<message_type>, std::any>> * = nullptr
		>
		void direct_dispatch(message_type && value) const
		{	// static typing dispatch
			const auto & channel = access<message_type>();
			for (const auto & elem : channel)
			{
				elem(value);
			}
		}

		template <typename T>
		channel::container_type<T> & access()
		{
			const std::type_index index = typeid(T);

			try
			{
				return channel::cast<T>(*content.at(index));
			}
			catch (const std::out_of_range &)
			{
				content.emplace(index, new channel::impl<T>());
				return channel::cast<T>(*content.at(index));
			}
		}
		template <typename T>
		const channel::container_type<T> & access() const
		{
			const std::type_index index = typeid(T);

			try
			{
				return channel::cast<T>(*content.at(index));
			}
			catch (const std::out_of_range &)
			{	// on-purpose constness violation
				// std::remove_const_t does not work ...
				const_cast<static_dispatcher*>(this)->content.emplace(index, new channel::impl<T>());
				return channel::cast<T>(*content.at(index));
			}
		}
	};
}