#include "GameScene.h"
#include"../SceneManager.h"
#include "../../main.h"

#include "../../GameObject/Terrain/Terrain.h"
#include "../../GameObject/Character/Character.h"

#include "../../GameObject/Camera/FPSCamera/FPSCamera.h"
#include "../../GameObject/Camera/TPSCamera/TPSCamera.h"
#include "../../GameObject/Camera/CCTVCamera/CCTVCamera.h"

// 少数第n位で四捨五入する
void round_n(float& number, int n)
{
	number = number * (float)pow(10, n - 1);
	number = round(number);
	number /= (float)pow(10, n - 1);
}

void GameScene::Init()
{
	//===================================================================
	// ステージ初期化
	//===================================================================
	std::shared_ptr<Terrain> _terrain = std::make_shared<Terrain>();
	_terrain->Init();
	AddObject(_terrain);

	//===================================================================
	// キャラクター初期化
	//===================================================================
	std::shared_ptr<Character> _character = std::make_shared<Character>();
	_character->Init();
	AddObject(_character);

	//===================================================================
	// カメラ初期化
	//===================================================================
//	std::shared_ptr<FPSCamera>		_camera = std::make_shared<FPSCamera>();
//	std::shared_ptr<TPSCamera>		_camera = std::make_shared<TPSCamera>();
	std::shared_ptr<CCTVCamera>		_camera = std::make_shared<CCTVCamera>();
	_camera->Init();
	_camera->SetTarget(_character);
	_camera->RegistHitObject(_terrain);
	_character->SetCamera(_camera);
	AddObject(_camera);

	//===================================================================
	// 乱数実験場
	//===================================================================
	//int randRes[10] = {};

	//// なぜrand()が偏るか
	//srand((unsigned)time(NULL));

	//// ランダムはだいだい10万回回す
	//for (int i = 0; i < 100000000; i++)
	//{
	//	int tmp = rand() % 10000;
	//	int idx = tmp / 1000;

	//	randRes[idx]++;
	//}
	//
	//// 結果を出力
	//OutputDebugStringA("----------------------------\n");
	//for (int i = 0; i < 10; i++)
	//{
	//	std::stringstream ss;
	//	ss << "取得した値が" << i * 1000 << "～" << (i + 1) * 1000 <<
	//		"の件数 : \t" << randRes[i] << "\n";
	//	std::string str = ss.str();
	//	OutputDebugStringA(str.c_str());
	//}
	//OutputDebugStringA("----------------------------\n");

	// メルセンヌついスタ！！！！！
	// ランダムはだいだい10万回回す
	//for (int i = 0; i < 100000000; i++)
	//{
	//	int tmp = KdGetInt(0,9999);
	//	int idx = tmp / 1000;

	//	randRes[idx]++;
	//}
	//
	//// 結果を出力
	//OutputDebugStringA("----------------------------\n");
	//for (int i = 0; i < 10; i++)
	//{
	//	std::stringstream ss;
	//	ss << "取得した値が" << i * 1000 << "～" << (i + 1) * 1000 <<
	//		"の件数 : \t" << randRes[i] << "\n";
	//	std::string str = ss.str();
	//	OutputDebugStringA(str.c_str());
	//}
	//OutputDebugStringA("----------------------------\n");

	// レッスンその1 : CカードとRカードをそれぞれ50％の確率で起動時に表示せよ！
	// メルセンヌツイスタ！！！！！
	//OutputDebugStringA("----------------------------\n");
	//int i = KdGetInt(0, 1);
	//std::stringstream ss;
	//if (i == 0)
	//{
	//	ss << "Cカード" << "\n";
	//}
	//else
	//{
	//	ss << "Rカード" << "\n";
	//}
	//std::string str = ss.str();
	//OutputDebugStringA(str.c_str());
	//
	//OutputDebugStringA("----------------------------\n");


	//// レッスンその2 : Cカードは99.5%、Rカードは0.5%で当選させる
	//int _bunbo			= 1000;
	//int _ThusenNum		= 10000000;
	//int _TosenNum[2]	= { 0,0 };

	//for (int i = 0; i < _ThusenNum; i++)
	//{
	//	int _rnd = KdGetFloat(0, _bunbo - 1);

	//	if (_rnd <= 4)
	//	{
	//		_TosenNum[1]++;
	//	}
	//	else
	//	{
	//		_TosenNum[0]++;
	//	}
	//}

	//OutputDebugStringA("----------------------------\n");
	//float prob = 0;
	//for (int i = 0; i < 2; i++)
	//{
	//	std::stringstream ss;
	//	prob = ((float)_TosenNum[i] / (float)_ThusenNum) * 100;
	//	round_n(prob, 3);
	//	switch (i)
	//	{
	//	case 0:
	//		ss << "Cカード当選回数" << _TosenNum[0] << " " <<
	//			"当選確率 = " << prob << "%" << "\n";
	//		break;
	//	case 1:
	//		ss << "Rカード当選回数" << _TosenNum[1] << " " <<
	//			"当選確率 = " << prob << "%" << "\n";
	//		break;
	//	}
	//	std::string str = ss.str();
	//	OutputDebugStringA(str.c_str());
	//}
	//OutputDebugStringA("----------------------------\n");

	//// レッスンその3 : C,R,SRカードをそれぞれ 34%,33%,33%の確率で起動時に表示せよ!
	//
	//// エクセルに確率を格納

	//// サーバーで処理をして、クライアントに提供

	//int _bunbo			= 100;
	//int _randNum[3]		= {34,33,33};
	//int _ThusenNum		= 10000000;
	//int _TosenNum[3]	= { 0,0,0 };

	//for (int i = 0; i < _ThusenNum; i++)
	//{
	//	int _rnd = KdGetInt(0, _bunbo - 1);

	//	for (int j = 0; j < sizeof(_randNum); j++)
	//	{
	//		_rnd -= _randNum[j];

	//		if (_rnd < 0)
	//		{
	//			_TosenNum[j]++;
	//			break;
	//		}

	//	}
	//}

	// レッスンその4 : C,R,SRカードをそれぞれ 50%,49.5%,0.5%の確率で起動時に表示せよ!
	
	// エクセルに確率を格納

	// サーバーで処理をして、クライアントに提供

	int _bunbo			= 1000;
	int _randNum[3]		= {500,495,5};
	int _ThusenNum		= 10000000;
	int _TosenNum[3]	= { 0,0,0 };

	for (int i = 0; i < _ThusenNum; i++)
	{
		int _rnd = KdGetInt(0, _bunbo - 1);

		for (int j = 0; j < std::size(_randNum); j++)
		{
			_rnd -= _randNum[j];

			if (_rnd < 0)
			{
				_TosenNum[j]++;
				break;
			}

		}
	}

	OutputDebugStringA("----------------------------\n");
	float prob = 0;
	for (int i = 0; i < std::size(_randNum); i++)
	{
		std::stringstream ss;
		prob = ((float)_TosenNum[i] / (float)_ThusenNum) * 100;
		round_n(prob, 3);
		switch (i)
		{
		case 0:
			ss << "Cカード当選回数" << _TosenNum[0] << " " <<
				"当選確率 = " << prob << "%" << "\n";
			break;
		case 1:
			ss << "Rカード当選回数" << _TosenNum[1] << " " <<
				"当選確率 = " << prob << "%" << "\n";
			break;
		case 2:
			ss << "SRカード当選回数" << _TosenNum[2] << " " <<
				"当選確率 = " << prob << "%" << "\n";
			break;
		}
		std::string str = ss.str();
		OutputDebugStringA(str.c_str());
	}
	OutputDebugStringA("----------------------------\n");
}

void GameScene::Event()
{
}