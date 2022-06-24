#ifndef CIRCUITPET_FIRMWARE_STATE_H
#define CIRCUITPET_FIRMWARE_STATE_H


class State {
public:
	State(State* parent = nullptr);
	virtual ~State() = default;

	void start();
	void stop();

	void push(State* parent);
	void pop();

protected:
	virtual void onStart() = 0;
	virtual void onStop() = 0;

private:
	State* parent = nullptr;
	bool started = false;

};


#endif //CIRCUITPET_FIRMWARE_STATE_H
