#pragma once

#ifndef GCL_CONTAINER__POLYMORPHIC_VECTOR_HPP__
# define GCL_CONTAINER__POLYMORPHIC_VECTOR_HPP__

#include <gcl_cpp/type_info.hpp>

#include <unordered_map>
#include <vector>
#include <functional>
#include <any>
#include <typeindex>

namespace gcl::container::v2
{
	struct holder final
	{	// hold a value anonymously
		// cost much more than std::any
		struct interface_t
		{	// as interface, for safe y-shaped inheritance tree (type erasure)
			interface_t() = default;
			interface_t(const interface_t &) = delete;
			interface_t(interface_t &&) = delete;
			virtual ~interface_t() = default;
		};
		using value_type = std::unique_ptr<interface_t>;
		template <typename T>
		struct type_eraser : interface_t, std::decay_t<T>
		{
			using target_type = std::decay_t<T>;

			type_eraser(target_type && value)
				: target_type{ std::forward<target_type>(value) }
			{}
		};

		holder() = delete;
		holder(holder &) = delete;
		holder(holder &&) = default;

		template <typename concret_t>
		holder(concret_t && elem)
			: value(std::unique_ptr<interface_t>(new type_eraser<concret_t>{ std::forward<concret_t>(elem) }))
			, id(type_info::id<concret_t>::value)
		{}

		template <typename T>
		T & cast() const
		{
			using target_type = std::decay_t<T>;

			if (id != gcl::type_info::id<target_type>::value)
				throw std::bad_cast();

			auto * as_type_eraser_ptr = static_cast<type_eraser<target_type&>*>(value.get());
			return static_cast<target_type&>(*as_type_eraser_ptr);
		}

		const gcl::type_info::id_type id;
		const value_type value;
	};

	namespace test
	{
		struct holder
		{
			struct interface_and_cast
			{
				static void proceed()
				{
					struct A {};
					struct B {};

					gcl::container::v2::holder holder_A_1{ A{} };
					gcl::container::v2::holder holder_A_2{ A{} };

					gcl::container::v2::holder holder_B_2{ B{} };

					GCL_TEST__EXPECT_VALUE(holder_A_1.id, holder_A_2.id);
					GCL_TEST__EXPECT_NOT_VALUE(holder_A_1.id, holder_B_2.id);

					GCL_TEST__EXPECT_EXCEPTION(std::bad_cast, [&holder_A_1]() { holder_A_1.cast<B>(); });
					A & value = holder_A_1.cast<A>();
				}
			};
			struct cleaning
			{
				static void proceed()
				{
					bool is_a_destructor_called_properly = false;
					struct A
					{
						bool & called;
						~A() { called = true; }
					};

					{
						gcl::container::v2::holder holder_A{ A{ is_a_destructor_called_properly } };
					}
					GCL_TEST__EXPECT(is_a_destructor_called_properly);
				}
			};
			using dependencies_t = std::tuple<interface_and_cast, cleaning>;
		};
	}

	struct polymorphic_vector
	{	// ordered container with type erasure, and type-grouped operations
		// e.g for (auto && elem : polymorphic_vector::get<my_type>())

		using value_type = std::any;
		using value_type_ptr = std::unique_ptr<value_type>;
		using value_type_raw_ptr = value_type * ;

		template <typename ... values_t>
		polymorphic_vector(values_t && ... values)
			// : content(sizeof...(values_t))
		{
			(push_back(std::forward<values_t>(values)), ...);
		}

		template <typename T, typename ... Args>
		T & emplace_back(Args&&... args)
		{
			static_assert((std::is_constructible_v<T, Args&&> && ...));

			push_back(std::move(T{ std::forward<Args>(args)... }));
			return std::any_cast<T&>(*(content.back().get()));
		}
		template <typename T>
		void push_back(T && obj)
		{
			content.push_back(std::make_unique<std::any>(std::forward<T>(obj)));
			auto & elem = *content.back();
			content_sorted_accessor[elem.type()].emplace_back(&elem);
		}

