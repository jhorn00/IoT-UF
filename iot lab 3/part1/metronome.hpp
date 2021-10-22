#pragma once

#include <vector>
#include <chrono>
#include <cstddef>
using namespace std;

class metronome
{
	bool m_timing;
	unsigned int track = 0;
	chrono::milliseconds m_rate = chrono::milliseconds(0);
	chrono::high_resolution_clock::time_point m_t1 = chrono::high_resolution_clock::now();
	chrono::high_resolution_clock::time_point m_t2 = chrono::high_resolution_clock::now();

	// Insert new samples at the end of the array, removing the oldest
	size_t m_beat_count;
public:
	metronome()
		: m_timing(false), m_beat_count(0) {}
	~metronome() {}

	// Call when entering "learn" mode
	void start_timing();
	// Call when leaving "learn" mode
	void stop_timing();
	void set_rate(int rate);
	// Should only record the current time when timing
	// Insert the time at the next free position of m_beats
	void tap();

	bool is_timing() const { return m_timing; }
	// Calculate the BPM from the deltas between m_beats
	// Return 0 if there are not enough samples
	chrono::milliseconds get_rate() const { return m_rate; }
};

void metronome::start_timing() {
	//set this to help calculate the first difference
	m_t2 = chrono::high_resolution_clock::now();
	//timing true and count initialized
	m_timing = true;
	m_beat_count = 0;
	cout << "Mode Changed: Now Learning." << endl;
}

void metronome::stop_timing() {
	m_timing = false;
	cout << "Mode Changed: Metronome." << endl;
	//if not enough taps we have 0 rate
	if (m_beat_count < 4)
	{
		cout << "Not enough taps." << endl;
		return;
	}
	//otherwise we calculate the rate
	unsigned int sum = 0;
	m_rate = chrono::milliseconds(track);
	track = 0;
	m_beat_count = 0;
}

void metronome::set_rate(int rate)
{
	m_rate = chrono::milliseconds(rate);
}

void metronome::tap() {
	//m_timing is whether or not we are timing
	if (m_timing) {
		cout << "Tap Recorded." << endl;
		if (m_beat_count == 0)
		{
			m_t1 = chrono::high_resolution_clock::now();
			auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(m_t1 - m_t2);
			track = diff.count();
		}
		else
		{
			m_t2 = chrono::high_resolution_clock::now();
			auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(m_t2 - m_t1);
			track = (track + diff.count()) / 2;
			m_t1 = m_t2;
		}
		m_beat_count++;
	}
}


