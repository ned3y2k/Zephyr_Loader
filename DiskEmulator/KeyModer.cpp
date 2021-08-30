#include "pch.h"
#include <Windows.h>
#include "KeyModer.h"

void CKeyModer::ProcKeyEvent() {
	GetWindowRect(m_hwnd, &m_rect);

	if (GetAsyncKeyState(VK_F1)) {
		// DecreaseSpeed(); // 속도를 감속하면 다운 됨...
	}

	if (GetAsyncKeyState(VK_F2)) {
		IncreaseSpeed();
	}

	long center = (m_rect.left + m_rect.right) / 2;
	long mid = (m_rect.top + m_rect.bottom) / 2;

	if (m_hwnd == nullptr)
		return;

	if (GetAsyncKeyState(VK_UP) && !m_isPressUp) {
		SetCursorPos(center, m_rect.top + 30);
		mouse_event(MOUSEEVENTF_LEFTDOWN, center, m_rect.top + 30, 0, 0);
		m_isPressUp = true;
	}
	if (!GetAsyncKeyState(VK_UP) && m_isPressUp) {
		mouse_event(MOUSEEVENTF_LEFTUP, center, m_rect.top + 30, 0, 0);
		m_isPressUp = false;
	}


	if (GetAsyncKeyState(VK_DOWN) && !m_isPressDown) {
		SetCursorPos(center, m_rect.bottom - 30);
		mouse_event(MOUSEEVENTF_LEFTDOWN, center, m_rect.bottom - 30, 0, 0);
		m_isPressDown = true;
	}
	if (!GetAsyncKeyState(VK_DOWN) && m_isPressDown) {
		mouse_event(MOUSEEVENTF_LEFTUP, center, m_rect.bottom - 30, 0, 0);
		m_isPressDown = false;
	}

	if (GetAsyncKeyState(VK_LEFT) && !m_isPressLeft) {
		SetCursorPos(m_rect.left + 20, mid);
		mouse_event(MOUSEEVENTF_LEFTDOWN, m_rect.left + 20, mid, 0, 0);
		m_isPressLeft = true;
	}
	if (!GetAsyncKeyState(VK_LEFT) && m_isPressLeft) {
		mouse_event(MOUSEEVENTF_LEFTUP, m_rect.left + 20, mid, 0, 0);
		m_isPressLeft = false;
	}

	if (GetAsyncKeyState(VK_RIGHT) && !m_isPressRight) {
		SetCursorPos(m_rect.right - 20, mid);
		mouse_event(MOUSEEVENTF_LEFTDOWN, m_rect.right - 20, mid, 0, 0);
		m_isPressRight = true;
	}
	if (!GetAsyncKeyState(VK_RIGHT) && m_isPressRight) {
		mouse_event(MOUSEEVENTF_LEFTUP, m_rect.right - 20, mid, 0, 0);
		m_isPressRight = false;
	}

	if (GetAsyncKeyState(VK_SPACE) && !m_isPressSpace) {
		POINT p;
		GetCursorPos(&p);

		mouse_event(MOUSEEVENTF_LEFTDOWN, p.x, p.y, 0, 0);
		m_isPressSpace = true;
	}
	if (!GetAsyncKeyState(VK_RIGHT) && m_isPressSpace) {
		POINT p;
		GetCursorPos(&p);

		mouse_event(MOUSEEVENTF_LEFTUP, p.x, p.y, 0, 0);
		m_isPressSpace = false;
	}
}

void CKeyModer::IncreaseSpeed() {
	static char msg[] = "IncreaseSpeed 9";
	m_speed++;
	if (m_speed >= 3)
		m_speed = 3;

	msg[14] = 0x30 + m_speed;
	MessageBoxA(nullptr, msg, "", 0);
}

void CKeyModer::DecreaseSpeed() {
	static char msg[] = "DecreaseSpeed 0";
	m_speed--;
	if (m_speed <= 1)
		m_speed = 1;

	msg[14] = 0x30 + m_speed;
	MessageBoxA(nullptr, msg, "", 0);
}
unsigned char CKeyModer::GetSpeed() {
	return m_speed;
}