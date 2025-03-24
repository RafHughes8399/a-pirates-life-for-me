#pragma once
#include <vector>
#include <algorithm>
#include <memory>
class Observer {
public:
	virtual ~Observer() = default;
	Observer()
		: time_notified_(0.0) {
	};
	Observer(const Observer& other)
		: time_notified_(other.time_notified_) {
	};
	Observer(Observer&& other) = default;
	Observer& operator=(Observer&& other) = default;

	virtual void on_notify() = 0;// for now
	virtual Observer* clone() const = 0; 
	void set_time_notified(double time);
protected:
	double time_notified_;
};


class Notifier {
public:
	~Notifier() = default;
	Notifier() = default;
	Notifier(const Notifier& other) {
		for (auto& o : other.observers_) {
			observers_.push_back(std::unique_ptr<Observer>(o->clone()));
		}
	}
	Notifier(Notifier&& other) = default;

	Notifier& operator=(const Notifier& other);
	Notifier& operator=(Notifier&& other);

	void add_observer(std::unique_ptr<Observer> observer);
	void remove_observer(Observer* observer);
	Observer* get_observer(size_t index);
	void notify();

protected:
	std::vector<std::unique_ptr<Observer>> observers_;
};