		template <typename T>
		inline auto const & get() const
		{
			return content_sorted_accessor.at(typeid(T));
		}
		inline auto const & get(const std::type_index & index) const
		{
			return content_sorted_accessor.at(index);
		}
		inline auto const & get() const
		{
			return content;
		}

		void visit(std::function<void(const value_type &)> func) const
		{
			for (const auto & elem : content)
			{
				func(*elem);
			}
		}
		void visit(std::function<void(value_type &)> func)
		{
			for (auto & elem : content)
			{
#ifdef _DEBUG // enforce type safety for debug only
				const std::type_index type_id = elem->type();
				func(*elem);
				if (std::type_index{ elem->type() } != type_id)
					throw std::bad_typeid();
#else
				func(*elem);
#endif
			}
		}
		template <typename T>
		void visit(std::function<void(const T &)> func) const
		{
			for (const auto & elem : content_sorted_accessor.at(typeid(T)))
			{
				func(std::any_cast<const T&>(*elem));
			}
		}
		template <typename T>
		void visit(std::function<void(T &)> func)
		{
			for (auto & elem : content_sorted_accessor.at(typeid(T)))
			{
				func(std::any_cast<T&>(*elem));
			}
		}

		template <typename T>
		void remove(const T & value)
		{
			auto & container = content_sorted_accessor.at(typeid(T));
			auto container_remover = [&value](const std::decay_t<decltype(container)>::value_type & elem)
			{
				return std::any_cast<const T&>(*elem) == value;
			};
			container.erase
			(
				std::remove_if(std::begin(container), std::end(container), container_remover),
				std::end(container)
			);
			auto content_remover = [&value, type_id = typeid(T)](const std::decay_t<decltype(container)>::value_type & elem)
			{
				return elem->type() == type_id && std::any_cast<const T&>(*elem) == value;
			};
			content.erase
			(
				std::remove_if(std::begin(content), std::end(content), content_remover),
				std::end(content)
			);
		}
		template <typename T>
		void remove()
		{
			auto & container = content_sorted_accessor.at(typeid(T));
			container.clear();
			auto remover = [](const std::decay_t<decltype(content)>::value_type & elem)
			{
				return elem->type() == typeid(T);
			};
			content.erase
			(
				std::remove_if(std::begin(content), std::end(content), remover),
				std::end(content)
			);
		}

		void remove_if(std::function<bool(const value_type &)> func)
		{
			auto content_sorted_accessor_remover = [&func](const value_type_raw_ptr & elem)
			{
				return func(*elem);
			};

			for (auto & content_accessor : content_sorted_accessor)
			{
				content_accessor.second.erase
				(
					std::remove_if(std::begin(content_accessor.second), std::end(content_accessor.second), content_sorted_accessor_remover),
					std::end(content_accessor.second)
				);
			}

			auto content_remover = [&func](const value_type_ptr & elem)
			{
				return func(*elem);
			};

			content.erase
			(
				std::remove_if(std::begin(content), std::end(content), content_remover),
				std::end(content)
			);
		}
		template <typename T>
		void remove_if(std::function<bool(const T &)> func)
		{
			auto & container = content_sorted_accessor.at(typeid(T));
			auto container_remover = [&func](const std::decay_t<decltype(container)>::value_type & elem)
			{
				return func(std::any_cast<const T&>(*elem));
			};
			container.erase
			(
				std::remove_if(std::begin(container), std::end(container), container_remover),
				std::end(container)
			);

			auto content_remover = [&func, type_id = typeid(T)](const std::decay_t<decltype(content)>::value_type & elem)
			{
				return elem->type == type_id && func(std::any_cast<const T&>(*elem));
			};
			content.erase
			(
				std::remove_if(std::begin(content), std::end(content), remover),
				std::end(content)
			);
		}

	private:

