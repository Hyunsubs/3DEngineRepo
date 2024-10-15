#pragma once


#define SINGLE(Type) private:\
						Type();\
						~Type();\
						friend class CSingleton<Type>;

#define DEVICE 	CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()
#define MAX_LAYER 32

#define DT CTimeMgr::GetInst()->GetDeltaTime()
#define EngineDT CTimeMgr::GetInst()->GetEngineDeltaTime()

#define KEY_CHECK(KEY, STATE) CKeyMgr::GetInst()->GetKeyState(KEY) == STATE
#define KEY_TAP(KEY) CKeyMgr::GetInst()->GetKeyState(KEY) == KEY_STATE::TAP
#define KEY_PRESSED(KEY) CKeyMgr::GetInst()->GetKeyState(KEY) == KEY_STATE::PRESSED
#define KEY_RELEASED(KEY) CKeyMgr::GetInst()->GetKeyState(KEY) == KEY_STATE::RELEASED

#define CLONE(Type) virtual Type* Clone(){return new Type(*this); }
#define CLONE_DISABLE(Type) virtual Type* Clone() { return nullptr; }

#define DELETE(p) if(nullptr != p) delete p; p = nullptr;


typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;