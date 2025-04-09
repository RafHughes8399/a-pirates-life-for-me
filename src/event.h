
#pragma once
#include <string>
#include <map>
#include <functional>
#include <vector>
template <typename T>
class Event {
public:
	virtual  ~Event() = default;
	Event(T type, const std::string& name = "")
		: event_type_(type), name_(name) {
	};


	const T get_type() const;
	std::string& get_name();
	bool is_handled();
	template<typename EventType>
	inline EventType to_type() const {
		return event_type_;
	}



protected:
	T event_type_;
	std::string name_;
	bool handled_ = false;
};

template<typename T>
inline const T Event<T>::get_type() const
{
	return event_type_;
}

template<typename T>
inline std::string& Event<T>::get_name()
{
	return name_;
}

template<typename T>
inline bool Event<T>::is_handled()
{
	return handled_;
}


template<typename T>
class EventDispatcher{
public:
	using EventFunction = std::function<void(const Event<T>&)>;

	int add_listener(T type, const EventFunction& function) {
		event_listeners_[type].push_back(function);
		int handle = next_listener_id_++;
		listener_handles_[handle] = {type, std::prev(event_listeners_[type].end())};
		return handle;
	}

	void remove_listener(int handle) {
		auto it = listener_handles_.find(handle);
		if (it != listener_handles_.end()) {
			const auto& listener_info = it->second;
			auto& listeners = event_listeners_[listener_info.first];
			listeners.erase(listener_info.second);
			listener_handles_.erase(it);
		}
	}

	void send_event(const Event<T>& event){
		if (event_listeners_.find(event.get_type()) == event_listeners_.end()) {
			return;
		}
		for (auto& listener : event_listeners_.at(event.get_type())) {
			if (not event.is_handled()) { listener(event); }
		}
	}
private:
	std::map<T, std::vector<EventFunction>> event_listeners_;
	int next_listener_id_ = 0;
	std::map<int, std::pair<T, typename std::vector<EventFunction>::iterator>> listener_handles_;

};