		using content_t = std::vector<value_type_ptr>;
		using content_sorted_per_type_t = std::unordered_map<std::type_index, std::vector<value_type_raw_ptr> >;

		content_t					content;
		content_sorted_per_type_t	content_sorted_accessor;
	};

	namespace test
	{
		struct polymorphic_vector
		{
			struct A
			{
				A(std::string && s = "")
					: str{std::forward<std::string>(s)}
				{}
				std::string str;
			};
			struct B {};
			struct C {};

			using type = gcl::container::v2::polymorphic_vector;

			struct create_and_get
			{
				static void proceed()
				{
					type container{ A{}, B{}, A{"titi"} };

					container.push_back(A{});
					container.push_back(B{});

					container.emplace_back<A>("toto");

					GCL_TEST__EXPECT_VALUE(container.get().size(), 6);
					GCL_TEST__EXPECT_VALUE(container.get<A>().size(), 4);
				}
			};
			struct visit
			{
				static inline type container_value { A{ "foo" }, B{}, A{ "toto" }, B{}, C{}, A{ "bar" }, A{ "titi" } };

				struct visit_any_const
				{
					static void proceed()
					{
						const auto & container = container_value;

						std::size_t any_visitor_counter{ 0 };
						container.visit([&any_visitor_counter](const std::any &) { ++any_visitor_counter; });

						GCL_TEST__EXPECT_VALUE(any_visitor_counter, container.get().size());
						GCL_TEST__EXPECT_VALUE(any_visitor_counter, 7);
					}
				};
				struct visit_any_with_type_safety
				{
					static void proceed()
					{
						{	// change std::any underlying type
							type container{ A{ "foo" } };

							GCL_TEST__EXPECT_EXCEPTION(std::bad_typeid, [&container]()
							{
								container.visit([](std::any & value)
								{
									value = B{};
								});
							});
						}
						{	// not changing std::any underlying type
							type container{ A{ "foo" } };

							container.visit([](std::any & value)
							{
								value = A{ "bar" };
							});

							const auto & first_element = container.get<A>().at(0);
							const auto & first_element_as_A = std::any_cast<const A &>(*first_element);
							GCL_TEST__EXPECT_VALUE("bar", first_element_as_A.str);
						}
					}
				};
				struct visit_type_const
				{
					static void proceed()
					{
						const auto & container = container_value;

						std::size_t A_visitor_counter{ 0 };
						std::size_t A_visitor_start_with_t_counter{ 0 };
						container.visit<A>([&A_visitor_counter, &A_visitor_start_with_t_counter](const A & value)
						{
							if (!value.str.empty() && value.str.at(0) == 't')
								++A_visitor_start_with_t_counter;
							++A_visitor_counter;
						});

						GCL_TEST__EXPECT_VALUE(A_visitor_counter, container.get<A>().size());
						GCL_TEST__EXPECT_VALUE(A_visitor_counter, 4);
						GCL_TEST__EXPECT_VALUE(A_visitor_start_with_t_counter, 2);
					}
				};
				struct visit_type
				{
					static void proceed()
					{
						// type cannot be copied (cf std::unique_ptr ctors)
						type container{ A{ "foo" }, B{}, A{ "toto" }, B{}, C{}, A{ "bar" }, A{ "titi" } };

						container.visit<A>([](A & value)
						{
							value.str = "foobar";
						});

						std::size_t A_visitor_counter{ 0 };
						container.visit<A>([&A_visitor_counter](const A & value)
						{
							if (value.str == "foobar")
								++A_visitor_counter;
						});

						GCL_TEST__EXPECT_VALUE(A_visitor_counter, container.get<A>().size());
					}
				};
				

				using dependencies_t = std::tuple
				<
					visit_any_const,
					visit_any_with_type_safety,
					visit_type_const,
					visit_type
				>;
			};

			using dependencies_t = std::tuple<create_and_get, visit>;
		};
	}
}

#endif // GCL_CONTAINER__POLYMORPHIC_VECTOR_HPP__
