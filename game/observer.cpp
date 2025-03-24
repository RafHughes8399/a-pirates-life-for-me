#include "observer.h"
#include "observer.h"


void Notifier::add_observer(std::unique_ptr<Observer> observer) {
	observers_.push_back(std::move(observer));
}

void Notifier::remove_observer(Observer* observer) {
	observers_.erase(std::remove_if(observers_.begin(), observers_.end(),
		[&](const std::unique_ptr<Observer>& o) { return o.get() == observer; }), observers_.end());
}
Observer* Notifier::get_observer(size_t index){
	return observers_.at(index).get();
}

void Notifier::notify() {
	for (auto & o : observers_) {
		o->on_notify();
	}
}

void Observer::set_time_notified(double time){
	time_notified_ = time;
}
