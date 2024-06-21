#include "CameraBase.h"

void CameraBase::Init()
{
	// nullチェックを忘れずに
	if (!m_spCamera)
	{
		m_spCamera = std::make_shared<KdCamera>();
	}

	// マウスを固定させる座標値
	// 理想はアプリケーションから画面の座標値を取得してくる
	m_FixMousePos.x = 640;
	m_FixMousePos.y = 320;
}

void CameraBase::Update()
{
	if (!m_spCamera) { return; }

	m_spCamera->SetCameraMatrix(m_mWorld);
}

void CameraBase::PreDraw()
{
	if (!m_spCamera) { return; }

	m_spCamera->SetToShader();
}

void CameraBase::UpdateRotateByMouse()
{
	// ①マウスの現在の位置を取得する
	POINT _nowPos;
	GetCursorPos(&_nowPos);

	// ②画面中央からの差分を算出する
	POINT _mouseMove{};

	_mouseMove.x = _nowPos.x - m_FixMousePos.x;
	_mouseMove.y = _nowPos.y - m_FixMousePos.y;

	// ③画面の中央にカーソルを戻す
	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// ④差分を利用して回転角度を設定する
	// DegのX回転なので、マウス差分は縦
	// DegのY回転なので、マウスは横

	const float correction = 0.15f;			// これがゲームでいる感度
	m_DegAng.x += _mouseMove.y * correction;
	m_DegAng.y += _mouseMove.x * correction;

	// 回転制御

	// X軸回転の最大回転角度、最小回転角度
	const float maxRotX = 45.0f, minRotX = -45.0f;
	m_DegAng.x = std::clamp(m_DegAng.x, minRotX, maxRotX);
}
