#include <iostream>
#include <type_traits>
#include <vector>

class Observer {
public:
	void notify() {
		std::cout << "Observer: Notified" << std::endl;
	}

	Observer() = default;
	Observer(const Observer &lhs) = delete;
	Observer(Observer &&lhs) { std::cout << "Move constructed Observer" << std::endl; }
	~Observer() { std::cout << "Observer destructed" << std::endl; }
};

template <typename T>
struct has_notify {
	typedef char yes[1];
	typedef char no[2];
	template <typename Y> static yes & has_reqs(Y * y, decltype(y->notify()) * = nullptr);
	template <typename> static no & has_reqs(...);
	static const bool value = sizeof(has_reqs<T>(nullptr)) == sizeof(yes);
};

class CallbackSaver {
public:
	template <typename O, typename = std::enable_if_t<has_notify<O>::value>>
	void addObserver(O && observer) {
		std::cout << "CallbackSaver: Adding observer" << std::endl;
		m_observers.emplace_back(new O(std::move(observer)), notifyFunctionTemplate<O>, destructorFunctionTemplate<O>);
		std::cout << "CallbackSaver: Added observer" << std::endl;
	}

	void notifyAll() {
		for (DegeneratedObserver &observer : m_observers) {
			observer.notifyFunction(observer.observer);
		}
	}

private:
	template <typename O>
	static void notifyFunctionTemplate(void *observer) {
		static_cast<O*>(observer)->notify();
	}

	template <typename O>
	static void destructorFunctionTemplate(void *observer) {
		delete static_cast<O*>(observer);
	}

	struct DegeneratedObserver {
		void *observer;
		void(*notifyFunction)(void*);
		void(*destructorFunction)(void*);

		DegeneratedObserver(void * observer, void (*notifyFunction)(void*), void(*destructorFunction)(void*))
			: observer(observer), notifyFunction(notifyFunction), destructorFunction(destructorFunction) {;}
		~DegeneratedObserver() { destructorFunction(observer); }
	};
	std::vector<DegeneratedObserver> m_observers;
};

int main() {
	Observer observer;
	CallbackSaver callbackSaver;
	callbackSaver.addObserver(std::move(observer));
	callbackSaver.notifyAll();
}