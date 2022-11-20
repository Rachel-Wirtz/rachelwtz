#ifndef RACHELWTZ__THREADING__SHARED_WRAPPER__HPP
#define RACHELWTZ__THREADING__SHARED_WRAPPER__HPP
#include "rw-threading.hpp"
RACHELWTZ_THREADING_BEGIN

#include <mutex>

// 
// Provides a simple interface to turn non-atomic types thread safe by using locks.
// 
// T: Value Type to be stored and protected
// M: Mutex Type to be used. Behavior and usaged should be similar to std::mutex.
// L: Lock Type to be used. Behavior and usaged should be similar to std::scoped_lock.
// 
// Usage:
// shared_wrapper<std::vector<int>> v = std::initializer_list{ 10, 20, 30, 40 };
// std::size_t size = v([](const auto& vec) noexcept -> std::size_t {
//     return vec.size();
// });
// 
template<typename T, typename M = std::mutex, typename L = std::scoped_lock<M>>
class shared_wrapper {
public:
	using value_type = T;
	using mutex_type = M;
	using lock_type  = L;

	// 
	// Default constructor is available if T is default constructible and declared noexcept(true) if the T is nothrow default constructible.
	// Note: Ignoring potentially throwing M constructor, so std::terminate is being called if throwing.
	// 
	shared_wrapper() noexcept(std::is_nothrow_default_constructible_v<value_type>) requires std::is_default_constructible_v<value_type>
		: m_Mtx{}
		, m_Data{}
	{
	}

	// 
	// Copy constructor is available if T is copy constructible and declared noexcept(true) if the T is nothrow copy constructible.
	// Note: Ignoring potentially throwing M constructor, so std::terminate is being called if throwing.
	// 
	shared_wrapper(const shared_wrapper& other) noexcept(std::is_nothrow_copy_constructible_v<value_type>) requires std::is_copy_constructible_v<value_type>
		: m_Mtx{}
		, m_Data(locked_reference(other))
	{
	}

	// 
	// Move constructor is available if T is move constructible and declared noexcept(true) if the T is nothrow move constructible.
	// Note: Ignoring potentially throwing M constructor, so std::terminate is being called if throwing.
	// 
	shared_wrapper(shared_wrapper&& other) noexcept(std::is_nothrow_move_constructible_v<value_type>) requires std::is_move_constructible_v<value_type>
		: m_Mtx{}
		, m_Data(locked_reference(std::forward<shared_wrapper>(other)))
	{
	}

	// 
	// Variadic templated constructor offering in place construction of T and declared nothrow if T::T(Args...) is declared not throwing.
	// Note: Ignoring potentially throwing M constructor, so std::terminate is being called if throwing.
	// 
	template<typename ... Args> requires std::is_constructible_v<value_type, Args...>
	shared_wrapper(Args&& ... args) noexcept(std::is_nothrow_constructible_v<value_type, Args...>)
		: m_Mtx{}
		, m_Data(std::forward<Args>(args)...)
	{
	}

	// 
	// Destructor. For completeness following explicit nothrow declaration according to T::~T().
	// 
	~shared_wrapper() noexcept(std::is_nothrow_destructible_v<value_type>) requires std::is_destructible_v<value_type> {

	}

	// 
	// Copy assignment operator is available if T is copy assignable and declared noexcept(true) if T is nothrow copy assignable.
	// Note: Ignoring potentially throwing L constructor, so std::terminate is being called if throwing.
	// 
	shared_wrapper& operator=(const shared_wrapper& other) noexcept(std::is_nothrow_copy_assignable_v<value_type>) requires std::is_copy_assignable_v<value_type> {
		if (this != &other) {
			lock_type lock(m_Mtx, other.m_Mtx);
			m_Data = other.m_Data;
		}
		return *this;
	}

	// 
	// Move assignment operator is available if T is move assignable and declared noexcept(true) if T is nothrow move assignable.
	// Note: Ignoring potentially throwing L constructor, so std::terminate is being called if throwing.
	// 
	shared_wrapper& operator=(shared_wrapper&& other) noexcept(std::is_nothrow_move_assignable_v<value_type>) requires std::is_move_assignable_v<value_type> {
		if (this != &other) {
			lock_type lock(m_Mtx, other.m_Mtx);
			m_Data = std::move(other.m_Data);
		}
		return *this;
	}

	// 
	// Variadic templated assginment operator offering direct assignment to underlaying T and declared nothrow accordingly.
	// Note: Ignoring potentially throwing L constructor, so std::terminate is being called if throwing.
	// 
	template<typename T> requires std::is_assignable_v<value_type, T>
	shared_wrapper& operator=(T&& arg) noexcept(std::is_nothrow_assignable_v<value_type, T>) {
		lock_type lock(m_Mtx);
		m_Data = std::forward<T>(arg);
		return *this;
	}

	// 
	// Function call operator acting as accessor to the underlaying T. Declared nothrow if the passed function is nothrow.
	// Note: Ignoring potentially throwing L constructor, so std::terminate is being called if throwing.
	// 
	// F: auto(value_type&) [throw|noexcept]
	// Passing the return value of F back to the callee.
	//
	template<typename F> requires std::is_invocable_v<F, value_type&>
	auto operator()(F&& f) noexcept(std::is_nothrow_invocable_v<F, value_type&>) -> std::invoke_result_t<F, value_type&> {
		lock_type lock(m_Mtx);
		if constexpr (!std::is_void_v<std::invoke_result_t<F, value_type&>>) {
			return f(m_Data);
		}
		else f(m_Data);
	}

	// 
	// Function call operator acting as accessor to the underlaying T. Declared nothrow if the passed function is nothrow.
	// Note: Ignoring potentially throwing L constructor, so std::terminate is being called if throwing.
	// 
	// F: auto(const value_type&) [throw|noexcept]
	// Passing the return value of F back to the callee.
	//
	template<typename F> requires std::is_invocable_v<F, const value_type&>
	auto operator()(F&& f) const noexcept(std::is_nothrow_invocable_v<F, const value_type&>) -> std::invoke_result_t<F, const value_type&> {
		lock_type lock(m_Mtx);
		if constexpr (!std::is_void_v<std::invoke_result_t<F, const value_type&>>) {
			return f(m_Data);
		}
		else f(m_Data);
	}

protected:
	//
	// Private utility function used in copy constructor to avoid double initialization.
	// Locks M and returns the reference of the other's underlaying T to be copied into the target T.
	//
	const value_type& locked_reference(const shared_wrapper& other) noexcept {
		lock_type lock(other.m_Mtx);
		return other.m_Data;
	}

	//
	// Private utility function used in move constructor to avoid double initialization.
	// Locks M and returns the reference of the other's underlaying T to be moved into the target T.
	//
	value_type&& locked_reference(shared_wrapper&& other) noexcept {
		lock_type lock(other.m_Mtx);
		return std::move(other.m_Data);
	}

    value_type         m_Data;
	mutable mutex_type m_Mtx;
};

RACHELWTZ_THREADING_END
#endif