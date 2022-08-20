#ifndef CIRCUITPET_FIRMWARE_STATE_H
#define CIRCUITPET_FIRMWARE_STATE_H


class State {
public:
	State(State* parent = nullptr);
	virtual ~State() = default;

	virtual void start();
	virtual void stop();

	bool isStarted() const;

	void push(State* parent);
	virtual void pop();

protected:
	virtual void onStart() = 0;
	virtual void onStop() = 0;
	State* getParent() const;

private:
	State* parent = nullptr;
	bool started = false;

};


#endif //CIRCUITPET_FIRMWARE_STATE_H