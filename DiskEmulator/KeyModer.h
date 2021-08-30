#pragma once

class CKeyModer {
private:
	HWND m_hwnd = nullptr;
	RECT m_rect;
	bool m_isPressUp = false;
	bool m_isPressDown = false;
	bool m_isPressLeft = false;
	bool m_isPressRight = false;
	bool m_isPressSpace = false;
	unsigned char m_speed = 1;

	void IncreaseSpeed();
	void DecreaseSpeed();
public:
	static CKeyModer* GetInstance() {
		static CKeyModer* instance = new CKeyModer();
		return instance;
	}
public:
	CKeyModer() {
		m_isPressUp = false;
		m_isPressDown = false;
		m_isPressLeft = false;
		m_isPressRight = false;
		m_isPressSpace = false;
		m_speed = 1;
	}

	void Initialize(HWND hwnd) {
		m_hwnd = hwnd;
	}
	void ProcKeyEvent();
	unsigned char GetSpeed();
};